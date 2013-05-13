/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_TYPE_SPEC_HPP
#define MORBID_IDL_COMPILER_TYPE_SPEC_HPP

#include <morbid/idl_compiler/common_types.hpp>
#include <morbid/idl_compiler/lookuped_type.hpp>
#include <morbid/idl_compiler/module.hpp>
#include <morbid/idl_compiler/generator/scoped_name.hpp>
#include <morbid/idl_parser/type_spec.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/phoenix.hpp>
#include <boost/fusion/include/adapt_adt.hpp>

namespace morbid { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;
namespace phoenix = boost::phoenix;

template <typename OutputIterator, typename Iterator>
struct type_spec : karma::grammar<OutputIterator, idl_parser::type_spec
                                  (lookuped_type)>
{
  type_spec()
    : type_spec::base_type(start)
  {
    using karma::_1;
    using karma::_r1;
    using karma::_val;
    using phoenix::at_c;
    namespace types = idl_parser::types;

    start = 
      (floating_point | integer | char_ | wchar_ | boolean | octet
       | any | object | value_base | void_ | scoped_name(_r1)
       | sequence(_r1)) [_1 = at_c<0>(_val)]
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
    // object = karma::string[_1 = "::morbid::Object"];
    // value_base = karma::string[_1 = "::morbid::ValueBase"];
    void_ = karma::string[_1 = "void"];
    sequence =
      karma::lit("::morbid::sequence<")
      << ">";
  }

  generator::scoped_name<OutputIterator, Iterator> scoped_name;
  karma::rule<OutputIterator, idl_parser::type_spec(lookuped_type)> start;
  karma::rule<OutputIterator, idl_parser::types::floating_point()> floating_point;
  karma::rule<OutputIterator, idl_parser::types::integer()> integer;
  karma::rule<OutputIterator, idl_parser::types::char_()> char_;
  karma::rule<OutputIterator, idl_parser::types::wchar_()> wchar_;
  karma::rule<OutputIterator, idl_parser::types::boolean()> boolean;
  karma::rule<OutputIterator, idl_parser::types::octet()> octet;
  karma::rule<OutputIterator, idl_parser::types::any()> any;
  karma::rule<OutputIterator, idl_parser::types::object()> object;
  karma::rule<OutputIterator, idl_parser::types::value_base()> value_base;
  karma::rule<OutputIterator, idl_parser::types::void_()> void_;
  karma::rule<OutputIterator, idl_parser::types::sequence(lookuped_type_wrapper)> sequence;
  
};

} } }

#endif
