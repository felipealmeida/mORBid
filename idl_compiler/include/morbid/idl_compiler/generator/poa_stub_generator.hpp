/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_POA_STUB_GENERATOR_HPP
#define TECORB_IDL_COMPILER_POA_STUB_GENERATOR_HPP

#include <morbid/idl_parser/interface_def.hpp>
#include <morbid/idl_compiler/generator/parameter.hpp>

#include <boost/spirit/home/karma.hpp>

namespace morbid { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator, typename Iterator>
struct header_poa_stub_generator : karma::grammar
<OutputIterator, idl_parser::interface_def<Iterator>(), karma::locals<std::string> >
{
  header_poa_stub_generator();

  idl_compiler::parameter<OutputIterator> parameter;
  karma::rule<OutputIterator, std::string()> construct_local_stub_function;
  karma::rule<OutputIterator> indent;
  karma::rule<OutputIterator> dispatch_function;
  karma::rule<OutputIterator
              , idl_parser::interface_def<Iterator>()> common_functions;
  karma::rule<OutputIterator
              , idl_parser::op_decl<Iterator>()> operation;
  karma::rule<OutputIterator
              , idl_parser::interface_def<Iterator>()
              , karma::locals<std::string> > start;
};

template <typename OutputIterator, typename Iterator>
struct cpp_poa_stub_generator : karma::grammar
<OutputIterator, idl_parser::interface_def<Iterator>(), karma::locals<std::string> >
{
  cpp_poa_stub_generator();

  idl_compiler::parameter<OutputIterator> parameter;
  karma::rule<OutputIterator, std::string()> is_a_impl_strcmp;
  karma::rule<OutputIterator, void(unsigned int)> var_indent;
  karma::rule<OutputIterator, idl_parser::op_decl<Iterator>
              (unsigned int, idl_parser::interface_def<Iterator>)>
    non_user_defined_implementation, is_a_impl;
  karma::rule<OutputIterator, idl_parser::op_decl<Iterator>
              (idl_parser::interface_def<Iterator>)
              , karma::locals<unsigned int> > non_user_defined_operations;
  karma::rule<OutputIterator, void(std::string)> class_name;
  karma::rule<OutputIterator, idl_parser::op_decl<Iterator>(unsigned int, std::string)> dispatching_if;
  karma::rule<OutputIterator> indent;
  karma::rule<OutputIterator, idl_parser::interface_def<Iterator>()
              , karma::locals<std::string> > dispatch_function;
  karma::rule<OutputIterator
              , idl_parser::interface_def<Iterator>()
              , karma::locals<std::string> > start;
};

} } }

#endif
