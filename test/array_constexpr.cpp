/* tests using constexpr on boost:array
 * (C) Copyright Antony Polukhin 2015
 * (C) Copyright Marshall Clow 2012
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <string>
#include <iostream>
#include <boost/array.hpp>
#include <algorithm>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#ifndef BOOST_NO_CXX11_CONSTEXPR
constexpr boost::array<int, 10> arr  {{ 0,1,2,3,4,5,6,7,8,9 }};

template <typename T>
void sink ( T t ) {}

template <typename T, size_t N>
void sink ( boost::array<T,N> &arr ) {}

BOOST_AUTO_TEST_CASE( test_main ) {
    constexpr int three = *(arr.begin() + 3);
    int whatever [ arr.back() ];
    (void)three;
    (void) whatever;
}

#ifndef BOOST_NO_CXX14_CONSTEXPR
constexpr boost::array<int, 10> constexpr_test_function(const boost::array<int, 10>& in) {
    boost::array<int, 10> res = in;
    res.front() = res.back();
    res[2] = res.at(res.size() - 2);
    res[3] = *(res.data() + res[4]);
    boost::get<4>(res) = boost::get<5>(res);

    return res;
}

constexpr bool is_equal_arrays(const boost::array<int, 10>& lhs, const boost::array<int, 10>& rhs) {
    for (boost::array<int, 10>::size_type i = 0; i < 10; ++i) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }

    return true;
}

BOOST_AUTO_TEST_CASE( test_constexpr_advanced )
{
    constexpr boost::array<int, 10> cx14 {{ 0,1,2,3,4,5,6,7,8,9 }};
    constexpr boost::array<int, 10> res = constexpr_test_function(cx14);
    constexpr boost::array<int, 10> ethalon {{ 9,1,8,4,5,5,6,7,8,9 }};
    // We can not use `operator ==` with constexpr because it uses std::equal
    BOOST_STATIC_ASSERT_MSG(
        (is_equal_arrays(res, ethalon)),
        "Failed static assert with ethalon value"
    );
}
#endif // #ifndef BOOST_NO_CXX11_CONSTEXPR

#else   // no constexpr means no constexpr tests!
BOOST_AUTO_TEST_CASE( test_main )
{
}
#endif
