
// Copyright Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: unpack_args.cpp 49240 2008-10-10 09:21:07Z agurtovoy $
// $Date: 2008-10-10 17:21:07 +0800 (周五, 2008-10-10) $
// $Revision: 49240 $

#include <boost/mpl/unpack_args.hpp>

#include <boost/mpl/vector/vector10.hpp>
#include <boost/mpl/aux_/test.hpp>
#include <boost/type_traits/is_same.hpp>

MPL_TEST_CASE()
{
    MPL_ASSERT(( apply1< 
          unpack_args< is_same<_1,_2> >
        , vector2<int,int>
        > ));
}
