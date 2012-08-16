/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_POA_STUB_GENERATOR_HPP
#define TECORB_IDL_COMPILER_POA_STUB_GENERATOR_HPP

#include <tecorb/idl_parser/interface_def.hpp>

#include <boost/spirit/home/karma.hpp>

namespace tecorb { namespace idl_compiler {

namespace karma = boost::spirit::karma;

template <typename OutputIterator, typename Iterator>
struct header_poa_stub_generator : karma::grammar
<OutputIterator, idl_parser::interface_def<Iterator>(), karma::locals<std::string> >
{
  header_poa_stub_generator()
    : header_poa_stub_generator::base_type(start)
  {
    namespace phoenix = boost::phoenix;
    using karma::_1;
    using karma::_val;
    using karma::_a;
    using karma::eol;

    start = 
      karma::eps[_a = phoenix::at_c<0>(_val)]
      << eol << "class POA_"
      << karma::string[_1 = _a]
      << eol << " : public ::tecorb::narrow< POA_"
      << karma::string[_1 = _a]
      << ", ::boost::mpl::vector1< ::tecorb::poa::ServantBase> >"
      << eol << "{" << eol
      << "public:" << eol
      << common_functions[_1 = _val]
      << indent << "// Start of operations defined in IDL" << eol
      << (*(operation << eol))
      [_1 = phoenix::at_c<1>(_val)]
      << indent << "// End of operations defined in IDL" << eol
      << construct_local_stub_function[_1 = _a]
      << "};" << eol << eol
      ;
    operation =
      indent
      << "virtual "
      << karma::string[_1 = phoenix::at_c<0>(_val)]
      << karma::space << karma::stream[_1 = phoenix::at_c<1>(_val)]
      << "() = 0;"
      ;

    common_functions =
      indent
      << "POA_" << karma::string[_1 = phoenix::at_c<0>(_val)] << "() {}" << eol
      << indent << "~POA_" << karma::string[_1 = phoenix::at_c<0>(_val)] << "();"
      << eol
      ;
    indent = karma::space << karma::space;
    construct_local_stub_function = 
      indent
      << "::tecorb::Object_ptr _construct_local_stub(std::string const& host" << eol
      << indent << indent << ", unsigned short port, ::tecorb::String_ptr poa_name) const" << eol
      << indent << "{" << eol
      << indent << indent << "return ::tecorb::Object_ptr(new ::tecorb::local_stub::"
      << karma::string[_1 = _val] << "(host, port, poa_name));" << eol
      << indent << "}" << eol
      ;
  }

  karma::rule<OutputIterator, std::string()> construct_local_stub_function;
  karma::rule<OutputIterator> indent;
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
  cpp_poa_stub_generator()
    : cpp_poa_stub_generator::base_type(start)
  {
    namespace phoenix = boost::phoenix;
    using karma::_a;
    using karma::_val;
    using karma::_1;
    using karma::eol;

    start = 
      karma::eps[_a = phoenix::at_c<0>(_val)]
      << "POA_" << karma::string[_1 = _a] << "::~POA_"
      << karma::string[_1 = _a] << "() {}" << eol
      << eol
      ;
  }

  karma::rule<OutputIterator
              , idl_parser::interface_def<Iterator>()
              , karma::locals<std::string> > start;
};

} }

#endif
