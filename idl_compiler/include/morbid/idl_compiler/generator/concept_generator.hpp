/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_CONCEPT_GENERATOR_HPP
#define MORBID_IDL_COMPILER_CONCEPT_GENERATOR_HPP

#include <morbid/idl_parser/interface_def.hpp>
#include <morbid/idl_compiler/generator/empty_reference_generator.hpp>
#include <morbid/idl_compiler/generator/proxy_reference_generator.hpp>
#include <morbid/idl_compiler/generator/parameter.hpp>
#include <morbid/idl_compiler/generator/return.hpp>
#include <morbid/idl_compiler/generator/type_spec.hpp>
#include <morbid/idl_compiler/interface.hpp>

#include <boost/spirit/home/karma.hpp>

#include <string>
#include <ostream>
#include <vector>

namespace morbid { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator, typename Iterator>
struct header_concept_generator : karma::grammar
  <OutputIterator, idl_parser::interface_def(interface_, std::vector<idl_parser::wave_string>)
   , karma::locals<idl_parser::wave_string> >
{
  header_concept_generator();

  karma::rule<OutputIterator, idl_parser::wave_string()> wave_string;
  idl_compiler::generator::parameter<OutputIterator, Iterator> parameter;
  idl_compiler::generator::type_spec<OutputIterator, Iterator> type_spec;
  idl_compiler::generator::return_<OutputIterator, Iterator> return_;
  idl_compiler::generator::empty_reference_generator<OutputIterator, Iterator> empty_reference;
  idl_compiler::generator::proxy_reference_generator<OutputIterator, Iterator> proxy_reference;
  karma::rule<OutputIterator, idl_parser::direction::in()> in_tag;
  karma::rule<OutputIterator, idl_parser::direction::out()> out_tag;
  karma::rule<OutputIterator, idl_parser::direction::inout()> inout_tag;
  karma::rule<OutputIterator, idl_parser::param_decl(interface_)>
    parameter_select;
  karma::rule<OutputIterator> indent;
  karma::rule<OutputIterator, idl_parser::wave_string(std::vector<idl_parser::wave_string>, idl_parser::wave_string)> public_members;
  karma::rule<OutputIterator
              , idl_parser::op_decl(interface_, std::vector<idl_parser::wave_string>, idl_parser::wave_string)
              , karma::locals<unsigned int> > operation_concept_interface_specialization;
  karma::rule<OutputIterator
              , idl_parser::param_decl(interface_)> arguments;
  karma::rule<OutputIterator
              , idl_parser::op_decl(interface_)
              , karma::locals<unsigned int> > operation;
  karma::rule<OutputIterator, idl_parser::param_decl(unsigned int)> args;
  karma::rule<OutputIterator
              , idl_parser::op_decl()> operation_name;
  karma::rule<OutputIterator
              , std::vector<idl_parser::op_decl >()> requirements;
  karma::rule<OutputIterator
              , idl_parser::interface_def(interface_, std::vector<idl_parser::wave_string>)
              , karma::locals<idl_parser::wave_string> > start;
  karma::rule<OutputIterator
              , idl_parser::interface_def(interface_, std::vector<idl_parser::wave_string>)
              , karma::locals<idl_parser::wave_string> > concept_class;
};

} } }

#endif
