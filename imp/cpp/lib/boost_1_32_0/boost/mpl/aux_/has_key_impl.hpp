
#ifndef BOOST_MPL_AUX_HAS_KEY_IMPL_HPP_INCLUDED
#define BOOST_MPL_AUX_HAS_KEY_IMPL_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2002-2004
// Copyright David Abrahams 2003
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /cvsroot/fit/fit/imp/cpp/lib/boost_1_32_0/boost/mpl/aux_/Attic/has_key_impl.hpp,v $
// $Date: 2005/02/02 04:02:32 $
// $Revision: 1.1 $

#include <boost/mpl/has_key_fwd.hpp>
#include <boost/mpl/aux_/traits_lambda_spec.hpp>

namespace boost { namespace mpl {

// no default implementation; the definition is needed to make MSVC happy 

template< typename Tag > struct has_key_impl
{
    template< typename AssociativeSequence, typename Key > struct apply;
};

BOOST_MPL_ALGORITM_TRAITS_LAMBDA_SPEC(2,has_key_impl)

}}

#endif // BOOST_MPL_AUX_HAS_KEY_IMPL_HPP_INCLUDED
