
#ifndef BOOST_MPL_MAP_AUX_CLEAR_IMPL_HPP_INCLUDED
#define BOOST_MPL_MAP_AUX_CLEAR_IMPL_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2003-2004
// Copyright David Abrahams 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /cvsroot/fit/fit/imp/cpp/lib/boost_1_32_0/boost/mpl/map/aux_/Attic/clear_impl.hpp,v $
// $Date: 2005/02/02 03:02:09 $
// $Revision: 1.1 $

#include <boost/mpl/clear_fwd.hpp>
#include <boost/mpl/map/aux_/map0.hpp>
#include <boost/mpl/map/aux_/tag.hpp>

namespace boost { namespace mpl {

template<>
struct clear_impl< aux::map_tag >
{
    template< typename Map > struct apply
    {
        typedef map0<> type;
    };
};

}}

#endif // BOOST_MPL_MAP_AUX_CLEAR_IMPL_HPP_INCLUDED
