
#ifndef BOOST_MPL_ITER_PUSH_FRONT_HPP_INCLUDED
#define BOOST_MPL_ITER_PUSH_FRONT_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /cvsroot/fit/fit/imp/cpp/lib/boost_1_32_0/boost/mpl/aux_/Attic/iter_push_front.hpp,v $
// $Date: 2005/02/02 04:02:32 $
// $Revision: 1.1 $

#include <boost/mpl/push_front.hpp>
#include <boost/mpl/deref.hpp>

namespace boost { namespace mpl { namespace aux {

template<
      typename Sequence
    , typename Iterator
    >
struct iter_push_front
{
    typedef typename push_front<
          Sequence
        , typename deref<Iterator>::type
        >::type type;
};

}}}

#endif // BOOST_MPL_ITER_PUSH_FRONT_HPP_INCLUDED
