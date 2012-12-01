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

