
#ifndef BOOST_MPL_AUX_CONFIG_DEPENDENT_NTTP_HPP_INCLUDED
#define BOOST_MPL_AUX_CONFIG_DEPENDENT_NTTP_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /cvsroot/fit/fit/imp/cpp/lib/boost_1_32_0/boost/mpl/aux_/config/Attic/dependent_nttp.hpp,v $
// $Date: 2005/02/02 04:02:34 $
// $Revision: 1.1 $

#include <boost/mpl/aux_/config/gcc.hpp>
#include <boost/mpl/aux_/config/workaround.hpp>

// GCC and EDG-based compilers incorrectly reject the following code:
//   template< typename T, T n > struct a;
//   template< typename T > struct b;
//   template< typename T, T n > struct b< a<T,n> > {};

#if    !defined(BOOST_MPL_CFG_NO_DEPENDENT_NONTYPE_PARAMETER_IN_PARTIAL_SPEC) \
    && !defined(BOOST_MPL_PREPROCESSING_MODE) \
    && (   BOOST_WORKAROUND(__EDG_VERSION__, BOOST_TESTED_AT(300)) \
        || BOOST_WORKAROUND(BOOST_MPL_CFG_GCC, BOOST_TESTED_AT(0x0302)) \
        )

#   define BOOST_MPL_CFG_NO_DEPENDENT_NONTYPE_PARAMETER_IN_PARTIAL_SPEC

#endif

#endif // BOOST_MPL_AUX_CONFIG_DEPENDENT_NTTP_HPP_INCLUDED
