
// Copyright Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /cvsroot/fit/fit/imp/cpp/lib/boost_1_32_0/libs/mpl/test/Attic/unpack_args.cpp,v $
// $Date: 2005/02/02 03:29:31 $
// $Revision: 1.1 $

#include <boost/mpl/unpack_args.hpp>

#include <boost/mpl/vector/vector10.hpp>
#include <boost/mpl/aux_/test.hpp>
#include <boost/type_traits/is_same.hpp>

MPL_TEST_CASE()
{
    MPL_ASSERT(( apply1< 
          unpack_args< is_same<_1,_2> >
        , vector2<int,int>
        > ));
}