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
#include <boost/ref.hpp>

namespace morbid {

template <typename T>
struct is_remote_reference : boost::mpl::false_ {};

template <typename C>
struct reference
  : boost::type_erasure::any<typename C::regular_requirements>
{
  typedef reference<C> self_type;
  typedef interface_tag _morbid_type_kind;

  typedef C concept_class;
  typedef boost::type_erasure::any<typename C::regular_requirements> any_type;

  reference()
    : any_type( typename C::empty_reference() )
    , _empty(true)
  {
    std::cout << "empty reference constructed" << std::endl;
  }

  template <typename T>
  reference(reference<T> const& other)
    : any_type(static_cast<typename reference<T>::any_type const&>(other))
    , _sior(other._sior)
    , _empty(other._empty)
  {
    std::cout << "Copying reference from " << typeid(T).name() << " to " << typeid(C).name()
              << " and is empty: " << _empty
              << " and is remote: " << !!_sior << std::endl;
  }

  template <typename T>
  reference(reference<T>& other)
    : any_type(static_cast<typename reference<T>::any_type&>(other))
    , _sior(other._sior)
    , _empty(other._empty)
  {
    std::cout << "Copying reference from " << typeid(T).name() << " to " << typeid(C).name()
              << " and is empty: " << _empty
              << " and is remote: " << !!_sior << std::endl;
  }

  template <typename T>
  reference(T const& object)
    : any_type(object)
    , _empty(false)
  {
    std::cout << "Creating reference from " << typeid(T).name() << " and is remote? "
              << is_remote_reference<T>::type::value << std::endl;
    mpl::if_<is_remote_reference<T>, with_remote
             , without_remote>::type::call(*this, object);
  }

  template <typename T>
  reference(T& object)
    : any_type(object)
    , _empty(false)
  {
    std::cout << "Creating reference from " << typeid(T).name() << " and is remote? "
              << is_remote_reference<T>::type::value << std::endl;
    mpl::if_<is_remote_reference<T>, with_remote
             , without_remote>::type::call(*this, object);
  }

  template <typename T>
  reference(boost::reference_wrapper<T> object)
    : any_type(typename C::template proxy_reference<T>(object))
    , _empty(false)
  {
    std::cout << "Creating reference from reference_wrapped " << typeid(T).name() << " and is remote? "
              << is_remote_reference<T>::type::value << std::endl;
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

  bool operator!() const { return _empty; }
  typedef bool(self_type::*unspecified_bool_type)();
  
  operator unspecified_bool_type() const
  {
    unspecified_bool_type nil = 0;
    return _empty ? nil : &self_type::operator!;
  }

  boost::optional<structured_ior> _sior;
  bool _empty;
};

template <typename C>
std::ostream& operator<<(std::ostream& os, reference<C> const& r)
{
  return os << "A reference";
}

}

#endif
