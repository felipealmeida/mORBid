/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_STUB_GENERATOR_HPP
#define TECORB_IDL_COMPILER_STUB_GENERATOR_HPP

#include <tecorb/idl_parser/interface_def.hpp>
#include <tecorb/idl_compiler/generator/parameter.hpp>

#include <boost/spirit/home/karma.hpp>

#include <string>
#include <ostream>
#include <vector>

namespace tecorb { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator, typename Iterator>
struct header_stub_generator : karma::grammar
<OutputIterator, idl_parser::interface_def<Iterator>(), karma::locals<std::string> >
{
  header_stub_generator();

  idl_compiler::parameter<OutputIterator> parameter;
  karma::rule<OutputIterator> indent, public_members;
  karma::rule<OutputIterator, std::string()> typedefs;
  karma::rule<OutputIterator
              , idl_parser::interface_def<Iterator>()> common_functions;
  karma::rule<OutputIterator
              , idl_parser::op_decl<Iterator>()> operation;
  karma::rule<OutputIterator
              , idl_parser::interface_def<Iterator>()
              , karma::locals<std::string> > start;
};

template <typename OutputIterator, typename Iterator>
struct cpp_stub_generator : karma::grammar
<OutputIterator, idl_parser::interface_def<Iterator>(), karma::locals<std::string> >
{
  cpp_stub_generator();

  idl_compiler::parameter<OutputIterator> parameter;
  karma::rule<OutputIterator> indent;
  karma::rule<OutputIterator, std::string()> members;
  karma::rule<OutputIterator, std::string()> construct_remote_stub;
  karma::rule<OutputIterator
              , idl_parser::interface_def<Iterator>()
              , karma::locals<std::string> > start;
};

} } }

#endif
