
#ifndef BOOST_MPL_SET_AUX_VALUE_TYPE_IMPL_HPP_INCLUDED
#define BOOST_MPL_SET_AUX_VALUE_TYPE_IMPL_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2003-2004
// Copyright David Abrahams 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /cvsroot/fit/fit/imp/cpp/lib/boost_1_32_0/boost/mpl/set/aux_/Attic/value_type_impl.hpp,v $
// $Date: 2005/02/02 03:02:32 $
// $Revision: 1.1 $

#include <boost/mpl/value_type_fwd.hpp>
#include <boost/mpl/set/aux_/tag.hpp>

namespace boost { namespace mpl {

template<>
struct value_type_impl< aux::set_tag >
{
    template< typename Set, typename T > struct apply
    {
        typedef T type;
    };
};

}}

#endif // BOOST_MPL_SET_AUX_VALUE_TYPE_IMPL_HPP_INCLUDED