
// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: iterator_tags.cpp 49240 2008-10-10 09:21:07Z agurtovoy $
// $Date: 2008-10-10 17:21:07 +0800 (周五, 2008-10-10) $
// $Revision: 49240 $

#include <boost/mpl/iterator_tags.hpp>

#include <boost/mpl/less.hpp>
#include <boost/mpl/aux_/test.hpp>

MPL_TEST_CASE()
{
    MPL_ASSERT(( less<mpl::forward_iterator_tag,mpl::bidirectional_iterator_tag> ));
    MPL_ASSERT(( less<mpl::bidirectional_iterator_tag,mpl::random_access_iterator_tag> ));
}
