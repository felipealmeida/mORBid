/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_REFERENCE_MODEL_GENERATOR_HPP
#define MORBID_IDL_COMPILER_REFERENCE_MODEL_GENERATOR_HPP

#include <morbid/idl_parser/interface_def.hpp>
#include <morbid/idl_compiler/generator/parameter.hpp>
#include <morbid/idl_compiler/generator/return.hpp>
#include <morbid/idl_compiler/generator/type_spec.hpp>

#include <boost/spirit/home/karma.hpp>

namespace morbid { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator, typename Iterator>
struct header_reference_model_generator : karma::grammar
<OutputIterator, idl_parser::interface_def<Iterator>(interface_, std::vector<std::string>)
 , karma::locals<std::string> >
{
  header_reference_model_generator();

  idl_compiler::generator::parameter<OutputIterator, Iterator> parameter;
  idl_compiler::generator::return_<OutputIterator, Iterator> return_;
  idl_compiler::generator::type_spec<OutputIterator, Iterator> type_spec;
  karma::rule<OutputIterator, idl_parser::interface_def<Iterator>()>
    class_name;
  karma::rule<OutputIterator, idl_parser::param_decl<Iterator>(interface_)>
    parameter_select;
  karma::rule<OutputIterator, idl_parser::type_spec<Iterator>(interface_)>
    type_spec_select;
  karma::rule<OutputIterator> ior_function;
  karma::rule<OutputIterator> indent;
  karma::rule<OutputIterator, std::string()> common_members;
  karma::rule<OutputIterator
              , idl_parser::interface_def<Iterator>()> common_functions;
  karma::rule<OutputIterator
              , idl_parser::op_decl<Iterator>(interface_, std::vector<std::string>)
              , karma::locals<unsigned int> > 
    operation;
  karma::rule<OutputIterator
              , idl_parser::interface_def<Iterator>(interface_, std::vector<std::string>)
              , karma::locals<std::string> > start;
  karma::rule<OutputIterator, idl_parser::param_decl<Iterator>(interface_)>
    synchronous_template_args;
  karma::rule<OutputIterator, idl_parser::param_decl<Iterator>(interface_, unsigned int)> synchronous_args;
  karma::rule<OutputIterator, idl_parser::direction::in()> in_tag;
  karma::rule<OutputIterator, idl_parser::direction::out()> out_tag;
  karma::rule<OutputIterator, idl_parser::direction::inout()> inout_tag;
};

} } }

#endif
