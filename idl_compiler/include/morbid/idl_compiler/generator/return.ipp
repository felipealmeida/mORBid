/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_RETURN_IPP
#define MORBID_IDL_COMPILER_RETURN_IPP

#include <morbid/idl_compiler/generator/return.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/phoenix.hpp>
#include <boost/phoenix/fusion/at.hpp>

namespace morbid { namespace idlc { namespace generator {

namespace karma = boost::spirit::karma;
namespace phoenix = boost::phoenix;

template <typename OutputIterator>
return_<OutputIterator>::return_()
    : return_::base_type(start)
{
  using karma::_1;
  using karma::_r1;
  using karma::_val;
  using phoenix::at_c;
  namespace types = idl_parser::types;

  start = karma::string[_1 = "::morbid::return_traits< "]
    << (floating_point | integer | char_ | wchar_ | boolean | octet
        | any | object | value_base | void_ | scoped_name(_r1)
        | sequence(_r1)) [_1 = at_c<0>(_val)]
    << " >::type"
    ;
  floating_point =
    (
     karma::eps(at_c<0>(_val) == types::floating_point::float_)
     << "float"
    )
    | (
     karma::eps(at_c<0>(_val) == types::floating_point::double_)
     << "double"
    )
    | (
     karma::eps(at_c<0>(_val) == types::floating_point::long_double_)
     << "::morbid::long_double"
    )
    ;
  integer =
    (
     karma::eps(at_c<0>(_val) == types::integer::signed_short_int)
     << "::morbid::short_"
    )
    | (
     karma::eps(at_c<0>(_val) == types::integer::signed_long_int)
     << "::morbid::long_"
    )
    | (
     karma::eps(at_c<0>(_val) == types::integer::signed_longlong_int)
     << "::morbid::long_long"
    )
    | (
     karma::eps(at_c<0>(_val) == types::integer::unsigned_short_int)
     << "::morbid::unsigned_short"
    )
    | (
     karma::eps(at_c<0>(_val) == types::integer::unsigned_long_int)
     << "::morbid::unsigned_long"
    )
    | (
     karma::eps(at_c<0>(_val) == types::integer::unsigned_longlong_int)
     << "::morbid::unsigned_long_long"
    )
    ;
  char_ = karma::string[_1 = "char"];
  wchar_ = karma::string[_1 = "wchar_t"];
  boolean = karma::string[_1 = "bool"];
  octet = karma::string[_1 = "::morbid::octet"];
  // any = karma::string[_1 = "::morbid::Any"];
  object = karma::string[_1 = "::morbid::object"];
  // value_base = karma::string[_1 = "::morbid::ValueBase"];
  void_ = karma::string[_1 = "void"];
  sequence =
    karma::lit("::morbid::sequence<")
    << ">";
}

} } }

#endif
