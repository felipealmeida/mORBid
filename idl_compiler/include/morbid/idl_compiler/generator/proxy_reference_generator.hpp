/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_PROXY_REFERENCE_GENERATOR_HPP
#define MORBID_IDL_COMPILER_PROXY_REFERENCE_GENERATOR_HPP

#include <morbid/idl_parser/interface_def.hpp>
#include <morbid/idl_compiler/generator/parameter.hpp>
#include <morbid/idl_compiler/generator/return.hpp>
#include <morbid/idl_compiler/generator/type_spec.hpp>
#include <morbid/idl_compiler/interface.hpp>

#include <boost/spirit/home/karma.hpp>

namespace morbid { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
struct proxy_reference_generator : karma::grammar
<OutputIterator, idl_parser::interface_def(interface_)>
{
  proxy_reference_generator();

  karma::rule<OutputIterator, idl_parser::wave_string()> wave_string;
  idl_compiler::generator::parameter<OutputIterator> parameter;
  idl_compiler::generator::return_<OutputIterator> return_;
  karma::rule<OutputIterator, idl_parser::param_decl(interface_)>
    parameter_select;
  karma::rule<OutputIterator> indent;
  karma::rule<OutputIterator, idl_parser::wave_string(bool)> class_name;
  karma::rule<OutputIterator
              , idl_parser::op_decl(interface_)
              , karma::locals<unsigned int> > operation;
  karma::rule<OutputIterator, idl_parser::param_decl(unsigned int)> synchronous_args;
  karma::rule<OutputIterator
              , idl_parser::interface_def(interface_)> start;
};

} } }

#endif
