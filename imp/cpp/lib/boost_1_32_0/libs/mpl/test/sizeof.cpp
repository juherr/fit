
// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License,Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /cvsroot/fit/fit/imp/cpp/lib/boost_1_32_0/libs/mpl/test/Attic/sizeof.cpp,v $
// $Date: 2005/02/02 03:29:29 $
// $Revision: 1.1 $

#include <boost/mpl/sizeof.hpp>
#include <boost/mpl/aux_/test.hpp>

struct my
{
    char a[100];
};

MPL_TEST_CASE()
{
    MPL_ASSERT_RELATION( sizeof_<char>::value, ==, sizeof(char) );
    MPL_ASSERT_RELATION( sizeof_<int>::value, ==, sizeof(int) );
    MPL_ASSERT_RELATION( sizeof_<double>::value, ==, sizeof(double) );
    MPL_ASSERT_RELATION( sizeof_<my>::value, ==, sizeof(my) );
}
