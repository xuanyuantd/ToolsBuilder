
#ifndef BOOST_MPL_LIST_AUX_SIZE_HPP_INCLUDED
#define BOOST_MPL_LIST_AUX_SIZE_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: size.hpp 49239 2008-10-10 09:10:26Z agurtovoy $
// $Date: 2008-10-10 17:10:26 +0800 (周五, 2008-10-10) $
// $Revision: 49239 $

#include <boost/mpl/size_fwd.hpp>
#include <boost/mpl/list/aux_/tag.hpp>

namespace boost { namespace mpl {

template<>
struct size_impl< aux::list_tag >
{
    template< typename List > struct apply
        : List::size
    {
    };
};

}}

#endif // BOOST_MPL_LIST_AUX_SIZE_HPP_INCLUDED
