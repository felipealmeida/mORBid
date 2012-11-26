/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_OPERATORS_HPP
#define MORBID_IIOP_OPERATORS_HPP

#include <morbid/iiop/meta_compiler.hpp>

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

} }

namespace morbid { namespace iiop {

namespace generator {

template <typename Elements, typename Modifiers>
struct make_composite<boost::proto::tag::bitwise_and, Elements, Modifiers>
  : boost::spirit::make_nary_composite<Elements, boost::spirit::karma::sequence>
{};

template <typename Elements, typename Modifiers>
struct make_composite<boost::proto::tag::logical_and, Elements, Modifiers>
  : boost::spirit::make_nary_composite<Elements, boost::spirit::karma::strict_sequence>
{};

}

namespace parser {

template <typename Elements, typename Modifiers>
struct make_composite<boost::proto::tag::bitwise_and, Elements, Modifiers>
  : boost::spirit::make_nary_composite<Elements, boost::spirit::qi::sequence>
{};

// template <typename Elements, typename Modifiers>
// struct make_composite<boost::proto::tag::logical_and, Elements, Modifiers>
//   : boost::spirit::make_nary_composite<Elements, boost::spirit::qi::strict_sequence>
// {};

}

} }

#endif
