/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_TYPEDEF_GENERATOR_HPP
#define MORBID_IDL_COMPILER_TYPEDEF_GENERATOR_HPP

#include <morbid/idl_compiler/generator/type_spec.hpp>

#include <morbid/idl_compiler/lookuped_type.hpp>
#include <morbid/idl_parser/typedef_def.hpp>

#include <boost/spirit/home/karma/nonterminal.hpp>

namespace morbid { namespace idlc { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator, typename Iterator>
struct typedef_generator : karma::grammar
  <OutputIterator, idl_parser::typedef_def(lookuped_type)>
{
  typedef_generator();

  generator::type_spec<OutputIterator> type_spec;
  karma::rule<OutputIterator, idl_parser::typedef_def(lookuped_type)> start;
};

} } }

#endif
