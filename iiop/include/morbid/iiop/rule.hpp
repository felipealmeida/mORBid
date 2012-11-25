/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_RULE_HPP
#define MORBID_IIOP_RULE_HPP

#include <morbid/giop/rule.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/support.hpp>

namespace morbid { namespace iiop {

namespace fusion = boost::fusion;
namespace mpl = boost::mpl;

namespace generator {

template <typename I, typename T1, typename T2, typename T3, typename T4>
struct rule
{
  typedef I param_iterator;

  typedef mpl::vector<T1, T2, T3, T4> template_params;

  typedef typename
    spirit::detail::extract_locals<template_params>::type
  locals_type;

  typedef typename
    spirit::detail::extract_sig<template_params>::type
  sig_type;

  typedef typename
    spirit::detail::attr_from_sig<sig_type>::type
  attr_type;

  typedef typename boost::add_reference<
    typename boost::add_const<attr_type>::type>::type
  attr_reference_type;

  typedef typename
    spirit::detail::params_from_sig<sig_type>::type
  parameter_types;

  typedef spirit::context<
    fusion::cons<attr_reference_type, parameter_types>
    , locals_type>
  context_type;

  typedef mpl::int_<karma::generator_properties::all_properties> properties;
  typedef karma::detail::output_iterator
    <param_iterator, properties> output_iterator;

  typedef boost::function
    <bool(output_iterator&, context_type&, /*delimiter_type*/spirit::unused_type const&)>
  function_type;

  template <typename Expr>
  rule& operator=(Expr const& expr)
  {
    f = karma::detail::bind_generator<mpl::false_>(expr);
    return *this;
  }

  template <typename Expr>
  rule& operator%=(Expr const& expr)
  {
    f = karma::detail::bind_generator<mpl::true_>(expr);
    return *this;
  }

  function_type f;
};

} }

namespace giop {

template <typename I, typename T1, typename T2, typename T3, typename T4>
struct rule_impl< iiop::generator_domain, I, T1, T2, T3, T4>
{
  typedef iiop::generator::rule<I, T1, T2, T3, T4> type;
};

}

namespace iiop { namespace generator {

template <typename R, typename Params>
struct rule_generator
{
  rule_generator(R const& rule, Params const& params)
    : rule(&rule), params(params)
  {
    std::cout << "rule_generator::rule_generator rule " << &rule
              << " this " << this
              << " this rule " << this->rule << std::endl;
  }

  typedef R rule_type;
  typedef mpl::int_<karma::generator_properties::all_properties> properties;

  template <typename Context, typename Iterator>
  struct attributes
  {
    typedef spirit::unused_type type;
  };

  typedef typename rule_type::output_iterator output_iterator;

  template <typename Context, typename Delimiter, typename Attr>
  bool generate(output_iterator& sink, Context& caller_context
                , Delimiter const& d, Attr const& attr) const
  {
    typedef typename rule_type::attr_type attr_type;
    BOOST_MPL_ASSERT((boost::is_same<Attr, attr_type>));
    // Create an attribute if none is supplied.
    typedef spirit::traits::make_attribute<attr_type, Attr>
      make_attribute;
    typedef spirit::traits::transform_attribute<
      typename make_attribute::type, attr_type, karma::domain>
      transform;

    typename transform::type attr_ =
      spirit::traits::pre_transform<karma::domain, attr_type>
      (make_attribute::call(attr));

    typedef typename rule_type::context_type context_type;

    // If you are seeing a compilation error here, you are probably
    // trying to use a rule or a grammar which has inherited
    // attributes, without passing values for them.
    context_type context(attr_, params, caller_context);

    std::cout << "generate for rule " << rule << std::endl;
    std::cout << "generate this " << this << std::endl;

    // If you are seeing a compilation error here stating that the
    // third parameter can't be converted to a karma::reference
    // then you are probably trying to use a rule or a grammar with
    // an incompatible delimiter type.
    if (rule->f(sink, context, d))
    {
      // typedef typename rule_type::delimiter_type delimiter_type;
      // // do a post-delimit if this is an implied verbatim
      // if (boost::is_same<delimiter_type, spirit::unused_type>::value)
      //   karma::delimit_out(sink, d);

      return true;
    }
    return false;
  }  

  R const* rule;
  Params params;
};

template <typename Iterator, typename T1, typename T2, typename T3, typename T4, typename Modifiers, typename Enable>
struct make_primitive<boost::reference_wrapper
                      <giop::rule< generator_domain, Iterator, T1, T2, T3, T4>const>
                       , Modifiers, Enable>
{
  typedef giop::rule< generator_domain, Iterator, T1, T2, T3, T4> rule_type;
  typedef rule_generator<rule_type, fusion::vector0<> > result_type;

  result_type operator()(rule_type const& val, boost::spirit::unused_type) const
  {
    return result_type(val);
  }
};

template <typename D, typename Iterator, typename T1, typename T2, typename T3, typename T4
          , typename Params, typename Modifiers, typename Enable>
struct make_primitive<giop::parameterized_nonterminal
                      <D, rule<Iterator, T1, T2, T3, T4>, Params>, Modifiers, Enable>
{
  BOOST_MPL_ASSERT((boost::is_same<D, generator_domain>));
  typedef rule<Iterator, T1, T2, T3, T4> rule_type;
  typedef rule_generator<rule_type, Params> result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, boost::spirit::unused_type) const
  {
    std::cout << "rule this: " << &term.ref.get() << std::endl;
    return result_type(term.ref.get(), term.params);
  }
};

} } }

namespace boost { namespace spirit {

template <typename Iterator, typename T1, typename T2, typename T3, typename T4, typename Enable>
struct use_terminal< ::morbid::iiop::generator_domain
                     , boost::reference_wrapper
                     < ::morbid::iiop::generator::rule<Iterator, T1, T2, T3, T4>const>, Enable> : mpl::true_ {};

template <typename D, typename Iterator, typename T1, typename T2, typename T3, typename T4
          , typename Params, typename Enable>
struct use_terminal< ::morbid::iiop::generator_domain
                     , ::morbid::giop::parameterized_nonterminal
                     <D, ::morbid::iiop::generator::rule< Iterator, T1, T2, T3, T4>, Params>, Enable> : mpl::true_ {};

} }

#endif
