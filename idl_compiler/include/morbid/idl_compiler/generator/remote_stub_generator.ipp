/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_REMOTE_STUB_GENERATOR_IPP
#define TECORB_IDL_COMPILER_REMOTE_STUB_GENERATOR_IPP

#include <morbid/idl_compiler/generator/parameter.hpp>
#include <morbid/idl_compiler/generator/remote_stub_generator.hpp>

#include <morbid/idl_parser/interface_def.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace morbid { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator, typename Iterator>
header_remote_stub_generator<OutputIterator, Iterator>::header_remote_stub_generator()
  : header_remote_stub_generator::base_type(start)
{
  namespace phoenix = boost::phoenix;
  using karma::_1;
  using karma::_val;
  using karma::_a;
  using karma::eol;
  using phoenix::at_c;
  
  start = 
    "namespace morbid { namespace remote_stub {"
    << eol[_a = at_c<0>(_val)]
    << eol << "class "
    << karma::string[_1 = _a] << eol
    << " : public ::" << karma::string[_1 = _a] << eol
    << "{" << eol
    << "public:" << eol
    << common_functions[_1 = _val]
    << indent << "// Start of operations defined in IDL" << eol
    << (*(operation << eol))
    [_1 = at_c<1>(_val)]
    << indent << "// End of operations defined in IDL" << eol
    << ior_function
    << "private:" << eol
    << common_members[_1 = _a]
    << "};" << eol << eol
    << "} }" << eol
    ;
  operation =
    indent
    << karma::string[_1 = at_c<0>(_val)]
    << karma::space << karma::string[_1 = at_c<1>(_val)]
    << "("
    << -(parameter % ", ")[_1 = at_c<2>(_val)]
    << ");"
    ;
  
  common_functions =
    indent
    << "// Constructor" << eol
    << indent
    << (
        karma::string[_1 = at_c<0>(_val)]
        << "(std::string const& host, unsigned short port" << eol
        << indent << indent << indent << ", std::string const& object_key)" << eol
        << indent << indent << " : host(host), port(port), object_key(object_key)"
        << eol
        << indent << "{}" << eol
       )
    << indent << "~" << karma::string[_1 = at_c<0>(_val)] << "();" << eol
    ;

  common_members =
    indent
    << "// Members" << eol
    << indent << "std::string host;" << eol
    << indent << "unsigned short port;" << eol
    << indent << "std::string object_key;" << eol
    ;
  indent = karma::space << karma::space;
  ior_function =
    indent
    << "::morbid::String_ptr ior() const;" << eol
    ;
}

template <typename OutputIterator, typename Iterator>
cpp_remote_stub_generator<OutputIterator, Iterator>::cpp_remote_stub_generator()
  : cpp_remote_stub_generator::base_type(start)
{
  namespace phoenix = boost::phoenix;
  using karma::_a;
  using karma::_val;
  using karma::_1;
  using karma::_2;
  using karma::eol;
  using karma::_r1;
  using phoenix::at_c;

  start = 
    "namespace morbid { namespace remote_stub {"
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
    "::morbid::String_ptr "
    << karma::string[_1 = _val] << "::ior() const" << eol
    << "{" << eol
    << indent << "return ::morbid::poa::create_ior_string" << eol
    << indent << indent << "(host, port, object_key);" << eol
    << "}" << eol
    ;
  operation =
    karma::string[_1 = phoenix::at_c<0>(_val)]
    << karma::space << karma::string[_1 = _r1]
    << "::" << karma::string[_1 = phoenix::at_c<1>(_val)]
    << "("
    << karma::eps[_a = 0]
    << -((parameter << " arg" << karma::lit(++_a)) % ", ")[_1 = at_c<2>(_val)]
    << ")" << eol
    << "{" << eol
    << (
        indent
        << "std::cout << \"Called " << karma::string[_1 = phoenix::at_c<1>(_val)]
        << " was called\" << std::endl;" << eol
        << indent << "return ::morbid::synchronous_call::call" << eol
        << indent << indent << "<" << karma::string[_1 = at_c<0>(_val)]
        << (*(eol << indent << indent << ", " << synchronous_template_args))[_1 = at_c<2>(_val)]
        << eol << indent << indent << ">" << eol
        << indent << indent
        << "(_repository_id, \"" << karma::string[_1 = at_c<1>(_val)]
        << "\", host, port, object_key"
        << (*(", " << synchronous_args))[_1 = at_c<2>(_val)]
        << ");" << eol
       )
    << "}" << eol
    ;
  synchronous_template_args
    = "::morbid::type_tag::value_type_tag<"
    << parameter[_1 = _val]
    << ", ::morbid::type_tag::" << karma::string[_1 = at_c<0>(_val)]
    << "_tag>";
  synchronous_args = karma::eps[_a = 0u] << "arg" << karma::lit(++_a);
  indent = karma::space << karma::space;
}

} } }

#endif
