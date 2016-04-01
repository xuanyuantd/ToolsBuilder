
// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: reverse.cpp 49240 2008-10-10 09:21:07Z agurtovoy $
// $Date: 2008-10-10 17:21:07 +0800 (周五, 2008-10-10) $
// $Revision: 49240 $

#include <boost/mpl/reverse.hpp>

#include <boost/mpl/list_c.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/at.hpp>

#include <boost/mpl/aux_/test.hpp>

MPL_TEST_CASE()
{
    typedef list_c<int,9,8,7,6,5,4,3,2,1,0> numbers;
    typedef reverse< numbers >::type result;

    typedef range_c<int,0,10> answer;
    
    MPL_ASSERT(( equal< result,answer,equal_to<_1,_2> > ));
}
