
// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /cvsroot/fit/fit/imp/cpp/lib/boost_1_32_0/libs/mpl/test/aux_/Attic/msvc_is_class.cpp,v $
// $Date: 2005/02/02 03:29:32 $
// $Revision: 1.1 $

#include <boost/mpl/aux_/config/msvc.hpp>
#include <boost/mpl/aux_/config/workaround.hpp>

#if BOOST_WORKAROUND(BOOST_MSVC, <= 1200)

#include <boost/mpl/aux_/msvc_is_class.hpp>
#include <boost/mpl/aux_/test.hpp>

template< typename T > struct A { T x[0]; };

MPL_TEST_CASE()
{
    MPL_ASSERT_NOT(( aux::msvc_is_class< int > ));
    MPL_ASSERT_NOT(( aux::msvc_is_class< char > ));
    MPL_ASSERT_NOT(( aux::msvc_is_class< enum_ > ));
    MPL_ASSERT_NOT(( aux::msvc_is_class< char* > ));
    MPL_ASSERT_NOT(( aux::msvc_is_class< UDT* > ));
    MPL_ASSERT_NOT(( aux::msvc_is_class< UDT& > ));
    MPL_ASSERT_NOT(( aux::msvc_is_class< incomplete* > ));
    MPL_ASSERT_NOT(( aux::msvc_is_class< incomplete& > ));
    MPL_ASSERT_NOT(( aux::msvc_is_class< int[5] > ));
    MPL_ASSERT_NOT(( aux::msvc_is_class< void (*)() > ));
    MPL_ASSERT_NOT(( aux::msvc_is_class< int (*)(int, char) > ));
    
    MPL_ASSERT(( aux::msvc_is_class< UDT > ));
    MPL_ASSERT(( aux::msvc_is_class< incomplete > ));
    MPL_ASSERT(( aux::msvc_is_class< abstract > ));
    MPL_ASSERT(( aux::msvc_is_class< noncopyable > ));
    MPL_ASSERT(( aux::msvc_is_class< A<int>  > ));
    MPL_ASSERT(( aux::msvc_is_class< A<incomplete> > ));
}

#endif
