
// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /cvsroot/fit/fit/imp/cpp/lib/boost_1_32_0/libs/mpl/test/Attic/empty.cpp,v $
// $Date: 2005/02/02 03:29:13 $
// $Revision: 1.1 $

#include <boost/mpl/empty.hpp>

#include <boost/mpl/list/list10.hpp>
#include <boost/mpl/aux_/test.hpp>

MPL_TEST_CASE()
{
    MPL_ASSERT(( empty< list0<> > ));
    MPL_ASSERT_NOT(( empty< list1<char> > ));
}
