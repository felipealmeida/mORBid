/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_LOCAL_STUB_GENERATOR_HPP
#define MORBID_IDL_COMPILER_LOCAL_STUB_GENERATOR_HPP

#include <morbid/idl_parser/interface_def.hpp>
#include <morbid/idl_compiler/generator/parameter.hpp>
#include <morbid/idl_compiler/generator/return.hpp>

#include <boost/spirit/home/karma.hpp>

namespace morbid { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator, typename Iterator>
struct header_local_stub_generator : karma::grammar
<OutputIterator, idl_parser::interface_def<Iterator>
 (interface_, std::vector<std::string>)
 , karma::locals<std::string> >
{
  header_local_stub_generator();

  idl_compiler::generator::parameter<OutputIterator, Iterator> parameter;
  idl_compiler::generator::return_<OutputIterator, Iterator> return_;
  karma::rule<OutputIterator, idl_parser::param_decl<Iterator>(interface_)>
    parameter_select;
  karma::rule<OutputIterator, std::string(std::vector<std::string>)>
    poa_class_name;
  karma::rule<OutputIterator> ior_function;
  karma::rule<OutputIterator> indent;
  karma::rule<OutputIterator, std::string(std::vector<std::string>)>
    common_members;
  karma::rule<OutputIterator
              , idl_parser::interface_def<Iterator>(std::vector<std::string>)>
    common_functions;
  karma::rule<OutputIterator
              , idl_parser::op_decl<Iterator>(interface_)
              , karma::locals<unsigned int> > operation;
  karma::rule<OutputIterator
              , idl_parser::interface_def<Iterator>
              (interface_, std::vector<std::string>)
              , karma::locals<std::string> > start;
  karma::rule<OutputIterator, idl_parser::param_decl<Iterator>(unsigned int)> args;
};

} } }

#endif
