/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_OPERATORS_HPP
#define MORBID_IIOP_OPERATORS_HPP

#include <morbid/iiop/meta_compiler.hpp>

#include <boost/spirit/home/karma.hpp>

namespace boost { namespace spirit {

template <typename Enable>
struct use_operator< ::morbid::iiop::generator_domain, proto::tag::bitwise_and, Enable> : mpl::true_ {};

template <typename Enable>
struct use_operator< ::morbid::iiop::generator_domain, proto::tag::logical_and, Enable> : mpl::true_ {};

template <>
struct flatten_tree< ::morbid::iiop::generator_domain, proto::tag::bitwise_and>
  : mpl::true_ {};

template <>
struct flatten_tree< ::morbid::iiop::generator_domain, proto::tag::logical_and>
  : mpl::true_ {};

template <>
struct use_operator< ::morbid::iiop::generator_domain, proto::tag::dereference> // enables *g
  : mpl::true_ {};

template <>
struct use_operator< ::morbid::iiop::generator_domain, proto::tag::unary_plus> // enables +p
  : mpl::true_ {};

template <>
struct use_operator< ::morbid::iiop::generator_domain, proto::tag::bitwise_or>  // enables |
  : mpl::true_ {};

template <>
struct flatten_tree< ::morbid::iiop::generator_domain, proto::tag::bitwise_or>  // flattens |
  : mpl::true_ {};

template <typename Enable>
struct use_operator< ::morbid::iiop::parser_domain, proto::tag::bitwise_and, Enable> : mpl::true_ {};

template <typename Enable>
struct use_operator< ::morbid::iiop::parser_domain, proto::tag::logical_and, Enable> : mpl::true_ {};

template <>
struct flatten_tree< ::morbid::iiop::parser_domain, proto::tag::bitwise_and>
  : mpl::true_ {};

template <>
struct flatten_tree< ::morbid::iiop::parser_domain, proto::tag::logical_and>
  : mpl::true_ {};

template <>
struct use_operator< ::morbid::iiop::parser_domain, proto::tag::dereference> // enables *p
  : mpl::true_ {};

template <>
struct use_operator< ::morbid::iiop::parser_domain, proto::tag::unary_plus> // enables +p
  : mpl::true_ {};

template <>
struct use_operator< ::morbid::iiop::parser_domain, proto::tag::bitwise_or>  // enables |
  : mpl::true_ {};

template <>
struct flatten_tree< ::morbid::iiop::parser_domain, proto::tag::bitwise_or>  // flattens |
  : mpl::true_ {};

} }

namespace morbid { namespace iiop {

namespace proto = boost::proto;
namespace spirit = boost::spirit;

namespace generator {

template <typename Elements, typename Modifiers>
struct make_composite<proto::tag::bitwise_and, Elements, Modifiers>
  : spirit::make_nary_composite<Elements, boost::spirit::karma::sequence>
{};

template <typename Elements, typename Modifiers>
struct make_composite<proto::tag::logical_and, Elements, Modifiers>
  : spirit::make_nary_composite<Elements, boost::spirit::karma::strict_sequence>
{};

template <typename Subject, typename Modifiers>
struct make_composite<proto::tag::dereference, Subject, Modifiers>
  : karma::make_composite<proto::tag::dereference, Subject, Modifiers>
{};

template <typename Subject, typename Modifiers>
struct make_composite<proto::tag::unary_plus, Subject, Modifiers>
  : karma::make_composite<proto::tag::unary_plus, Subject, Modifiers>
{};

template <typename Elements, typename Modifiers>
struct make_composite<proto::tag::bitwise_or, Elements, Modifiers>
  : karma::make_composite<proto::tag::bitwise_or, Elements, Modifiers>
{};

}

namespace parser {

template <typename Elements, typename Modifiers>
struct make_composite<proto::tag::bitwise_and, Elements, Modifiers>
  : boost::spirit::make_nary_composite<Elements, qi::sequence>
{};

template <typename Subject, typename Modifiers>
struct make_composite<proto::tag::dereference, Subject, Modifiers>
  : qi::make_composite<proto::tag::dereference, Subject, Modifiers>
{};

template <typename Subject, typename Modifiers>
struct make_composite<proto::tag::unary_plus, Subject, Modifiers>
  : qi::make_composite<proto::tag::unary_plus, Subject, Modifiers>
{};

template <typename Elements, typename Modifiers>
struct make_composite<proto::tag::bitwise_or, Elements, Modifiers>
  : qi::make_composite<proto::tag::bitwise_or, Elements, Modifiers>
{};

}

} }

#endif
