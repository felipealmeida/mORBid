/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_TYPE_SPEC_HPP
#define MORBID_IDL_COMPILER_TYPE_SPEC_HPP

// #include <morbid/idl_compiler/common_types.hpp>
#include <morbid/idl_compiler/lookuped_type.hpp>
// // #include <morbid/idl_compiler/module.hpp>
#include <morbid/idl_compiler/generator/scoped_name.hpp>
#include <morbid/idl_parser/type_spec.hpp>

#include <boost/spirit/home/karma/nonterminal.hpp>

namespace morbid { namespace idlc { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
struct type_spec : karma::grammar<OutputIterator, idl_parser::type_spec
                                  (lookuped_type)>
{
  type_spec();

  generator::scoped_name<OutputIterator> scoped_name;
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
  karma::rule<OutputIterator, idl_parser::types::sequence(lookuped_type)> sequence;
  karma::rule<OutputIterator, idl_parser::types::sequence()> fail_sequence;
};

} } }

#endif
