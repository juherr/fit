
#ifndef BOOST_MPL_INDEX_OF_HPP_INCLUDED
#define BOOST_MPL_INDEX_OF_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2003-2004
// Copyright Eric Friedman 2003
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /cvsroot/fit/fit/imp/cpp/lib/boost_1_32_0/boost/mpl/Attic/index_of.hpp,v $
// $Date: 2005/02/02 02:59:08 $
// $Revision: 1.1 $

#include <boost/mpl/index_if.hpp>
#include <boost/mpl/same_as.hpp>
#include <boost/mpl/aux_/na_spec.hpp>
#include <boost/mpl/aux_/lambda_support.hpp>

namespace boost { namespace mpl {

template<
      typename BOOST_MPL_AUX_NA_PARAM(Sequence)
    , typename BOOST_MPL_AUX_NA_PARAM(T)
    >
struct index_of
    : index_if< Sequence,same_as<T> >
{
    BOOST_MPL_AUX_LAMBDA_SUPPORT(2,index_of,(Sequence,T))
};

BOOST_MPL_AUX_NA_SPEC(2, index_of)

}}

#endif // BOOST_MPL_INDEX_OF_HPP_INCLUDED