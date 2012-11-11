/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_STUB_GENERATOR_HPP
#define TECORB_IDL_COMPILER_STUB_GENERATOR_HPP

#include <morbid/idl_parser/interface_def.hpp>
#include <morbid/idl_compiler/generator/parameter.hpp>
#include <morbid/idl_compiler/generator/type_spec.hpp>
#include <morbid/idl_compiler/interface.hpp>

#include <boost/spirit/home/karma.hpp>

#include <string>
#include <ostream>
#include <vector>

namespace morbid { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator, typename Iterator>
struct header_stub_generator : karma::grammar
  <OutputIterator, idl_compiler::interface_def_type(interface_)
   , karma::locals<std::string> >
{
  header_stub_generator();

  idl_compiler::generator::parameter<OutputIterator, Iterator> parameter;
  idl_compiler::generator::type_spec<OutputIterator, Iterator> type_spec;
  karma::rule<OutputIterator, idl_parser::param_decl<Iterator>(interface_)>
    parameter_select;
  karma::rule<OutputIterator> indent, public_members;
  karma::rule<OutputIterator, std::string()> typedefs;
  karma::rule<OutputIterator
              , idl_parser::interface_def<Iterator>()> common_functions;
  karma::rule<OutputIterator
              , idl_parser::op_decl<Iterator>(interface_)> operation;
  karma::rule<OutputIterator
              , idl_compiler::interface_def_type(interface_)
              , karma::locals<std::string> > start;
};

template <typename OutputIterator, typename Iterator>
struct cpp_stub_generator : karma::grammar
<OutputIterator, idl_parser::interface_def<Iterator>(interface_)>
{
  cpp_stub_generator();

  idl_compiler::generator::parameter<OutputIterator, Iterator> parameter;
  karma::rule<OutputIterator> indent;
  karma::rule<OutputIterator, std::string()> members;
  karma::rule<OutputIterator, std::string()> construct_remote_stub;
  karma::rule<OutputIterator
              , idl_parser::interface_def<Iterator>(interface_)> start;
};

} } }

#endif
