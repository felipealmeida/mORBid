/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/giop/compile.hpp>

namespace test_compile {

struct domain
{
};

struct rule;

}

namespace morbid { namespace giop {

template <>
struct compile_impl< ::test_compile::domain>
{
  template <typename T>
  struct result;
  template <typename This, typename Expr>
  struct result<This(Expr)>
    : boost::spirit::result_of::compile< ::test_compile::domain, Expr>
  {
  };

  template <typename Expr>
  typename result<compile_impl< ::test_compile::domain>(Expr)>::type
  operator()(Expr const& expr) const
  {
    return boost::spirit::compile< ::test_compile::domain>(expr);
  }
};

template <typename I, typename T1, typename T2, typename T3
          , typename T4>
struct rule_impl<test_compile::domain, I, T1, T2, T3, T4>
{
  typedef test_compile::rule type;
};

} }

namespace test_compile {

struct rule
{
  rule() {}

  template <typename C>
  rule& operator=(C const& c)
  {
    return *this;
  }

  template <typename C>
  rule& operator%=(C const& c)
  {
    return *this;
  }

private:
  rule(rule const&) {}
  rule& operator=(rule const&);
};

template <typename T, typename Modifiers, typename Enable = void>
struct make_primitive;

template <typename Tag, typename Elements
          , typename Modifiers, typename Enable = void>
struct make_composite;

template <typename Elements>
struct sequence
{
  sequence(Elements const& subject)
  {
  }

  typedef Elements elements;
};

template <typename Elements, typename Modifiers>
struct make_composite<boost::proto::tag::bitwise_and, Elements, Modifiers>
  : boost::spirit::make_nary_composite<Elements, sequence>
{};

}

namespace boost { namespace spirit {

template <typename Enable>
struct use_operator< test_compile::domain, proto::tag::bitwise_and, Enable> : mpl::true_ {};

template <>
struct flatten_tree< test_compile::domain, proto::tag::bitwise_and>
  : mpl::true_ {};

template <typename Enable>
struct use_terminal< test_compile::domain, tag::float_, Enable> : mpl::true_ {};

template <typename Enable>
struct use_terminal< test_compile::domain, tag::double_, Enable> : mpl::true_ {};

template <typename Enable>
struct use_terminal< test_compile::domain, ::morbid::giop::tag::long_double, Enable> : mpl::true_ {};

template <typename Enable>
struct use_terminal< test_compile::domain, tag::ushort_, Enable> : mpl::true_ {};

template <typename Enable>
struct use_terminal< test_compile::domain
                     , boost::reference_wrapper< ::test_compile::rule const>
                     , Enable>
  : mpl::true_ {};

template <typename D, typename Params, typename Enable>
struct use_terminal< test_compile::domain
                     , morbid::giop::parameterized_nonterminal<D, ::test_compile::rule, Params>
                     , Enable>
  : mpl::true_ {};

template <>
struct make_component< test_compile::domain, proto::tag::terminal>
{
  template <typename Sig>
  struct result;

  template <typename This, typename Elements, typename Modifiers>
  struct result<This(Elements, Modifiers)>
  {
    typedef typename test_compile::make_primitive<
      typename remove_const<typename Elements::car_type>::type
      , typename remove_reference<Modifiers>::type
      >::result_type type;
  };

  template <typename Elements, typename Modifiers>
  typename result<make_component(Elements, Modifiers)>::type
  operator()(Elements const& elements, Modifiers const& modifiers) const
  {
    typedef typename remove_const<typename Elements::car_type>::type term;
    // BOOST_MPL_ASSERT((boost::is_same<term, boost::spirit::tag::float_>));
    return test_compile::make_primitive<term, Modifiers>()(elements.car, modifiers);
  }
};

template <typename Tag>
struct make_component< test_compile::domain, Tag>
{
  template <typename Sig>
  struct result;

  template <typename This, typename Elements, typename Modifiers>
  struct result<This(Elements, Modifiers)>
  {
    typedef typename
    test_compile::make_composite<Tag, Elements
                   , typename remove_reference<Modifiers>::type>::result_type
    type;
  };

  template <typename Elements, typename Modifiers>
  typename result<make_component(Elements, Modifiers)>::type
  operator()(Elements const& elements, Modifiers const& modifiers) const
  {
    return test_compile::make_composite<Tag, Elements, Modifiers>()(elements, modifiers);
  }
};

} }

