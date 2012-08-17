/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_LOCAL_STUB_GENERATOR_HPP
#define TECORB_IDL_COMPILER_LOCAL_STUB_GENERATOR_HPP

#include <tecorb/idl_parser/interface_def.hpp>

#include <boost/spirit/home/karma.hpp>

namespace tecorb { namespace idl_compiler {

namespace karma = boost::spirit::karma;

template <typename OutputIterator, typename Iterator>
struct header_local_stub_generator : karma::grammar
<OutputIterator, idl_parser::interface_def<Iterator>(), karma::locals<std::string> >
{
  header_local_stub_generator()
    : header_local_stub_generator::base_type(start)
  {
    namespace phoenix = boost::phoenix;
    using karma::_1;
    using karma::_val;
    using karma::_a;
    using karma::eol;

    start = 
      "namespace tecorb { namespace local_stub {"
      << eol[_a = phoenix::at_c<0>(_val)]
      << eol << "class "
      << karma::string[_1 = _a] << eol
      << " : public ::" << karma::string[_1 = _a] << eol
      << "{" << eol
      << "public:" << eol
      << common_functions[_1 = _val]
      << indent << "// Start of operations defined in IDL" << eol
      << (*(operation << eol))
      [_1 = phoenix::at_c<1>(_val)]
      << indent << "// End of operations defined in IDL" << eol
      << ior_function
      << "private:" << eol
      << common_members[_1 = _a]
      << "};" << eol << eol
      << "} }" << eol
      ;
    operation =
      indent
      << karma::string[_1 = phoenix::at_c<0>(_val)]
      << karma::space << karma::string[_1 = phoenix::at_c<1>(_val)]
      << "();" << eol
      ;

    common_functions =
      indent
      << "// Constructor" << eol
      << indent
      << (
          karma::string[_1 = phoenix::at_c<0>(_val)]
          << "(std::string const& host, unsigned short port" << eol
          << indent << indent << ", ::tecorb::String_ptr poa_name" << eol
          << indent << indent << ", POA_" << karma::string[_1 = phoenix::at_c<0>(_val)]
          << "* servant)" << eol
          << indent << " : host(host), port(port), poa_name(poa_name), servant(servant)" << eol
          << indent << "{}" << eol
         )
      << indent << "~" << karma::string[_1 = phoenix::at_c<0>(_val)] << "();" << eol
      << indent << "bool _is_a(const char*)" << eol
      << indent << "{" << eol
      << indent << indent << "return true;" << eol
      << indent << "}" << eol
      ;

    common_members =
      indent
      << "// Members" << eol
      << indent << "std::string host;" << eol
      << indent << "unsigned short port;" << eol
      << indent << "::tecorb::String_ptr poa_name;" << eol
      << indent << "::POA_" << karma::string[_1 = _val] << "* servant;" << eol
      ;
    indent = karma::space << karma::space;
    ior_function =
      indent
      << "::tecorb::String_ptr ior() const;" << eol
      ;
  }

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
struct cpp_local_stub_generator : karma::grammar
<OutputIterator, idl_parser::interface_def<Iterator>(), karma::locals<std::string> >
{
  cpp_local_stub_generator()
    : cpp_local_stub_generator::base_type(start)
  {
    namespace phoenix = boost::phoenix;
    using karma::_a;
    using karma::_val;
    using karma::_1;
    using karma::_2;
    using karma::eol;
    using karma::_r1;

    start = 
      "namespace tecorb { namespace local_stub {"
      << eol << eol
      << karma::eps[_a = phoenix::at_c<0>(_val)]
      << karma::string[_1 = _a] << "::~" << karma::string[_1 = _a] << "() {}" << eol
      << eol
      << "// Start of operations defined in IDL" << eol
      << (*(operation(_a) << eol))
      [_1 = phoenix::at_c<1>(_val)]
      << "// End of operations defined in IDL" << eol
      << ior_function[_1 = _a] << eol
      << "} }" << eol << eol
      ;
    ior_function =
      "::tecorb::String_ptr "
      << karma::string[_1 = _val] << "::ior() const" << eol
      << "{" << eol
      << indent << "return ::tecorb::poa::create_ior_string" << eol
      << indent << indent << "(host, port, poa_name, servant);" << eol
      << "}" << eol
      ;
    operation =
      karma::string[_1 = phoenix::at_c<0>(_val)]
      << karma::space << karma::string[_1 = _r1]
      << "::" << karma::string[_1 = phoenix::at_c<1>(_val)]
      << "()" << eol
      << "{" << eol
      << (
          indent << "return servant->" << karma::string[_1 = phoenix::at_c<1>(_val)]
          << "();" << eol
         )
      << "}" << eol
      ;
    indent = karma::space << karma::space;
  }

  karma::rule<OutputIterator, std::string()> ior_function;
  karma::rule<OutputIterator> indent;
  karma::rule<OutputIterator
              , idl_parser::op_decl<Iterator>(std::string)> operation;
  karma::rule<OutputIterator
              , idl_parser::interface_def<Iterator>()
              , karma::locals<std::string> > start;
};

} }

#endif
