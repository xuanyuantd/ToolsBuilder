
#ifndef BOOST_FSM_TRANSITION_INCLUDED
#define BOOST_FSM_TRANSITION_INCLUDED

// Copyright Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: transition.hpp 49240 2008-10-10 09:21:07Z agurtovoy $
// $Date: 2008-10-10 17:21:07 +0800 (周五, 2008-10-10) $
// $Revision: 49240 $

#include <cassert>

namespace fsm { namespace aux {

// represent a signle transition between states |From| and |To|

template<
      typename T
    , typename From
    , typename Event
    , typename To
    , bool (T::* transition_func)(Event const&)
    >
struct transition
{
    typedef T       fsm_t;
    typedef From    from_state_t;
    typedef Event   event_t;
    typedef To      to_state_t;

    typedef typename Event::base_t  base_event_t;
    static bool do_transition(T& x, base_event_t const& e)
    {
        assert(dynamic_cast<event_t const*>(&e) == &e);
        return (x.*transition_func)(static_cast<event_t const &>(e));
    }
};

}}

#endif // BOOST_FSM_TRANSITION_INCLUDED
