/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_POA_STUB_GENERATOR_IPP
#define TECORB_IDL_COMPILER_POA_STUB_GENERATOR_IPP

#include <tecorb/idl_compiler/generator/poa_stub_generator.hpp>

#include <tecorb/idl_parser/interface_def.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace tecorb { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator, typename Iterator>
header_poa_stub_generator<OutputIterator, Iterator>::header_poa_stub_generator()
  : header_poa_stub_generator::base_type(start)
{
  namespace phoenix = boost::phoenix;
  using karma::_1;
  using karma::_val;
  using karma::_a;
  using karma::eol;
  using phoenix::at_c;

  start = 
    karma::eps[_a = at_c<0>(_val)]
    << eol << "class POA_"
    << karma::string[_1 = _a] << eol
    << " : public ::tecorb::poa::ServantBase" << eol
    << "{" << eol
    << "public:" << eol
    << common_functions[_1 = _val]
    << indent << "// Start of operations defined in IDL" << eol
    << (*(operation << eol))
    [_1 = at_c<1>(_val)]
    << indent << "// End of operations defined in IDL" << eol
    << construct_local_stub_function[_1 = _a]
    << dispatch_function
    << "};" << eol << eol
    ;
  operation =
    indent
    << -(karma::eps(at_c<3>(_val)) << "virtual ")
    << karma::string[_1 = at_c<0>(_val)]
    << karma::space << karma::stream[_1 = at_c<1>(_val)]
    << "("
    << -(param % ", ")[_1 = at_c<2>(_val)]
    << ")"
    << -(karma::eps(at_c<3>(_val)) << " = 0")
    << ";" << eol
    ;
  param =
    (karma::eps(at_c<1>(_val) == "string")
     << -(karma::eps(at_c<0>(_val) == "in")
          << "const ")
     << "char*"
    )
    | karma::string[_1 = at_c<1>(_val)]
    ;
  common_functions =
    indent
    << "POA_" << karma::string[_1 = at_c<0>(_val)] << "() {}" << eol
    << indent << "~POA_" << karma::string[_1 = at_c<0>(_val)] << "();"
    << eol
    ;
  indent = karma::space << karma::space;
  construct_local_stub_function = 
    indent
    << "::tecorb::Object_ptr _construct_local_stub(std::string const& host" << eol
    << indent << indent << ", unsigned short port, ::tecorb::String_ptr poa_name)"
    << eol
    << indent << "{" << eol
    << indent << indent << "return ::tecorb::Object_ptr(new ::tecorb::local_stub::"
    << karma::string[_1 = _val] << "(host, port, poa_name, this));" << eol
    << indent << "}" << eol
    ;
  dispatch_function =
    indent << "void _dispatch(const char*, const char*, const char*, const char*"
           << ", bool little_endian, ::tecorb::reply&);" << eol
    ;
    
  start.name("header_poa_stub_generator");
  param.name("param");
  construct_local_stub_function.name("construct_local_stub_function");
  dispatch_function.name("dispatch_function");
  common_functions.name("common_functions");
  operation.name("operation");
  karma::debug(start);
  karma::debug(param);
  karma::debug(construct_local_stub_function);
  karma::debug(dispatch_function);
  karma::debug(common_functions);
  karma::debug(operation);
}

template <typename OutputIterator, typename Iterator>
cpp_poa_stub_generator<OutputIterator, Iterator>::cpp_poa_stub_generator()
  : cpp_poa_stub_generator::base_type(start)
{
  namespace phoenix = boost::phoenix;
  using karma::_a;
  using karma::_val;
  using karma::_1;
  using karma::_r1;
  using karma::_r2;
  using karma::_r3;
  using karma::eol;
  using phoenix::at_c;

  start = 
    karma::eps[_a = at_c<0>(_val)]
    << class_name(_a) << "::~"
    << class_name(_a) << "() {}" << eol
    << eol
    << dispatch_function[_1 = _val] << eol
    << (*(non_user_defined_operations(_val) << eol))[_1 = at_c<1>(_val)]
    ;
  dispatch_function =
    karma::eps[_a = at_c<0>(_val)]
    << "void " << class_name(_a) << "::_dispatch(const char* name, const char* first" << eol
    << indent << indent << ", const char* rq_first, const char* rq_last"
    << ", bool little_endian, ::tecorb::reply& reply)" << eol
    << "{" << eol
    << indent
    << (dispatching_if(1, _a) % (indent << "else "))[_1 = at_c<1>(_val)] << eol
    << "}" << eol
    ;
  dispatching_if =
    "if (!std::strcmp(name, \""
    << karma::string[_1 = at_c<1>(_val)] << "\"))" << eol
    << var_indent(_r1) << "{" << eol
    << var_indent(_r1+1) << "// should call &" << class_name(_r2)
    << "::" << karma::string[_1 = at_c<1>(_val)] << eol
    << var_indent(_r1+1) << "std::cout << \"" << class_name(_r2)
    << "::" << karma::string[_1 = at_c<1>(_val)] << "\" << std::endl;" << eol
    << var_indent(_r1+1)
    << "::tecorb::handle_request_body(this, &" << class_name(_r2)
    << "::" << karma::string[_1 = at_c<1>(_val)]
    << ", first, rq_first, rq_last, little_endian, reply);" << eol
    << var_indent(_r1) << "}" << eol
    ;
  indent = karma::space << karma::space;
  class_name = karma::lit("POA_") << karma::string[_1 = _r1];
  non_user_defined_operations =
    -(
      karma::eps(!at_c<3>(_val))[_a = 0u]
      << karma::string[_1 = at_c<0>(_val)] << " " << class_name(at_c<0>(_r1)) << "::"
      << karma::string[_1 = at_c<1>(_val)]
      << "(" << -(param(++_a) % ", ")[_1 = at_c<2>(_val)]
      << ")" << eol
      << "{" << eol
      << non_user_defined_implementation(1, _r1)[_1 = _val]
      << "}" << eol
     )
    ;
  param =
    (
     (karma::eps(at_c<1>(_val) == "string")
      << -(karma::eps(at_c<0>(_val) == "in")
           << "const ")
      << "char*"
     )
     | karma::string[_1 = at_c<1>(_val)]
    ) << " arg" << karma::uint_(_r1)
    ;
  non_user_defined_implementation %= 
    karma::omit[karma::eps(at_c<1>(_val) == "_is_a")] << is_a_impl(_r1, _r2)
    ;
  is_a_impl_strcmp %=
    "!std::strcmp(arg1, \"" << karma::string << "\")"
    ;
  is_a_impl =
    var_indent(_r1)
    << "// implementation for _is_a for interface " << karma::string[_1 = at_c<0>(_r2)] << eol
    << var_indent(_r1) << "return "
    << (is_a_impl_strcmp % " || ")[_1 = at_c<2>(_r2)]
    << ";" << eol
    ;
  var_indent = karma::repeat(_r1)[karma::space << karma::space]
    ;

  start.name("cpp_poa_stub_generator");
  param.name("param");
  non_user_defined_operations.name("non_user_defined_operations");
  non_user_defined_implementation.name("non_user_defined_implementation");
  is_a_impl.name("is_a_impl");
  class_name.name("class_name");
  dispatching_if.name("dispatching_if");
  dispatch_function.name("dispatching_function");
  is_a_impl_strcmp.name("is_a_impl_strcmp");
  karma::debug(start);
  karma::debug(param);
  karma::debug(non_user_defined_operations);
  karma::debug(class_name);
  karma::debug(dispatching_if);
  karma::debug(dispatch_function);
  karma::debug(non_user_defined_implementation);
  karma::debug(is_a_impl);
  karma::debug(is_a_impl_strcmp);
}

} } }

#endif
