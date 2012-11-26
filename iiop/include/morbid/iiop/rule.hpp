/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_RULE_HPP
#define MORBID_IIOP_RULE_HPP

#include <morbid/giop/rule.hpp>
#include <morbid/iiop/endianness.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/support.hpp>

namespace morbid { namespace iiop {

namespace fusion = boost::fusion;
namespace mpl = boost::mpl;
namespace spirit = boost::spirit;
namespace proto = boost::proto;
namespace qi = spirit::qi;
namespace karma = spirit::karma;

template <typename I, typename T1, typename T2, typename T3, typename T4>
struct rule_base
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
  attr_const_reference_type;
  typedef typename boost::add_reference<attr_type>::type
    attr_reference_type;
};

namespace parser {

template <typename I, typename T1, typename T2, typename T3, typename T4>
struct rule : iiop::rule_base<I, T1, T2, T3, T4>
{
  typedef iiop::rule_base<I, T1, T2, T3, T4> rule_base;
  typedef typename rule_base::locals_type locals_type;
  typedef typename rule_base::attr_type attr_type;
  typedef typename rule_base::attr_reference_type attr_reference_type;
  typedef typename rule_base::sig_type sig_type;

  typedef typename
    spirit::detail::params_from_sig<sig_type>::type
  parameter_types;

  typedef spirit::context<
    fusion::cons<attr_reference_type, parameter_types>
    , locals_type>
  context_type;

  typedef boost::function
    <bool(I& /*first*/, I const& /*last*/
          , context_type& /*context*/
          , spirit::unused_type const& /*skipper*/
          )>
  function_type;
  
  template <typename Expr>
  rule& operator=(Expr const& expr)
  {
    BOOST_MPL_ASSERT((spirit::traits::is_parser<Expr>));
    // BOOST_MPL_ASSERT((boost::is_same<Expr, void>));
    f = qi::detail::bind_parser<mpl::false_>(expr);
    return *this;
  }

  template <typename Expr>
  rule& operator%=(Expr const& expr)
  {
    BOOST_MPL_ASSERT((spirit::traits::is_parser<Expr>));
    BOOST_MPL_ASSERT((boost::is_same<Expr, void>));
    f = qi::detail::bind_parser<mpl::true_>(expr);
    return *this;
  }

  function_type f;
};

}

namespace generator {

template <typename I, typename T1, typename T2, typename T3, typename T4>
struct rule : iiop::rule_base<I, T1, T2, T3, T4>
{
  typedef iiop::rule_base<I, T1, T2, T3, T4> rule_base;
  typedef typename rule_base::locals_type locals_type;
  typedef typename rule_base::attr_type attr_type;
  typedef typename rule_base::attr_const_reference_type attr_const_reference_type;
  typedef typename rule_base::sig_type sig_type;

  typedef typename fusion::result_of::as_list
  <typename fusion::result_of::push_back
  <typename spirit::detail::params_from_sig<sig_type>::type
   , iiop::endianness_attribute>::type>::type
  parameter_types;

  typedef spirit::context<
    fusion::cons<attr_const_reference_type, parameter_types>
    , locals_type>
  context_type;

  typedef mpl::int_<karma::generator_properties::all_properties> properties;

  typedef I param_iterator;
  
  typedef karma::detail::output_iterator
    <param_iterator, properties> output_iterator;

  typedef boost::function
    <bool(output_iterator&, context_type&, /*delimiter_type*/spirit::unused_type const&)>
  function_type;

  rule()
  {
  }

  template <typename Expr>
  rule& operator=(Expr const& expr)
  {
    BOOST_MPL_ASSERT((spirit::traits::is_generator<Expr>));
    f = karma::detail::bind_generator<mpl::false_>(expr);
    return *this;
  }

  template <typename Expr>
  rule& operator%=(Expr const& expr)
  {
    BOOST_MPL_ASSERT((spirit::traits::is_generator<Expr>));
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

template <typename I, typename T1, typename T2, typename T3, typename T4>
struct rule_impl< iiop::parser_domain, I, T1, T2, T3, T4>
{
  typedef iiop::parser::rule<I, T1, T2, T3, T4> type;
};

}

namespace iiop {

namespace parser {

template <typename R, typename Params>
struct rule_parser : qi::primitive_parser<rule_parser<R, Params> >
{
  rule_parser(R const& rule, Params const& params)
    : rule(&rule), params(params)
  {
  }

  template <typename Iterator, typename Context, typename Skipper, typename Attribute>
  bool parse(Iterator& first, Iterator const& last
             , Context& caller_context, Skipper const& skipper
             , Attribute& attr_param) const
  {
    std::cout << "rule parse" << std::endl;
    if(rule->f)
    {
      // do a preskip if this is an implied lexeme
      // if (boost::is_same<skipper_type, unused_type>::value)
      //   qi::skip_over(first, last, skipper);

      typedef typename R::attr_type attr_type;
      typedef typename R::context_type context_type;

      typedef spirit::traits::make_attribute<attr_type, Attribute> make_attribute;

      // do down-stream transformation, provides attribute for
      // rhs parser
      typedef spirit::traits::transform_attribute<
        typename make_attribute::type, attr_type, qi::domain>
        transform;

      typename make_attribute::type made_attr = make_attribute::call(attr_param);
      typename transform::type attr_ = transform::pre(made_attr);

      // If you are seeing a compilation error here, you are probably
      // trying to use a rule or a grammar which has inherited
      // attributes, without passing values for them.
      context_type context(attr_, params, caller_context);

      // If you are seeing a compilation error here stating that the
      // fourth parameter can't be converted to a required target type
      // then you are probably trying to use a rule or a grammar with
      // an incompatible skipper type.
      if (rule->f(first, last, context, skipper))
      {
        // do up-stream transformation, this integrates the results
        // back into the original attribute value, if appropriate
        spirit::traits::post_transform(attr_param, attr_);
        return true;
      }

      // inform attribute transformation of failed rhs
      spirit::traits::fail_transform(attr_param, attr_);
    }
    return false;
  }  

  R const* rule;
  Params params;
};

template <typename Iterator, typename T1, typename T2, typename T3, typename T4, typename Modifiers, typename Enable>
struct make_primitive<boost::reference_wrapper
                      <parser::rule<Iterator, T1, T2, T3, T4>const>
                       , Modifiers, Enable>
{
  typedef parser::rule<Iterator, T1, T2, T3, T4> rule_type;
  typedef rule_parser<rule_type, fusion::vector0<> > result_type;

  result_type operator()(rule_type const& val, boost::spirit::unused_type) const
  {
    return result_type(val, fusion::vector0<>());
  }
};

template <typename D, typename Iterator, typename T1, typename T2, typename T3, typename T4
          , typename Params, typename Modifiers, typename Enable>
struct make_primitive<giop::parameterized_nonterminal
                      <D, rule<Iterator, T1, T2, T3, T4>, Params>, Modifiers, Enable>
{
  BOOST_MPL_ASSERT((boost::is_same<D, parser_domain>));
  typedef rule<Iterator, T1, T2, T3, T4> rule_type;
  typedef rule_parser<rule_type, Params> result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, boost::spirit::unused_type) const
  {
    std::cout << "rule this: " << &term.ref.get() << std::endl;
    return result_type(term.ref.get(), term.params);
  }
};

}

namespace generator {

template <typename R, typename Params>
struct rule_generator : karma::primitive_generator<rule_generator<R, Params> >
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
    std::cout << "rule_generator::generate " << rule->f << std::endl;
    if(rule->f)
    {
      typedef typename rule_type::attr_type attr_type;
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
      BOOST_MPL_ASSERT((mpl::not_<boost::is_const<context_type> >));

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

template <typename Iterator, typename T1, typename T2, typename T3, typename T4, typename Enable>
struct use_terminal< ::morbid::iiop::parser_domain
                     , boost::reference_wrapper
                     < ::morbid::iiop::generator::rule<Iterator, T1, T2, T3, T4>const>, Enable> : mpl::true_ {};

template <typename D, typename Iterator, typename T1, typename T2, typename T3, typename T4
          , typename Params, typename Enable>
struct use_terminal< ::morbid::iiop::parser_domain
                     , ::morbid::giop::parameterized_nonterminal
                     <D, ::morbid::iiop::generator::rule< Iterator, T1, T2, T3, T4>, Params>, Enable> : mpl::true_ {};

// template <typename R, typename Params, typename Enable>
// struct use_terminal< qi::domain
//                      , ::morbid::iiop::parser::rule_parser<R, Params>, Enable>
//  : mpl::true_ {};

} }

#endif
