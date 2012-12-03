/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_ORB_ARGUMENTS_TRAITS_HPP
#define MORBID_ORB_ARGUMENTS_TRAITS_HPP

#include <morbid/in_out_traits.hpp>
#include <morbid/type_tag.hpp>
#include <morbid/giop/common_terminals.hpp>

#include <boost/proto/core.hpp>
#include <boost/spirit/home/support.hpp>

namespace morbid {

namespace spirit = boost::spirit;

namespace detail {

template <typename T>
struct argument_giop_grammar;

template <>
struct argument_giop_grammar<morbid::String>
{
  typedef spirit::terminal<giop::tag::string>const& result_type;
  result_type operator()() const { return giop::string; }
};

template <>
struct argument_giop_grammar<morbid::Char>
{
  typedef spirit::terminal<giop::tag::octet>const& result_type;
  result_type operator()() const { return giop::octet; }
};

template <>
struct argument_giop_grammar<morbid::Boolean>
{
  typedef spirit::terminal<spirit::tag::bool_>const& result_type;
  result_type operator()() const { return giop::bool_; }
};

template <>
struct argument_giop_grammar<morbid::Double>
{
  typedef spirit::terminal<spirit::tag::double_>const& result_type;
  result_type operator()() const { return giop::double_; }
};

template <>
struct argument_giop_grammar<morbid::Float>
{
  typedef spirit::terminal<spirit::tag::float_>const& result_type;
  result_type operator()() const { return giop::float_; }
};

template <>
struct argument_giop_grammar<morbid::LongLong>
{
  typedef spirit::terminal<spirit::tag::long_long>const& result_type;
  result_type operator()() const { return giop::long_long; }
};

template <>
struct argument_giop_grammar<morbid::ULongLong>
{
  typedef spirit::terminal<spirit::tag::ulong_long>const& result_type;
  result_type operator()() const { return giop::ulong_long; }
};

template <>
struct argument_giop_grammar<morbid::Long>
{
  typedef spirit::terminal<spirit::tag::long_>const& result_type;
  result_type operator()() const { return giop::long_; }
};

template <>
struct argument_giop_grammar<morbid::ULong>
{
  typedef spirit::terminal<spirit::tag::ulong_>const& result_type;
  result_type operator()() const { return giop::ulong_; }
};

template <>
struct argument_giop_grammar<morbid::Short>
{
  typedef spirit::terminal<spirit::tag::short_>const& result_type;
  result_type operator()() const { return giop::short_; }
};

template <>
struct argument_giop_grammar<morbid::UShort>
{
  typedef spirit::terminal<spirit::tag::ushort_>const& result_type;
  result_type operator()() const { return giop::ushort_; }
};

template <>
struct argument_giop_grammar<morbid::Octet>
{
  typedef spirit::terminal<giop::tag::octet>const& result_type;
  result_type operator()() const { return giop::octet; }
};

}

struct arguments_traits
{
  template <typename T>
  typename detail::argument_giop_grammar<typename T::original_type>::result_type
  call() const
  {
    return detail::argument_giop_grammar<typename T::original_type>()();
  }
};

}

#endif

