
// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License,Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: if.cpp 49240 2008-10-10 09:21:07Z agurtovoy $
// $Date: 2008-10-10 17:21:07 +0800 (周五, 2008-10-10) $
// $Revision: 49240 $

#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/aux_/test.hpp>

MPL_TEST_CASE()
{
    typedef if_<true_,char,long>::type t1;
    typedef if_c<true,char,long>::type t2;
    typedef if_<false_,char,long>::type t3;
    typedef if_c<false,char,long>::type t4;

    MPL_ASSERT(( is_same<t1, char> ));
    MPL_ASSERT(( is_same<t2, char> ));
    MPL_ASSERT(( is_same<t3, long> ));
    MPL_ASSERT(( is_same<t4, long> ));
}
