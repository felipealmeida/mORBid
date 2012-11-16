/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_STRUCT_GENERATOR_GENERATOR_HPP
#define MORBID_IDL_COMPILER_STRUCT_GENERATOR_GENERATOR_HPP

#include <morbid/idl_compiler/module.hpp>
#include <morbid/idl_compiler/generator/type_spec.hpp>
#include <morbid/idl_compiler/generator/scoped_name.hpp>

#include <boost/spirit/home/karma.hpp>

#include <string>
#include <ostream>
#include <vector>

namespace morbid { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator, typename Iterator>
struct struct_generator_generator : karma::grammar
  <OutputIterator, idl_compiler::struct_def_type(struct_)>
{
  struct_generator_generator()
    : struct_generator_generator::base_type(start)
  {
    using phoenix::at_c;
    using karma::eol;
    using karma::string;
    using karma::_1; using karma::_r1;
    using karma::_val;
    
    namespace types = idl_parser::types;

    floating_point_generator = 
      (
       karma::eps(at_c<0>(_val) == types::floating_point::float_)
       << "float_(true/*little_endian*/)"
      )
      | (
       karma::eps(at_c<0>(_val) == types::floating_point::double_)
       << "double_(true/*little_endian*/)"
      )
      | (
       karma::eps(at_c<0>(_val) == types::floating_point::long_double_)
       << karma::eps(false) //"CORBA::LongDouble"
      )
      ;
    integer_generator =
      (
       karma::eps(at_c<0>(_val) == types::integer::signed_short_int)
       << "karma::attr_cast< ::CORBA::Short>(word(true/*little_endian*/))"
      )
      | (
       karma::eps(at_c<0>(_val) == types::integer::signed_long_int)
       << "karma::attr_cast< ::CORBA::Long>(dword(true/*little_endian*/))"
      )
      | (
       karma::eps(at_c<0>(_val) == types::integer::signed_longlong_int)
       << "karma::attr_cast< ::CORBA::LongLong>(qword(true/*little_endian*/))"
      )
      | (
       karma::eps(at_c<0>(_val) == types::integer::unsigned_short_int)
       << "karma::attr_cast< ::CORBA::UShort>(word(true/*little_endian*/))"
      )
      | (
       karma::eps(at_c<0>(_val) == types::integer::unsigned_long_int)
       << "karma::attr_cast< ::CORBA::ULong>(dword(true/*little_endian*/))"
      )
      | (
       karma::eps(at_c<0>(_val) == types::integer::unsigned_longlong_int)
       << "karma::attr_cast< ::CORBA::ULongLong>(qword(true/*little_endian*/))"
      )
      ;
    char_generator = karma::string[_1 = "karma::char_"];
    wchar_generator = karma::string[_1 = "karma::wchar_"];
    boolean_generator = karma::string[_1 = "karma::attr_cast< ::CORBA::Boolean>(karma::char_)"];
    octet_generator = karma::string[_1 = "karma::attr_cast< ::CORBA::Octet>(karma::char_)"];
    // any_generator = karma::eps;
    // object_generator = karma::eps;
    // value_base_generator = karma::eps;
    // void_generator = karma::eps;
    scoped_name_generator = karma::string[_1 = "scoped_name_generator"];//karma::eps;
    sequence_generator = karma::string[_1 = "sequence_generator"];//karma::eps;
    // any = karma::string[_1 = "CORBA::Any"];
    // object = karma::string[_1 = "CORBA::Object"];
    // value_base = karma::string[_1 = "CORBA::ValueBase"];

    member_generator =
      (floating_point_generator | integer_generator | char_generator | wchar_generator
       | boolean_generator | octet_generator
       | any_generator | object_generator | value_base_generator | void_generator
       | scoped_name_generator
       (
        at_c<1>(_r1)[at_c<0>(_val)]
       )
       | sequence_generator
       (
        at_c<1>(_r1)[at_c<0>(_val)]
       )
       ) [_1 = phoenix::at_c<0>(at_c<0>(_val))]
      ;    
    start =
      eol
      << "template <typename OutputIterator>" << eol
      << "struct _morbid_generator : ::boost::spirit::karma::grammar<OutputIterator" << eol
      << indent << ", " << karma::string[_1 = at_c<0>(_val)] << "()>" << eol
      << '{' << eol
      << indent << "_morbid_generator() : _morbid_generator::base_type(start)" << eol
      << indent << '{' << eol
      << indent << indent << "namespace karma = boost::spirit::karma;" << eol
      << indent << indent << "start %= " << eol
      << indent << indent << indent
      << (
          ((member_generator(_r1) << eol) % (indent << indent << indent << "<< "))[_1 = at_c<1>(_val)]
          | ("karma::eps" << eol)
         ) << indent << indent << indent << ';' << eol
      << indent << '}' << eol
      << indent << "::boost::spirit::karma::rule<OutputIterator" << eol
      << indent << indent << ", " << karma::string[_1 = at_c<0>(_val)] << "()> start;" << eol
      << indent << "::morbid::iiop::generator::float_<OutputIterator> float_;" << eol
      << indent << "::morbid::iiop::generator::double_<OutputIterator> double_;" << eol
      << indent << "::morbid::iiop::generator::word<OutputIterator> word;" << eol
      << indent << "::morbid::iiop::generator::dword<OutputIterator> dword;" << eol
      << indent << "::morbid::iiop::generator::qword<OutputIterator> qword;" << eol
      << "};" << eol << eol
      ;
    indent = karma::space << karma::space;
  }

  karma::rule<OutputIterator, idl_parser::types::scoped_name(lookuped_type_wrapper)> scoped_name_generator;
  karma::rule<OutputIterator, idl_parser::types::floating_point()> floating_point_generator;
  karma::rule<OutputIterator, idl_parser::types::integer()> integer_generator;
  karma::rule<OutputIterator, idl_parser::types::char_()> char_generator;
  karma::rule<OutputIterator, idl_parser::types::wchar_()> wchar_generator;
  karma::rule<OutputIterator, idl_parser::types::boolean()> boolean_generator;
  karma::rule<OutputIterator, idl_parser::types::octet()> octet_generator;
  karma::rule<OutputIterator, idl_parser::types::any()> any_generator;
  karma::rule<OutputIterator, idl_parser::types::object()> object_generator;
  karma::rule<OutputIterator, idl_parser::types::value_base()> value_base_generator;
  karma::rule<OutputIterator, idl_parser::types::void_()> void_generator;
  karma::rule<OutputIterator, idl_parser::types::sequence<Iterator>(lookuped_type_wrapper)> sequence_generator;

  karma::rule<OutputIterator, idl_parser::struct_member<Iterator>(struct_)> member_generator;
  karma::rule<OutputIterator> indent;
  karma::rule<OutputIterator, idl_compiler::struct_def_type(struct_)> start;
};

} } }

#endif
