
// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /cvsroot/fit/fit/imp/cpp/lib/boost_1_32_0/libs/mpl/test/Attic/contains.cpp,v $
// $Date: 2005/02/02 03:29:12 $
// $Revision: 1.1 $

#include <boost/mpl/contains.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/aux_/test.hpp>

MPL_TEST_CASE()
{    
    typedef vector<int,char,long,short,char,long,double,long>::type types;

    MPL_ASSERT(( contains< types,short > ));
    MPL_ASSERT_NOT(( contains< types,unsigned > ));
}
