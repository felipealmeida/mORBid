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

#include <boost/spirit/home/karma/nonterminal.hpp>

namespace morbid { namespace idlc { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
struct parameter : karma::grammar<OutputIterator
                                  , idl_parser::param_decl(lookuped_type)>
{
  parameter();

  generator::type_spec<OutputIterator> type_spec;
  karma::rule<OutputIterator, idl_parser::direction::in()> in_traits;
  karma::rule<OutputIterator, idl_parser::direction::out()> out_traits;
  karma::rule<OutputIterator, idl_parser::direction::inout()> inout_traits;
  karma::rule<OutputIterator, idl_parser::param_decl(lookuped_type)> start;
};


} } }

#endif
