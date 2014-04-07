/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_META_COMPILER_HPP
#define MORBID_IIOP_META_COMPILER_HPP

#include <morbid/iiop/domain.hpp>

#include <morbid/giop/common_terminals.hpp>
#include <morbid/giop/compile.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/phoenix.hpp>
#include <boost/phoenix/fusion/at.hpp>

namespace morbid { namespace iiop {

namespace generator {

template <typename Tag, typename Modifiers, typename Enable = void>
struct make_primitive;

template <typename Tag, typename Elements
          , typename Modifiers, typename Enable = void>
struct make_composite;

template <typename Directive, typename Body
          , typename Modifiers, typename Enable = void>
struct make_directive;

}

namespace parser {

template <typename Tag, typename Modifiers, typename Enable = void>
struct make_primitive;

template <typename Tag, typename Elements
          , typename Modifiers, typename Enable = void>
struct make_composite;

template <typename Directive, typename Body
          , typename Modifiers, typename Enable = void>
struct make_directive;

}

} }

namespace boost { namespace spirit {

template <>
struct make_component< ::morbid::iiop::generator_domain, proto::tag::terminal>
{
  template <typename Sig>
  struct result;

  template <typename This, typename Elements, typename Modifiers>
  struct result<This(Elements, Modifiers)>
  {
    typedef typename ::morbid::iiop::generator::make_primitive<
      typename remove_const<typename Elements::car_type>::type
      , typename remove_reference<Modifiers>::type
      >::result_type type;
  };

  template <typename Elements, typename Modifiers>
  typename result<make_component(Elements, Modifiers)>::type
  operator()(Elements const& elements, Modifiers const& modifiers) const
  {
    // int i = 5;
    typedef typename remove_const<typename Elements::car_type>::type term;
    return ::morbid::iiop::generator::make_primitive<term, Modifiers>()(elements.car, modifiers);
  }
};

template <typename Tag>
struct make_component< ::morbid::iiop::generator_domain, Tag>
{
  template <typename Sig>
  struct result;

  template <typename This, typename Elements, typename Modifiers>
  struct result<This(Elements, Modifiers)>
  {
    typedef typename
    ::morbid::iiop::generator::make_composite<Tag, Elements
                                   , typename remove_reference<Modifiers>::type>::result_type
    type;
  };

  template <typename Elements, typename Modifiers>
  typename result<make_component(Elements, Modifiers)>::type
  operator()(Elements const& elements, Modifiers const& modifiers) const
  {
    // int i = 5;
    return ::morbid::iiop::generator::make_composite<Tag, Elements, Modifiers>()(elements, modifiers);
  }
};

template <>
struct make_component< ::morbid::iiop::generator_domain, tag::action>
{
  template <typename Sig>
  struct result;

  template <typename This, typename Elements, typename Modifiers>
  struct result<This(Elements, Modifiers)>
  {
    typedef typename
      boost::remove_const<typename Elements::car_type>::type
    subject_type;

    typedef typename
      boost::remove_const<typename Elements::cdr_type::car_type>::type
    action_type;

    typedef karma::action<subject_type, action_type> type;
  };

  template <typename Elements>
  typename result<make_component(Elements, unused_type)>::type
  operator()(Elements const& elements, unused_type) const
  {
    // int i = 5;
    typename result<make_component(Elements, unused_type)>::type
      result(elements.car, elements.cdr.car);
    return result;
  }
};

template <>
struct make_component< ::morbid::iiop::generator_domain, tag::directive>
{
  template <typename Sig>
  struct result;

  template <typename This, typename Elements, typename Modifiers>
  struct result<This(Elements, Modifiers)>
  {
    typedef typename
    ::morbid::iiop::generator::make_directive<
      typename boost::remove_const<typename Elements::car_type>::type,
      typename boost::remove_const<typename Elements::cdr_type::car_type>::type,
      typename boost::remove_reference<Modifiers>::type
      >::result_type
    type;
  };

  template <typename Elements, typename Modifiers>
  typename result<make_component(Elements, Modifiers)>::type
  operator()(Elements const& elements, Modifiers const& modifiers) const
  {
    // int i = 5;
    return ::morbid::iiop::generator::make_directive<
      typename boost::remove_const<typename Elements::car_type>::type,
      typename boost::remove_const<typename Elements::cdr_type::car_type>::type,
      Modifiers>()(elements.car, elements.cdr.car, modifiers);
  }
};

template <>
struct make_component< ::morbid::iiop::parser_domain, proto::tag::terminal>
{
  template <typename Sig>
  struct result;

  template <typename This, typename Elements, typename Modifiers>
  struct result<This(Elements, Modifiers)>
  {
    typedef typename ::morbid::iiop::parser::make_primitive<
      typename remove_const<typename Elements::car_type>::type
      , typename remove_reference<Modifiers>::type
      >::result_type type;
  };

  template <typename Elements, typename Modifiers>
  typename result<make_component(Elements, Modifiers)>::type
  operator()(Elements const& elements, Modifiers const& modifiers) const
  {
    typedef typename remove_const<typename Elements::car_type>::type term;
    return ::morbid::iiop::parser::make_primitive<term, Modifiers>()
      (elements.car, modifiers);
  }
};

template <typename Tag>
struct make_component< ::morbid::iiop::parser_domain, Tag>
{
  template <typename Sig>
  struct result;

  template <typename This, typename Elements, typename Modifiers>
  struct result<This(Elements, Modifiers)>
  {
    typedef typename
    ::morbid::iiop::parser::make_composite
      <Tag, Elements, typename remove_reference<Modifiers>::type>::result_type
    type;
  };

  template <typename Elements, typename Modifiers>
  typename result<make_component(Elements, Modifiers)>::type
  operator()(Elements const& elements, Modifiers const& modifiers) const
  {
    return ::morbid::iiop::parser::make_composite<Tag, Elements, Modifiers>()
      (elements, modifiers);
  }
};

template <>
struct make_component< ::morbid::iiop::parser_domain, tag::action>
{
  template <typename Sig>
  struct result;

  template <typename This, typename Elements, typename Modifiers>
  struct result<This(Elements, Modifiers)>
  {
    typedef typename
      boost::remove_const<typename Elements::car_type>::type
    subject_type;

    typedef typename
      boost::remove_const<typename Elements::cdr_type::car_type>::type
    action_type;

    typedef karma::action<subject_type, action_type> type;
  };

  template <typename Elements>
  typename result<make_component(Elements, unused_type)>::type
  operator()(Elements const& elements, unused_type) const
  {
    typename result<make_component(Elements, unused_type)>::type
      result(elements.car, elements.cdr.car);
    return result;
  }
};

template <>
struct make_component< ::morbid::iiop::parser_domain, tag::directive>
{
  template <typename Sig>
  struct result;

  template <typename This, typename Elements, typename Modifiers>
  struct result<This(Elements, Modifiers)>
  {
    typedef typename
    ::morbid::iiop::parser::make_directive<
      typename boost::remove_const<typename Elements::car_type>::type,
      typename boost::remove_const<typename Elements::cdr_type::car_type>::type,
      typename boost::remove_reference<Modifiers>::type
      >::result_type
    type;
  };

  template <typename Elements, typename Modifiers>
  typename result<make_component(Elements, Modifiers)>::type
  operator()(Elements const& elements, Modifiers const& modifiers) const
  {
    return ::morbid::iiop::parser::make_directive<
      typename boost::remove_const<typename Elements::car_type>::type,
      typename boost::remove_const<typename Elements::cdr_type::car_type>::type,
      Modifiers>()(elements.car, elements.cdr.car, modifiers);
  }
};

} }

namespace morbid { namespace iiop {




} }

#endif
