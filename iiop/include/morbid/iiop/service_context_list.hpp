/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IIOP_SERVICE_CONTEXT_LIST_HPP
#define TECORB_IIOP_SERVICE_CONTEXT_LIST_HPP

#include <boost/fusion/include/adapt_struct.hpp>

namespace morbid { namespace iiop {

struct service_context_list
{
  int i;
};

} }

BOOST_FUSION_ADAPT_STRUCT(morbid::iiop::service_context_list
                          , (int, i))

#endif

