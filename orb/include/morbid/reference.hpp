/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_ORB_REFERENCE_HPP
#define MORBID_ORB_REFERENCE_HPP

#include <morbid/tags.hpp>
#include <morbid/structured_ior.hpp>
#include <morbid/detail/requirements.hpp>

#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/member.hpp>
#include <boost/type_erasure/is_empty.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/optional.hpp>
#include <boost/ref.hpp>

BOOST_TYPE_ERASURE_MEMBER((morbid)(poa)(has_call), call, 6)
BOOST_TYPE_ERASURE_MEMBER((morbid)(poa)(has_set_orb), set_orb, 1)
BOOST_TYPE_ERASURE_MEMBER((morbid)(poa)(has_type_id), type_id, 0)

namespace morbid {

struct reply;
struct orb;

} 

namespace boost { namespace type_erasure {

template <>
struct is_placeholder< ::morbid::reply> : ::boost::mpl::false_ {};
template <>
struct is_placeholder< ::morbid::orb> : ::boost::mpl::false_ {};

} }

namespace morbid { namespace poa {

template <typename C, typename T>
struct object_registration;

typedef boost::type_erasure::any
  <boost::mpl::vector
   <boost::type_erasure::copy_constructible<>
    , boost::type_erasure::relaxed
    , poa::has_call<void(std::string, std::size_t, const char*&
                         , const char*, bool
                         , reply&
                        )>
    , poa::has_set_orb<void(struct orb)>
    , poa::has_type_id<const char*()>
    > > object_registration_any;

}

namespace mpl = boost::mpl;

namespace detail {

template <typename T, typename U>
struct is_concept_base_and_derived
 :mpl::if_
  <
    mpl::contains<typename U::bases, T>
    , mpl::true_
    , mpl::not_
      <
        boost::is_same
        <
          typename mpl::find_if<typename U::bases, is_concept_base_and_derived<T, mpl::_> >::type
          , typename mpl::end<typename U::bases>::type
        >
      >
  >::type
{};

template <typename T>
struct is_concept_base_and_derived<T, T> : mpl::true_ {};

}

template <typename T>
struct is_remote_reference : boost::mpl::false_ {};

template <typename C>
struct reference : boost::type_erasure::any<typename detail::regular_requirements<C>::type>
{
  typedef reference<C> self_type;
  typedef interface_tag _morbid_type_kind;

  typedef C concept_class;
  typedef boost::type_erasure::any<typename detail::regular_requirements<C>::type> any_type;

  reference()
  {
    // std::cout << "empty reference constructed" << std::endl;
  }

  template <typename T>
  reference(reference<T> const& other
            , typename boost::enable_if<detail::is_concept_base_and_derived<C, T> >::type* = 0)
    : any_type(static_cast<typename reference<T>::any_type const&>(other))
    , _sior(other._sior)
    , _object_registration(other._object_registration)
  {
    // std::cout << "Copying reference from " << typeid(T).name() << " to " << typeid(C).name()
    //           << " and is empty: " << boost::type_erasure::is_empty(*this)
    //           << " and is remote: " << !!_sior << std::endl;
  }

  template <typename T>
  reference(reference<T>& other
            , typename boost::enable_if<detail::is_concept_base_and_derived<C, T> >::type* = 0)
    : any_type(static_cast<typename reference<T>::any_type&>(other))
    , _sior(other._sior)
    , _object_registration(other._object_registration)
  {
    // std::cout << "Copying reference from " << typeid(T).name() << " to " << typeid(C).name()
    //           << " and is empty: " << boost::type_erasure::is_empty(*this)
    //           << " and is remote: " << !!_sior << std::endl;
  }

  template <typename T>
  reference(T const& object)
    : any_type(object)
    , _object_registration(poa::object_registration<C, T>(object))
  {
    // std::cout << "Creating reference from " << typeid(T).name() << " and is remote? "
    //           << is_remote_reference<T>::type::value << std::endl;
    mpl::if_<is_remote_reference<T>, with_remote
             , without_remote>::type::call(*this, object);
  }

  template <typename T>
  reference(T& object)
    : any_type(object)
    , _object_registration(poa::object_registration<C, T>(object))
  {
    // std::cout << "Creating reference from " << typeid(T).name() << " and is remote? "
    //           << is_remote_reference<T>::type::value << std::endl;
    mpl::if_<is_remote_reference<T>, with_remote
             , without_remote>::type::call(*this, object);
  }

  template <typename T>
  reference(boost::reference_wrapper<T> object)
    : any_type(typename C::template proxy_reference<T>(object))
    , _object_registration(poa::object_registration<C, T>(object))
  {
    // std::cout << "Creating reference from reference_wrapped " << typeid(T).name() << " and is remote? "
    //           << is_remote_reference<T>::type::value << std::endl;
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

  bool operator!() const { return boost::type_erasure::is_empty(*this); }
  typedef bool(self_type::*unspecified_bool_type)() const;
  
  operator unspecified_bool_type() const
  {
    unspecified_bool_type nil = 0;
    return boost::type_erasure::is_empty(*this) ? nil : &self_type::operator!;
  }

  poa::object_registration_any& _object_registration_ref(struct orb orb);
  poa::object_registration_any _object_registration_copy(struct orb orb);

  boost::optional<structured_ior> _sior;
  poa::object_registration_any _object_registration;
};

template <typename C>
std::ostream& operator<<(std::ostream& os, reference<C> const& r)
{
  return os << "A reference";
}

}

#endif
