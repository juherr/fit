
// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /cvsroot/fit/fit/imp/cpp/lib/boost_1_32_0/libs/mpl/test/aux_/Attic/template_arity.cpp,v $
// $Date: 2005/02/02 03:29:32 $
// $Revision: 1.1 $

#include <boost/mpl/aux_/template_arity.hpp>
#include <boost/mpl/aux_/test.hpp>

#if defined(BOOST_EXTENDED_TEMPLATE_PARAMETERS_MATCHING)

struct my {};
template< typename T1 > struct my1 {};
template< typename T1, typename T2 = void > struct my2 {};

MPL_TEST_CASE()
{
    MPL_ASSERT_RELATION( (aux::template_arity<my>::value), ==, -1 );
    MPL_ASSERT_RELATION( (aux::template_arity< my1<int> >::value), ==, 1 );
    MPL_ASSERT_RELATION( (aux::template_arity< my2<int,long> >::value), ==, 2 );
    MPL_ASSERT_RELATION( (aux::template_arity< my2<int> >::value), ==, 2 );
}

#endif