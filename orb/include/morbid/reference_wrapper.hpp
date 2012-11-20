/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_REFERENCE_WRAPPER_HPP
#define MORBID_REFERENCE_WRAPPER_HPP

#include <morbid/iiop/profile_body.hpp>
#include <morbid/structured_ior.hpp>

#include <cstring>

namespace morbid {

template <typename T>
struct reference_wrapper
{
  typedef T original_type;
  typedef typename T::_ptr_type ptr_type;

  morbid::structured_ior ior;

  ptr_type& get_ptr()
  {
    if(!ptr)
      ptr = original_type::_construct_remote_stub(ior);
    return ptr;
  }

  mutable ptr_type ptr;
};

}

#endif
