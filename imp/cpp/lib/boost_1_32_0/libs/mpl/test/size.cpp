
// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /cvsroot/fit/fit/imp/cpp/lib/boost_1_32_0/libs/mpl/test/Attic/size.cpp,v $
// $Date: 2005/02/02 03:29:29 $
// $Revision: 1.1 $

#include <boost/mpl/size.hpp>

#include <boost/mpl/list.hpp>
#include <boost/mpl/aux_/test.hpp>

MPL_TEST_CASE()
{
    typedef list<> empty_list;
    typedef list<char,short,int,long> list;
    
    MPL_ASSERT_RELATION( size<empty_list>::value, ==, 0 );
    MPL_ASSERT_RELATION( size<list>::value, ==, 4 );
}
