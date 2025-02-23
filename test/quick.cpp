// Copyright 2023 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/array.hpp>

int main()
{
    boost::array<int, 2> a = {{ 0, 1 }};
    return a[0];
}
