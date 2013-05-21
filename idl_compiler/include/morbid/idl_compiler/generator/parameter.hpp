/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_PARAMETER_HPP
#define MORBID_IDL_COMPILER_PARAMETER_HPP

#include <morbid/idl_compiler/generator/scoped_name.hpp>
#include <morbid/idl_compiler/lookuped_type.hpp>
#include <morbid/idl_compiler/generator/type_spec.hpp>
#include <morbid/idl_parser/op_decl.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace morbid { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;
namespace phoenix = boost::phoenix;

template <typename OutputIterator>
struct parameter : karma::grammar<OutputIterator
                                  , idl_parser::param_decl(lookuped_type)>
{
  parameter()
    : parameter::base_type(start)
  {
    using karma::_1;
    using karma::_val;
    using karma::_r1; using karma::_r2;
    using phoenix::at_c;
    namespace types = idl_parser::types;

    in_traits = karma::string[_1 = "::morbid::in_traits< "];
    out_traits = karma::string[_1 = "::morbid::out_traits< "];
    inout_traits = karma::string[_1 = "::morbid::inout_traits< "];
    start %= 
      (out_traits | in_traits | inout_traits)
      << type_spec(_r1)
      << ">::type"
      ;

    start.name("parameter");
    karma::debug(start);
  }

  typedef boost::variant<idl_parser::direction::in, idl_parser::direction::out, idl_parser::direction::inout>
    direction_variant;

  generator::type_spec<OutputIterator> type_spec;
  karma::rule<OutputIterator, idl_parser::direction::in()> in_traits;
  karma::rule<OutputIterator, idl_parser::direction::out()> out_traits;
  karma::rule<OutputIterator, idl_parser::direction::inout()> inout_traits;
  karma::rule<OutputIterator, idl_parser::param_decl(lookuped_type)> start;
};


} } }

#endif
