/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_GIOP_RULE_HPP
#define MORBID_GIOP_RULE_HPP

#include <morbid/giop/common_terminals.hpp>
#include <morbid/giop/compile.hpp>
#include <morbid/giop/detail/parameterized_nonterminal.hpp>
#include <morbid/giop/detail/max_args.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>

namespace morbid { namespace giop {

namespace spirit = boost::spirit;
namespace karma = spirit::karma;
namespace mpl = boost::mpl;
namespace proto = boost::proto;
namespace fusion = boost::fusion;

template <typename Domain, typename I, typename T1, typename T2, typename T3
          , typename T4>
struct rule_impl;

template <typename Domain, typename Iterator, typename T1, typename T2 = spirit::unused_type
          , typename T3 = spirit::unused_type, typename T4 = spirit::unused_type>
struct rule
  : proto::extends
    <typename proto::terminal
     <boost::reference_wrapper<rule<Domain, Iterator, T1, T2, T3, T4> const> >::type
     , rule<Domain, Iterator, T1, T2, T3, T4>
    >
{
  typedef rule<Domain, Iterator, T1, T2, T3, T4> self_type;
  typedef typename proto::terminal<boost::reference_wrapper<self_type const> >::type terminal_type;

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

  static size_t const params_size = fusion::result_of::size<parameter_types>::type::value;

  rule()
    : proto::extends<terminal_type, self_type>(terminal_type::make(boost::cref(*this)))
  {}
  ~rule()
  {
    std::cout << "~rule " << this << std::endl;
  }

  template <typename Expr>
  self_type& operator=(Expr const& expr)
  {
    typedef typename giop::result_of::compile<Domain, Expr>::type compilation_result;
    r = karma::detail::bind_generator<mpl::false_>(giop::compile<Domain>(expr));
    return *this;
  }

  template <typename Expr>
  self_type& operator%=(Expr const& expr)
  {
    typedef typename giop::result_of::compile<Domain, Expr>::type compilation_result;
    r %= karma::detail::bind_generator<mpl::true_>(giop::compile<Domain>(expr));
    return *this;
  }

  typedef Domain domain_type;

  self_type const& get_parameterized_subject() const { return *this; }
  typedef self_type parameterized_subject_type;
#define BOOST_PP_ITERATION_PARAMS_1 (3, (1, MORBID_GIOP_MAX_ARGS, "morbid/giop/detail/nonterminal_fcall.hpp"))
#include BOOST_PP_ITERATE()

  typedef typename rule_impl<Domain, Iterator, T1, T2, T3, T4>::type rule_type;
  rule_type r;
private:
  rule(self_type const&);
  self_type& operator=(self_type const&);
};

} }

#endif
