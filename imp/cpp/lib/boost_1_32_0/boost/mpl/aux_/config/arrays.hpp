
#ifndef BOOST_MPL_AUX_CONFIG_ARRAYS_HPP_INCLUDED
#define BOOST_MPL_AUX_CONFIG_ARRAYS_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /cvsroot/fit/fit/imp/cpp/lib/boost_1_32_0/boost/mpl/aux_/config/Attic/arrays.hpp,v $
// $Date: 2005/02/02 04:02:34 $
// $Revision: 1.1 $

#include <boost/mpl/aux_/config/msvc.hpp>
#include <boost/mpl/aux_/config/workaround.hpp>

#if    !defined(BOOST_MPL_CFG_NO_DEPENDENT_ARRAY_TYPES) \
    && !defined(BOOST_MPL_PREPROCESSING_MODE) \
    && ( BOOST_WORKAROUND(__BORLANDC__, < 0x600) \
        || BOOST_WORKAROUND(BOOST_MSVC, <= 1300) \
        )

#   define BOOST_MPL_CFG_NO_DEPENDENT_ARRAY_TYPES

#endif

#endif // BOOST_MPL_AUX_CONFIG_ARRAYS_HPP_INCLUDED
