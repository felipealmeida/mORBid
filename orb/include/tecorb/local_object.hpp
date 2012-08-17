/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_LOCAL_OBJECT_HPP
#define TECORB_LOCAL_OBJECT_HPP

#include <tecorb/narrow.hpp>
#include <tecorb/object.hpp>

namespace tecorb {

struct LocalObject : narrow<LocalObject, boost::mpl::vector1<Object> >
{
  String_ptr ior() const
  {
    throw MARSHALL();
  }
};

typedef boost::shared_ptr<LocalObject> LocalObject_ptr;
typedef tecorb::var<LocalObject> LocalObject_var;

}

#endif
