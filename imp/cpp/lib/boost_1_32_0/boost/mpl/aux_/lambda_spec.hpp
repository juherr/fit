
#ifndef BOOST_MPL_AUX_LAMBDA_SPEC_HPP_INCLUDED
#define BOOST_MPL_AUX_LAMBDA_SPEC_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /cvsroot/fit/fit/imp/cpp/lib/boost_1_32_0/boost/mpl/aux_/Attic/lambda_spec.hpp,v $
// $Date: 2005/02/02 04:02:33 $
// $Revision: 1.1 $

#include <boost/mpl/void.hpp>
#include <boost/mpl/lambda_fwd.hpp>
#include <boost/mpl/int_fwd.hpp>
#include <boost/mpl/aux_/preprocessor/params.hpp>
#include <boost/mpl/aux_/lambda_arity_param.hpp>
#include <boost/mpl/aux_/config/lambda.hpp>

#if !defined(BOOST_MPL_CFG_NO_FULL_LAMBDA_SUPPORT)

#   define BOOST_MPL_AUX_PASS_THROUGH_LAMBDA_SPEC(i, name) \
template< \
      BOOST_MPL_PP_PARAMS(i, typename T) \
    , typename Tag \
    > \
struct lambda< \
      name< BOOST_MPL_PP_PARAMS(i, T) > \
    , Tag \
    BOOST_MPL_AUX_LAMBDA_ARITY_PARAM(int_<i>) \
    > \
{ \
    typedef false_ is_le; \
    typedef name< BOOST_MPL_PP_PARAMS(i, T) > type; \
}; \
/**/

#else

#   define BOOST_MPL_AUX_PASS_THROUGH_LAMBDA_SPEC(i, name) /**/

#endif

#endif // BOOST_MPL_AUX_LAMBDA_SPEC_HPP_INCLUDED
