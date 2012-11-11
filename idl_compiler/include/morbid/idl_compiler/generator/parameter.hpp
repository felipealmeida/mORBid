/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_PARAMETER_HPP
#define TECORB_IDL_COMPILER_PARAMETER_HPP

#include <morbid/idl_compiler/generator/scoped_name.hpp>
#include <morbid/idl_compiler/lookuped_type.hpp>
#include <morbid/idl_parser/op_decl.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace morbid { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;
namespace phoenix = boost::phoenix;

template <typename OutputIterator, typename Iterator>
struct parameter : karma::grammar<OutputIterator
                                  , idl_parser::param_decl<Iterator>(lookuped_type)>
{
  parameter()
    : parameter::base_type(start)
  {
    using karma::_1;
    using karma::_val;
    using karma::_r1; using karma::_r2;
    using phoenix::at_c;
    namespace types = idl_parser::types;

    in_traits = "::morbid::in_traits< ";
    out_traits = "::morbid::out_traits< ";
    inout_traits = "::morbid::inout_traits< ";
    start = 
      param(at_c<0>(_val), _r1)[_1 = at_c<1>(_val)];
    param = 
      (in_traits[_1 = _r1]
       | out_traits[_1 = _r1]
       | inout_traits[_1 = _r1])
      <<
      (floating_point | integer | char_ | wchar_ | boolean | octet
       | any | object | value_base | void_ | scoped_name(_r1, _r2)
       | sequence) [_1 = at_c<0>(_val)]
      << ">::type"
      ;
    floating_point =
      (
       karma::eps(at_c<0>(_val) == types::floating_point::float_)
       << "CORBA::Float"
      )
      | (
       karma::eps(at_c<0>(_val) == types::floating_point::double_)
       << "CORBA::Double"
      )
      | (
       karma::eps(at_c<0>(_val) == types::floating_point::long_double_)
       << "CORBA::LongDouble"
      )
      ;
    integer =
      (
       karma::eps(at_c<0>(_val) == types::integer::signed_short_int)
       << "CORBA::Short"
      )
      | (
       karma::eps(at_c<0>(_val) == types::integer::signed_long_int)
       << "CORBA::Long"
      )
      | (
       karma::eps(at_c<0>(_val) == types::integer::signed_longlong_int)
       << "CORBA::LongLong"
      )
      | (
       karma::eps(at_c<0>(_val) == types::integer::unsigned_short_int)
       << "CORBA::UShort"
      )
      | (
       karma::eps(at_c<0>(_val) == types::integer::unsigned_long_int)
       << "CORBA::ULong"
      )
      | (
       karma::eps(at_c<0>(_val) == types::integer::unsigned_longlong_int)
       << "CORBA::ULongLong"
      )
      ;
    char_ = karma::string[_1 = "CORBA::Char"];
    wchar_ = karma::string[_1 = "CORBA::WChar"];
    boolean = karma::string[_1 = "CORBA::Boolean"];
    octet = karma::string[_1 = "CORBA::Octet"];
    any = karma::string[_1 = "CORBA::Any"];
    object = karma::string[_1 = "CORBA::Object"];
    value_base = karma::string[_1 = "CORBA::ValueBase"];
    void_ = karma::string[_1 = "void"];
    scoped_name =
      (karma::string % "::")[_1 = at_c<0>(_r2)]
      << "::" << (karma::string % "::")
      [_1 = at_c<1>(_val)]
      ;

    start.name("parameter");
    karma::debug(start);
    param.name("param");
    karma::debug(param);
  }

  typedef boost::variant<idl_parser::direction::in, idl_parser::direction::out, idl_parser::direction::inout>
    direction_variant;

  karma::rule<OutputIterator, idl_parser::direction::in> in_traits;
  karma::rule<OutputIterator, idl_parser::direction::out> out_traits;
  karma::rule<OutputIterator, idl_parser::direction::inout> inout_traits;
  karma::rule<OutputIterator, idl_parser::types::scoped_name(direction_variant, lookuped_type_wrapper)> scoped_name;
  karma::rule<OutputIterator, idl_parser::param_decl<Iterator>(lookuped_type)> start;
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
  karma::rule<OutputIterator, idl_parser::types::sequence<Iterator>()> sequence;
  karma::rule<OutputIterator, idl_parser::type_spec<Iterator>
              (boost::variant<idl_parser::direction::in
               , idl_parser::direction::out, idl_parser::direction::inout>
               , lookuped_type)>
  param;
  
};


} } }

#endif
