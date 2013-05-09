/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_ORB_REFERENCE_HPP
#define MORBID_ORB_REFERENCE_HPP

#include <morbid/tags.hpp>
#include <morbid/structured_ior.hpp>

#include <boost/type_erasure/any.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/optional.hpp>

namespace morbid {

template <typename T>
struct is_remote_reference : boost::mpl::false_ {};

template <typename C>
struct reference
  : boost::type_erasure::any<typename C::regular_requirements>
{
  typedef interface_tag _morbid_type_kind;

  typedef C concept_class;
  typedef boost::type_erasure::any<typename C::regular_requirements> any_type;

  reference()
    : any_type( typename C::empty_reference() )
  {}

  template <typename T>
  reference(reference<T> const& other)
    : any_type(*(typename reference<T>::any_type*)&other)
    , _sior(other._sior)
  {}
  template <typename T>
  reference(reference<T>& other)
    : any_type(*(typename reference<T>::any_type*)&other)
    , _sior(other._sior)
  {}

  template <typename T>
  reference(T const& object)
    : any_type(object)
  {
    mpl::if_<is_remote_reference<T>, with_remote
             , without_remote>::type::call(*this, object);
  }

  template <typename T>
  reference(T& object)
    : any_type(object)
  {
    mpl::if_<is_remote_reference<T>, with_remote
             , without_remote>::type::call(*this, object);
  }

  struct with_remote
  {
    template <typename T>
    static void call(reference& self, T& object)
    {
      self._sior = object._structured_ior();
    }
  };
  struct without_remote
  {
    template <typename T>
    static void call(reference& self, T& object) {}
  };

  bool operator!() const { return true; }

  boost::optional<structured_ior> _sior;
};

template <typename C>
std::ostream& operator<<(std::ostream& os, reference<C> const& r)
{
  std::cout << "reference " << std::endl;
}

}

#endif
