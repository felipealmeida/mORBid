/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_AUXILIARY_HPP
#define MORBID_IIOP_AUXILIARY_HPP

#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/support.hpp>

namespace morbid { namespace iiop {

namespace spirit = boost::spirit;
namespace qi = spirit::qi;
namespace karma = spirit::karma;

namespace parser {

template <typename Modifiers>
struct make_primitive<spirit::tag::eps, Modifiers>
{
  typedef qi::eps_parser result_type;
  result_type operator()(spirit::unused_type, spirit::unused_type) const
  {
    return result_type();
  }
};

template <typename Modifiers, typename A0>
struct make_primitive<
  spirit::terminal_ex<spirit::tag::eps, fusion::vector1<A0> >
  , Modifiers>
{
  typedef qi::semantic_predicate result_type;
  template <typename Terminal>
  result_type operator()(Terminal const& term, spirit::unused_type) const
  {
    return result_type(fusion::at_c<0>(term.args) ? true : false);
  }
};

template <typename Terminal, typename Actor, int Arity, typename Modifiers>
struct make_primitive<spirit::lazy_terminal<Terminal, Actor, Arity>, Modifiers>
{
  typedef qi::lazy_parser<Actor, Modifiers> result_type;
  result_type operator()(spirit::lazy_terminal<Terminal, Actor, Arity> const& lt
                         , Modifiers const& modifiers) const
  {
    return result_type(lt.actor, modifiers);
  }
};

}

namespace generator {

template <typename Modifiers>
struct make_primitive<spirit::tag::eps, Modifiers>
{
  typedef karma::eps_generator result_type;
  result_type operator()(spirit::unused_type, spirit::unused_type) const
  {
    return result_type();
  }
};
  
template <typename Modifiers, typename A0>
struct make_primitive<
  spirit::terminal_ex<spirit::tag::eps, fusion::vector1<A0> >
  , Modifiers>
{
  typedef karma::semantic_predicate result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, spirit::unused_type) const
  {
    return result_type(fusion::at_c<0>(term.args));
  }
};

}

} }

namespace boost { namespace spirit {

template <>
struct use_terminal< ::morbid::iiop::parser_domain, spirit::tag::eps>       // enables eps
  : mpl::true_ {};

template <typename A0>
struct use_terminal
  < ::morbid::iiop::parser_domain
    , terminal_ex<tag::eps, fusion::vector1<A0> > // enables eps(bool-condition)
  > : is_convertible<A0, bool> {};

template <>                                     // enables eps(f)
struct use_lazy_terminal<
  ::morbid::iiop::parser_domain, tag::eps, 1 /*arity*/
  > : mpl::true_ {};

// enables eps
template <>
struct use_terminal< ::morbid::iiop::generator_domain, tag::eps>
  : mpl::true_ {};

// enables eps(bool-condition)
template <typename A0>
struct use_terminal
 < ::morbid::iiop::generator_domain
   , terminal_ex<tag::eps, fusion::vector1<A0> > > 
: is_convertible<A0, bool> {};

// enables lazy eps(f)
template <>
struct use_lazy_terminal< ::morbid::iiop::generator_domain, tag::eps, 1>
  : mpl::true_ {};

} }

#endif
