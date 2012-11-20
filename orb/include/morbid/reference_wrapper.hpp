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

  iiop::profile_body profile_body;

  ptr_type& get_ptr()
  {
    morbid::structured_ior ior;
    ior.type_id.insert(ior.type_id.end()
                       , original_type::_repository_id
                       , original_type::_repository_id
                       + std::strlen(original_type::_repository_id));
    ior.structured_profiles.push_back(profile_body);
    ptr = original_type::_construct_remote_stub(ior);
    return ptr;
  }

  mutable ptr_type ptr;
};

}

#endif
