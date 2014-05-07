/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_REFERENCE_MODEL_GENERATOR_HPP
#define MORBID_IDL_COMPILER_REFERENCE_MODEL_GENERATOR_HPP

#include <morbid/idl_compiler/interface.hpp>
#include <morbid/idl_compiler/generator/type_spec.hpp>
#include <morbid/idl_compiler/generator/indent.hpp>
#include <morbid/idl_compiler/generator/operation.hpp>
#include <morbid/idl_compiler/generator/attribute.hpp>
#include <morbid/idl_parser/interface_def.hpp>

#include <boost/spirit/home/karma.hpp>

namespace morbid { namespace idlc { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
struct header_reference_model_generator : karma::grammar
<OutputIterator, idl_parser::interface_def(interface_, std::vector<idl_parser::wave_string>)>
{
  header_reference_model_generator();

  idlc::generator::type_spec<OutputIterator> type_spec;
  idlc::generator::indent<OutputIterator> indent;
  idlc::generator::operation<OutputIterator> operation;
  idlc::generator::attribute<OutputIterator> attribute;
  
  karma::rule<OutputIterator, idl_parser::wave_string()>
    class_name;
  karma::rule<OutputIterator
              , idl_parser::types::scoped_name(interface_)> base_spec;
  karma::rule<OutputIterator> ior_function;
  karma::rule<OutputIterator> common_members;
  karma::rule<OutputIterator
              , idl_parser::interface_def(interface_)> common_functions;
  karma::rule<OutputIterator
              , idl_parser::interface_def(interface_, std::vector<idl_parser::wave_string>)> start;
  // karma::rule<OutputIterator
  //             , idl_parser::interface_def
  //             (std::vector<idl_parser::wave_string>, std::vector<idl_parser::wave_string>)>
  //   specialization;
};

} } }

#endif
