/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_ORB_OBJECT_HPP
#define MORBID_ORB_OBJECT_HPP

#include <morbid/structured_ior.hpp>
#include <morbid/reference.hpp>
#include <morbid/orb.hpp>

namespace morbid {

struct object_ref
{
  object_ref( ::morbid::orb orb, ::morbid::structured_ior const& structured_ior)
     : _orb_(orb), _structured_ior_(structured_ior)
  {}
  object_ref( ::morbid::orb orb, std::string const& ior)
     : _orb_(orb), _structured_ior_( ::morbid::string_to_structured_ior(ior.begin(), ior.end()))
  {}
  
  ::morbid::structured_ior _structured_ior() const { return _structured_ior_; }
private:
  ::morbid::orb _orb_;
  ::morbid::structured_ior _structured_ior_;
};

template <>
struct is_remote_reference< object_ref > : ::boost::mpl::true_ {};

struct object_concept
{
  inline static const char* type_id() { return "IDL:omg.org/Object:1.0"; }

  typedef boost::mpl::vector< > requirements;
  typedef boost::mpl::vector< ::boost::type_erasure::copy_constructible<>, ::boost::type_erasure::relaxed >
    regular_requirements;
  typedef ::morbid::interface_tag _morbid_type_kind;

  struct empty_reference
  {
  };

  template <typename T>
  struct proxy_reference
  {
    proxy_reference(boost::reference_wrapper<T> ref) : _ptr(&boost::unwrap_ref(ref)) {}
    T* _ptr;
  };

  typedef object_ref remote_reference;
};

typedef reference<object_concept> object;

}

#endif
