
#ifndef BOOST_MPL_SET_AUX_ITEM_HPP_INCLUDED
#define BOOST_MPL_SET_AUX_ITEM_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2003-2004
// Copyright David Abrahams 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /cvsroot/fit/fit/imp/cpp/lib/boost_1_32_0/boost/mpl/set/aux_/Attic/item.hpp,v $
// $Date: 2005/02/02 03:02:32 $
// $Revision: 1.1 $

#include <boost/mpl/long.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/prior.hpp>
#include <boost/mpl/set/aux_/set0.hpp>
#include <boost/mpl/aux_/type_wrapper.hpp>
#include <boost/mpl/aux_/config/arrays.hpp>

namespace boost { namespace mpl {

template< typename T, typename Base >
struct s_item
    : Base
{
    typedef void_       last_masked_;
    typedef Base        next_;
    typedef T           item_type_;
    typedef item_type_  type;
    typedef Base        base;
    
    typedef typename next< typename Base::size >::type  size;
    typedef typename next< typename Base::order >::type order;

#if defined(BOOST_MPL_CFG_NO_DEPENDENT_ARRAY_TYPES)
    typedef typename aux::weighted_tag<BOOST_MPL_AUX_MSVC_VALUE_WKND(order)::value>::type order_tag_;
#else
    typedef char (&order_tag_)[BOOST_MPL_AUX_MSVC_VALUE_WKND(order)::value];
#endif

    BOOST_MPL_AUX_SET_OVERLOAD( order_tag_, ORDER_BY_KEY, s_item, aux::type_wrapper<T>* );
    BOOST_MPL_AUX_SET_OVERLOAD( aux::no_tag, IS_MASKED, s_item, aux::type_wrapper<T>* );
};


template< typename T, typename Base >
struct s_mask
    : Base
{
    typedef T       last_masked_;
    typedef void_   item_type_;
    typedef Base    base;
    typedef typename prior< typename Base::size >::type  size;

    BOOST_MPL_AUX_SET_OVERLOAD( aux::yes_tag, IS_MASKED, s_mask, aux::type_wrapper<T>* );
};


template< typename T, typename Base >
struct s_unmask
    : Base
{
    typedef void_   last_masked_;
    typedef T       item_type_;
    typedef Base    base;
    typedef typename next< typename Base::size >::type  size;

    BOOST_MPL_AUX_SET_OVERLOAD( aux::no_tag, IS_MASKED, s_unmask, aux::type_wrapper<T>* );
};

}}

#endif // BOOST_MPL_SET_AUX_ITEM_HPP_INCLUDED