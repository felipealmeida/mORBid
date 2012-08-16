/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_OBJECT_HPP
#define TECORB_OBJECT_HPP

#include <tecorb/narrow.hpp>
#include <tecorb/var.hpp>

namespace tecorb {

struct Object
{
  virtual ~Object() {} 
};

typedef boost::shared_ptr<Object> Object_ptr;
typedef tecorb::var<Object> Object_var;

struct LocalObject : narrow<LocalObject, boost::mpl::vector1<Object> >
{
  
};

typedef boost::shared_ptr<LocalObject> LocalObject_ptr;
typedef tecorb::var<LocalObject> LocalObject_var;

inline bool is_nil(Object_ptr obj)
{
  return !obj;
}

}

#endif
