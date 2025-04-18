/* tests for using class array<> specialization for size 0
 * (C) Copyright Alisdair Meredith 2006.
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#define BOOST_ALLOW_DEPRECATED_SYMBOLS // get_c_array

#include <boost/array.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/config.hpp>
#include <string>
#include <iostream>
#include <algorithm>

#if defined(BOOST_GCC) && BOOST_GCC / 10000 == 13
// false -Warray-bounds positive when using -fsanitize=undefined
// restricted to GCC 13 because that's what is tested on Drone
# pragma GCC diagnostic ignored "-Warray-bounds"
#endif

namespace {
    template< class T >
    void    RunTests()
    {
        typedef boost::array< T, 5 >    test_type;
        typedef T arr[5];
        test_type           test_case; //   =   { 1, 1, 2, 3, 5 };
    
        arr &aRef = get_c_array ( test_case );
        BOOST_TEST ( &*test_case.begin () == &aRef[0] );
        
        const arr &caRef = get_c_array ( test_case );
        typename test_type::const_iterator iter = test_case.begin ();
        BOOST_TEST ( &*iter == &caRef[0] );

    //  Confirm at() throws the std lib defined exception
        try {
            test_case.at( test_case.size());
            BOOST_TEST(false);
            }
        catch ( const std::out_of_range & ) {}

        try {
            test_case.at( test_case.size() + 1);
            BOOST_TEST(false);
            }
        catch ( const std::out_of_range & ) {}

        try {
            test_case.at( test_case.size() + 100);
            BOOST_TEST(false);
            }
        catch ( const std::out_of_range & ) {}
    }
}

int main ()
{
    RunTests< bool >();
    RunTests< void * >();
    RunTests< long double >();
    RunTests< std::string >();

    return boost::report_errors();
}

