/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_ORB_REFERENCE_CAST_HPP
#define MORBID_ORB_REFERENCE_CAST_HPP

#include <morbid/reference.hpp>

namespace morbid {

namespace detail {

template <typename R, typename T>
R remote_ref_cast(T const& ref
                  , boost::mpl::false_ /* false is downcast, must be checked */)
{
  typedef typename R::concept_class to_concept;
  typedef typename T::concept_class from_concept;
  BOOST_MPL_ASSERT((is_concept_base_and_derived<from_concept, to_concept>));

  if(ref._orb() && !ref._structured_ior().structured_profiles.empty())
  {
    bool is =
      ::morbid::synchronous_call::call<bool>
      ( *ref._orb(), to_concept::type_id(), "_is_a", ref._structured_ior()
        , boost::fusion::vector1
        < type_tag::value_type_tag<string, type_tag::in_tag>
        >(type_tag::value_type_tag<string, type_tag::in_tag>(to_concept::type_id()))
      );
    if(is)
    {
      std::cout << "OK" << std::endl;
      return R(typename to_concept::remote_reference(*ref._orb(), ref._structured_ior()));
    }
  }
  std::cout << "Not OK" << std::endl;
  return R();
}

template <typename R, typename T>
R remote_ref_cast(T const& ref, boost::mpl::true_ /* same or upcast */)
{
  return ref;
}

template <typename R>
struct check_if_is_reference
{
  template <typename>
  struct false_ : mpl::false_ {};

  static void test()
  {
    BOOST_MPL_ASSERT((false_<R>));
  };
};

template <typename C>
struct check_if_is_reference<reference<C> >
{
  static void test()
  {
  };
};

template <typename R, typename C>
R ref_cast(reference<C> const& ref, struct orb orb, boost::mpl::false_ /* false is downcast, must be checked */)
{
  typedef typename R::concept_class to_concept;
  typedef typename reference<C>::concept_class from_concept;
  BOOST_MPL_ASSERT((is_concept_base_and_derived<from_concept, to_concept>));

  if(ref._sior && !ref._sior->structured_profiles.empty())
  {
    bool is =
      ::morbid::synchronous_call::call<bool>
      ( orb, to_concept::type_id(), "_is_a", *ref._sior
        , boost::fusion::vector1
        < type_tag::value_type_tag<string, type_tag::in_tag>
        >(type_tag::value_type_tag<string, type_tag::in_tag>(to_concept::type_id()))
      );
    if(is)
    {
      std::cout << "OK" << std::endl;
      return R(typename to_concept::remote_reference(orb, *ref._sior));
    }
  }
  std::cout << "Not OK" << std::endl;
  return R();
}

template <typename R, typename C>
R ref_cast(reference<C> const& ref, struct orb, boost::mpl::true_ /* same or upcast */)
{
  return ref;
}

}

template <typename R, typename C>
R reference_cast(reference<C> const& ref
                 , struct orb orb)
{
  detail::check_if_is_reference<R>::test();
  typedef typename R::concept_class to_concept;
  typedef typename reference<C>::concept_class from_concept;
  return detail::ref_cast<R>
    (ref, orb, typename detail::is_concept_base_and_derived<to_concept, from_concept>::type());
}

template <typename R, typename T>
R reference_cast(T const& ref
                 , struct orb orb
                 , typename boost::enable_if<is_remote_reference<T> >::type* = 0)
{
  detail::check_if_is_reference<R>::test();
  typedef typename R::concept_class to_concept;
  typedef typename T::concept_class from_concept;
  return detail::remote_ref_cast<R>
    (ref, typename detail::is_concept_base_and_derived<to_concept, from_concept>::type());
}

template <typename R, typename T>
R reference_cast(T const& ref
                 , struct orb orb
                 , typename boost::disable_if<is_remote_reference<T> >::type* = 0);

}

#endif
