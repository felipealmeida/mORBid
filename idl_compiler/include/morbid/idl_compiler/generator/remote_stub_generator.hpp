/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_REMOTE_STUB_GENERATOR_HPP
#define TECORB_IDL_COMPILER_REMOTE_STUB_GENERATOR_HPP

#include <morbid/idl_parser/interface_def.hpp>
#include <morbid/idl_compiler/generator/parameter.hpp>

#include <boost/spirit/home/karma.hpp>

namespace morbid { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator, typename Iterator>
struct header_remote_stub_generator : karma::grammar
<OutputIterator, idl_parser::interface_def<Iterator>(), karma::locals<std::string> >
{
  header_remote_stub_generator();

  idl_compiler::parameter<OutputIterator, Iterator> parameter;
  karma::rule<OutputIterator> ior_function;
  karma::rule<OutputIterator> indent;
  karma::rule<OutputIterator, std::string()> common_members;
  karma::rule<OutputIterator
              , idl_parser::interface_def<Iterator>()> common_functions;
  karma::rule<OutputIterator
              , idl_parser::op_decl<Iterator>()> operation;
  karma::rule<OutputIterator
              , idl_parser::interface_def<Iterator>()
              , karma::locals<std::string> > start;
};

template <typename OutputIterator, typename Iterator>
struct cpp_remote_stub_generator : karma::grammar
<OutputIterator, idl_parser::interface_def<Iterator>(), karma::locals<std::string> >
{
  cpp_remote_stub_generator();

  idl_compiler::parameter<OutputIterator, Iterator> parameter;
  karma::rule<OutputIterator, idl_parser::param_decl<Iterator>()> synchronous_template_args;
  karma::rule<OutputIterator, idl_parser::param_decl<Iterator>()
              , karma::locals<unsigned int> > synchronous_args;
  karma::rule<OutputIterator, std::string()> ior_function;
  karma::rule<OutputIterator> indent;
  karma::rule<OutputIterator
              , idl_parser::op_decl<Iterator>(std::string)
              , karma::locals<unsigned int> > operation;
  karma::rule<OutputIterator
              , idl_parser::interface_def<Iterator>()
              , karma::locals<std::string> > start;
};

} } }

#endif
