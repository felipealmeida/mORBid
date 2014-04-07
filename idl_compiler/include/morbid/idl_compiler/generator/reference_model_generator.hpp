/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_REFERENCE_MODEL_GENERATOR_HPP
#define MORBID_IDL_COMPILER_REFERENCE_MODEL_GENERATOR_HPP

#include <morbid/idl_parser/interface_def.hpp>
#include <morbid/idl_compiler/interface.hpp>
#include <morbid/idl_compiler/generator/parameter.hpp>
#include <morbid/idl_compiler/generator/return.hpp>
#include <morbid/idl_compiler/generator/type_spec.hpp>

#include <boost/spirit/home/karma.hpp>

namespace morbid { namespace idlc { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
struct header_reference_model_generator : karma::grammar
<OutputIterator, idl_parser::interface_def(interface_, std::vector<idl_parser::wave_string>)>
{
  header_reference_model_generator();

  idlc::generator::parameter<OutputIterator> parameter;
  idlc::generator::return_<OutputIterator> return_traits;
  idlc::generator::type_spec<OutputIterator> type_spec;
  karma::rule<OutputIterator, idl_parser::type_spec(lookuped_type, std::string)> return_;
  karma::rule<OutputIterator, idl_parser::wave_string()>
    class_name;
  karma::rule<OutputIterator, idl_parser::param_decl(interface_)>
    parameter_select;
  karma::rule<OutputIterator, idl_parser::type_spec(interface_)>
    type_spec_select;
  karma::rule<OutputIterator
              , idl_parser::types::scoped_name(interface_)> base_spec;
  karma::rule<OutputIterator> ior_function;
  karma::rule<OutputIterator> indent;
  karma::rule<OutputIterator> common_members;
  karma::rule<OutputIterator
              , idl_parser::interface_def(interface_)> common_functions;
  karma::rule<OutputIterator
              , idl_parser::op_decl(interface_, std::vector<idl_parser::wave_string>, idl_parser::wave_string)
              , karma::locals<unsigned int> > 
    operation;
  karma::rule<OutputIterator
              , idl_parser::attribute(interface_, std::vector<idl_parser::wave_string>, idl_parser::wave_string)
              , karma::locals<unsigned int> > 
    attribute;
  karma::rule<OutputIterator
              , idl_parser::interface_def(interface_, std::vector<idl_parser::wave_string>)> start;
  // karma::rule<OutputIterator
  //             , idl_parser::interface_def
  //             (std::vector<idl_parser::wave_string>, std::vector<idl_parser::wave_string>)>
  //   specialization;
  karma::rule<OutputIterator, idl_parser::param_decl(interface_)>
    synchronous_template_args;
  karma::rule<OutputIterator, idl_parser::param_decl(interface_, unsigned int)> synchronous_args;
  karma::rule<OutputIterator, idl_parser::direction::in()> in_tag;
  karma::rule<OutputIterator, idl_parser::direction::out()> out_tag;
  karma::rule<OutputIterator, idl_parser::direction::inout()> inout_tag;
};

} } }

#endif
