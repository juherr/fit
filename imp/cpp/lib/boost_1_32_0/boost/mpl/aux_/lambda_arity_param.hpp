
#ifndef BOOST_MPL_AUX_LAMBDA_ARITY_PARAM_HPP_INCLUDED
#define BOOST_MPL_AUX_LAMBDA_ARITY_PARAM_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /cvsroot/fit/fit/imp/cpp/lib/boost_1_32_0/boost/mpl/aux_/Attic/lambda_arity_param.hpp,v $
// $Date: 2005/02/02 04:02:32 $
// $Revision: 1.1 $

#include <boost/mpl/aux_/config/ttp.hpp>

#if !defined(BOOST_MPL_CFG_EXTENDED_TEMPLATE_PARAMETERS_MATCHING)
#   define BOOST_MPL_AUX_LAMBDA_ARITY_PARAM(param)    
#else
#   define BOOST_MPL_AUX_LAMBDA_ARITY_PARAM(param) , param
#endif

#endif // BOOST_MPL_AUX_LAMBDA_ARITY_PARAM_HPP_INCLUDED
