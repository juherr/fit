
#ifndef BOOST_MPL_MAP_AUX_CONTAINS_IMPL_HPP_INCLUDED
#define BOOST_MPL_MAP_AUX_CONTAINS_IMPL_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /cvsroot/fit/fit/imp/cpp/lib/boost_1_32_0/boost/mpl/map/aux_/Attic/contains_impl.hpp,v $
// $Date: 2005/02/02 03:02:09 $
// $Revision: 1.1 $

#include <boost/mpl/contains_fwd.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/map/aux_/at_impl.hpp>
#include <boost/mpl/map/aux_/tag.hpp>

#include <boost/type_traits/is_same.hpp>

namespace boost { namespace mpl {

template<>
struct contains_impl< aux::map_tag >
{
    template< typename Map, typename Pair > struct apply
        : is_same< 
              typename at_impl<aux::map_tag>::apply<
                  Map
                , typename Pair::first
                >::type
            , typename Pair::second
            >
    {
    };
};

}}

#endif // BOOST_MPL_MAP_AUX_CONTAINS_IMPL_HPP_INCLUDED
