/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_TYPE_SPEC_HPP
#define TECORB_IDL_COMPILER_TYPE_SPEC_HPP

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
struct type_spec : karma::grammar<OutputIterator, idl_parser::type_spec<Iterator>
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
    char_ = karma::string[_1 = "CORBA::Char"];
    wchar_ = karma::string[_1 = "CORBA::WChar"];
    boolean = karma::string[_1 = "CORBA::Boolean"];
    octet = karma::string[_1 = "CORBA::Octet"];
    any = karma::string[_1 = "CORBA::Any"];
    object = karma::string[_1 = "CORBA::Object"];
    value_base = karma::string[_1 = "CORBA::ValueBase"];
    void_ = karma::string[_1 = "void"];
    sequence =
      karma::lit("::morbid::sequence<")
      << ">";
  }

  generator::scoped_name<OutputIterator, Iterator> scoped_name;
  karma::rule<OutputIterator, idl_parser::type_spec<Iterator>(lookuped_type)> start;
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
  karma::rule<OutputIterator, idl_parser::types::sequence<Iterator>(lookuped_type_wrapper)> sequence;
  
};

} } }

#endif
