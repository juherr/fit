
#ifndef BOOST_FSM_STATE_INCLUDED
#define BOOST_FSM_STATE_INCLUDED

// Copyright Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /cvsroot/fit/fit/imp/cpp/lib/boost_1_32_0/libs/mpl/example/fsm/aux_/Attic/state.hpp,v $
// $Date: 2005/02/02 04:06:22 $
// $Revision: 1.1 $

#include <boost/mpl/integral_c.hpp>

namespace fsm { namespace aux {

namespace mpl = boost::mpl;

// represent a FSM state

template<
      typename T
    , long State
    , void (T::* invariant_func)() const
    >
struct state
    : mpl::integral_c<long,State>
{
    static long do_check_invariant(T const& x)
    {
        if (invariant_func) (x.*invariant_func)();
        return State;
    }
};

}}

#endif // BOOST_FSM_STATE_INCLUDED
