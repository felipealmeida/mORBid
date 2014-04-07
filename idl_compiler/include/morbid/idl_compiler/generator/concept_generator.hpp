/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_CONCEPT_GENERATOR_HPP
#define MORBID_IDL_COMPILER_CONCEPT_GENERATOR_HPP

#include <morbid/idl_parser/interface_def.hpp>
#include <morbid/idl_compiler/generator/proxy_reference_generator.hpp>
#include <morbid/idl_compiler/generator/parameter.hpp>
#include <morbid/idl_compiler/generator/return.hpp>
#include <morbid/idl_compiler/generator/type_spec.hpp>
#include <morbid/idl_compiler/interface.hpp>

//#include <boost/spirit/home/karma/nonterminal.hpp>
#include <boost/spirit/home/karma.hpp>

#include <string>
#include <ostream>
#include <vector>

namespace morbid { namespace idlc { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
struct header_concept_generator : karma::grammar
  <OutputIterator, idl_parser::interface_def(interface_, std::vector<idl_parser::wave_string>)
   , karma::locals<idl_parser::wave_string> >
{
  header_concept_generator();

  // karma::rule<OutputIterator, idl_parser::wave_string()> wave_string;
  idlc::generator::parameter<OutputIterator> parameter;
  idlc::generator::type_spec<OutputIterator> type_spec;
  idlc::generator::return_<OutputIterator> return_;
  // // idlc::generator::proxy_reference_generator<OutputIterator> proxy_reference;
  karma::rule<OutputIterator, idl_parser::direction::in()> in_tag;
  karma::rule<OutputIterator, idl_parser::direction::out()> out_tag;
  karma::rule<OutputIterator, idl_parser::direction::inout()> inout_tag;
  karma::rule<OutputIterator, idl_parser::param_decl(interface_)>
    parameter_select;
  karma::rule<OutputIterator> indent;
  karma::rule<OutputIterator, idl_parser::wave_string(std::vector<idl_parser::wave_string>, idl_parser::wave_string)> public_members;
  karma::rule<OutputIterator
              , idl_parser::param_decl(interface_)> arguments;
  karma::rule<OutputIterator
              , idl_parser::types::scoped_name(interface_)> base_spec;
  karma::rule<OutputIterator
              , idl_parser::op_decl(interface_)
              , karma::locals<unsigned int> > operation;
  karma::rule<OutputIterator
              , idl_parser::attribute(interface_)
              , karma::locals<unsigned int> > attribute;
  karma::rule<OutputIterator, idl_parser::param_decl(unsigned int)> args;
  karma::rule<OutputIterator
              , idl_parser::op_decl()> operation_name;
  karma::rule<OutputIterator
              , idl_parser::attribute()> attribute_name;
  karma::rule<OutputIterator
              , idl_parser::interface_def()> requirements;
  karma::rule<OutputIterator
              , idl_parser::interface_def(interface_, std::vector<idl_parser::wave_string>)
              , karma::locals<idl_parser::wave_string> > start;
  karma::rule<OutputIterator
              , idl_parser::interface_def(interface_, std::vector<idl_parser::wave_string>)
              , karma::locals<idl_parser::wave_string> > concept_class;
  karma::rule<OutputIterator, idl_parser::param_decl(unsigned int)> template_parameter;
};

} } }

#endif
