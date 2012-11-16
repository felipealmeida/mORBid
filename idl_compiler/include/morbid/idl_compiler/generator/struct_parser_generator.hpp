/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_STRUCT_PARSER_GENERATOR_HPP
#define TECORB_IDL_COMPILER_STRUCT_PARSER_GENERATOR_HPP

#include <morbid/idl_compiler/module.hpp>
#include <morbid/idl_compiler/generator/type_spec.hpp>
#include <morbid/idl_compiler/generator/scoped_name.hpp>

#include <boost/spirit/home/karma.hpp>

#include <string>
#include <ostream>
#include <vector>

namespace std {

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, std::pair<T, U> p)
{
  return os << "[pair first: " << p.first << " second: " << p.second << ']';
}

}

namespace morbid { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator, typename Iterator>
struct struct_parser_generator : karma::grammar
  <OutputIterator, idl_compiler::struct_def_type(struct_)>
{
  struct_parser_generator()
    : struct_parser_generator::base_type(start)
  {
    using phoenix::at_c;
    using karma::eol;
    using karma::string;
    using karma::_1; using karma::_r1;
    using karma::_val;
    namespace types = idl_parser::types;

    floating_point_parser = 
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
    integer_parser =
      (
       karma::eps(at_c<0>(_val) == types::integer::signed_short_int)
       << "qi::as< ::CORBA::Short>()[ word(true/*little_endian*/)]"
      )
      | (
       karma::eps(at_c<0>(_val) == types::integer::signed_long_int)
       << "qi::as< ::CORBA::Long>()[ dword(true/*little_endian*/)]"
      )
      | (
       karma::eps(at_c<0>(_val) == types::integer::signed_longlong_int)
       << "qi::as< ::CORBA::LongLong>()[ qword(true/*little_endian*/)]"
      )
      | (
       karma::eps(at_c<0>(_val) == types::integer::unsigned_short_int)
       << "qi::as< ::CORBA::UShort>()[ word(true/*little_endian*/)]"
      )
      | (
       karma::eps(at_c<0>(_val) == types::integer::unsigned_long_int)
       << "qi::as< ::CORBA::ULong>()[ dword(true/*little_endian*/)]"
      )
      | (
       karma::eps(at_c<0>(_val) == types::integer::unsigned_longlong_int)
       << "qi::as< ::CORBA::ULongLong>()[ qword(true/*little_endian*/)]"
      )
      ;
    char_parser = karma::string[_1 = "qi::char_"];
    wchar_parser = karma::string[_1 = "qi::wchar_"];
    boolean_parser = karma::string[_1 = "qi::as< ::morbid::Boolean>()[qi::char_]"];
    octet_parser = karma::string[_1 = "qi::as< ::morbid::Octet>()[qi::char_]"];
    // any_parser = karma::eps;
    // object_parser = karma::eps;
    // value_base_parser = karma::eps;
    // void_parser = karma::eps;
    scoped_name_parser = karma::string[_1 = "scoped_name_parser"];//karma::eps;
    sequence_parser = karma::string[_1 = "sequence_parser"];//karma::eps;
    // any = karma::string[_1 = "CORBA::Any"];
    // object = karma::string[_1 = "CORBA::Object"];
    // value_base = karma::string[_1 = "CORBA::ValueBase"];

    member_parser =
      (floating_point_parser | integer_parser | char_parser | wchar_parser
       | boolean_parser | octet_parser
       | any_parser | object_parser | value_base_parser | void_parser
       | scoped_name_parser
       (
        at_c<1>(_r1)[at_c<0>(_val)]
       )
       | sequence_parser
       (
        at_c<1>(_r1)[at_c<0>(_val)]
       )
       ) [_1 = phoenix::at_c<0>(at_c<0>(_val))]
      ;    
    start =
      eol
      << "template <typename Iterator>" << eol
      << "struct _morbid_parser : ::boost::spirit::qi::grammar<Iterator" << eol
      << indent << ", " << karma::string[_1 = at_c<0>(_val)] << "()>" << eol
      << '{' << eol
      << indent << "_morbid_parser() : _morbid_parser::base_type(start)" << eol
      << indent << '{' << eol
      << indent << indent << "namespace qi = boost::spirit::qi;" << eol
      << indent << indent << "start %= " << eol
      << indent << indent << indent
      << (
          ((member_parser(_r1) << eol) % (indent << indent << indent << ">> "))[_1 = at_c<1>(_val)]
          | ("qi::eps" << eol)
         ) << indent << indent << indent << ';' << eol
      << indent << '}' << eol
      << indent << "::boost::spirit::qi::rule<Iterator" << eol
      << indent << indent << ", " << karma::string[_1 = at_c<0>(_val)] << "()> start;" << eol
      << indent << "::morbid::iiop::grammar::float_<Iterator> float_;" << eol
      << indent << "::morbid::iiop::grammar::double_<Iterator> double_;" << eol
      << indent << "::morbid::iiop::grammar::word<Iterator> word;" << eol
      << indent << "::morbid::iiop::grammar::dword<Iterator> dword;" << eol
      << indent << "::morbid::iiop::grammar::qword<Iterator> qword;" << eol
      << "};" << eol << eol
      ;
    indent = karma::space << karma::space;

    start.name("struct_parser_generator");
    karma::debug(start);
    member_parser.name("member_parser");
    karma::debug(member_parser);
    // scoped_name_parser.name("scoped_name_parser");
    // karma::debug(scoped_name_parser);
    // sequence_parser.name("sequence_parser");
    // karma::debug(sequence_parser);
    floating_point_parser.name("floating_point_parser");
    karma::debug(floating_point_parser);
    integer_parser.name("integer_parser");
    karma::debug(integer_parser);
  }

  karma::rule<OutputIterator, idl_parser::types::scoped_name(lookuped_type_wrapper)> scoped_name_parser;
  karma::rule<OutputIterator, idl_parser::types::floating_point()> floating_point_parser;
  karma::rule<OutputIterator, idl_parser::types::integer()> integer_parser;
  karma::rule<OutputIterator, idl_parser::types::char_()> char_parser;
  karma::rule<OutputIterator, idl_parser::types::wchar_()> wchar_parser;
  karma::rule<OutputIterator, idl_parser::types::boolean()> boolean_parser;
  karma::rule<OutputIterator, idl_parser::types::octet()> octet_parser;
  karma::rule<OutputIterator, idl_parser::types::any()> any_parser;
  karma::rule<OutputIterator, idl_parser::types::object()> object_parser;
  karma::rule<OutputIterator, idl_parser::types::value_base()> value_base_parser;
  karma::rule<OutputIterator, idl_parser::types::void_()> void_parser;
  karma::rule<OutputIterator, idl_parser::types::sequence<Iterator>(lookuped_type_wrapper)> sequence_parser;

  karma::rule<OutputIterator, idl_parser::struct_member<Iterator>(struct_)> member_parser;
  karma::rule<OutputIterator> indent;
  karma::rule<OutputIterator, idl_compiler::struct_def_type(struct_)> start;
};

} } }

#endif
