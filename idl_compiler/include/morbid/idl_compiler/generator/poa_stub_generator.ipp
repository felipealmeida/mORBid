/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_POA_STUB_GENERATOR_IPP
#define TECORB_IDL_COMPILER_POA_STUB_GENERATOR_IPP

#include <morbid/idl_compiler/generator/poa_stub_generator.hpp>

#include <morbid/idl_parser/interface_def.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace std {

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, std::pair<T, U> p)
{
  return os << "[pair first: " << p.first << " second: " << p.second << ']';
}

}

namespace morbid { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator, typename Iterator>
header_poa_stub_generator<OutputIterator, Iterator>::header_poa_stub_generator()
  : header_poa_stub_generator::base_type(start)
{
  namespace phoenix = boost::phoenix;
  using karma::_1;
  using karma::_val;
  using karma::_a; using karma::_r1; using karma::_r2;
  using karma::eol;
  using phoenix::at_c;

  start = 
    eol << "class " << class_name(_r2)[_1 = at_c<0>(_val)] << eol
    << " : public ::morbid::poa::ServantBase" << eol
    << "{" << eol
    << "public:" << eol
    << common_functions(_r1, _r2)[_1 = _val]
    << indent << "// Start of operations defined in IDL" << eol
    << (*(operation(_r1) << eol))[_1 = at_c<1>(_val)]
    << indent << "// End of operations defined in IDL" << eol
    << construct_local_stub_function[_1 = at_c<0>(_val)]
    << dispatch_function
    << "};" << eol << eol
    ;
  class_name =
    -(karma::eps(_r1) << "POA_")
    << karma::string[_1 = _val]
    ;
  operation =
    indent
    << -(karma::eps(at_c<3>(_val)) << "virtual ")
    << type_spec
    (
     at_c<1>(_r1)[at_c<0>(_val)] // interface_.lookups[type_spec]
    )
    [_1 = at_c<0>(_val)]
    << karma::space
    << karma::string[_1 = at_c<1>(_val)]
    << "("
    << -(parameter_select(_r1) % ", ")[_1 = at_c<2>(_val)]
    << ")"
    << -(karma::eps(at_c<3>(_val)) << " = 0")
    << ";"
    ;
  parameter_select %= parameter(at_c<1>(_r1)[at_c<1>(_val)]);
  common_functions =
    indent
    << class_name(_r2)[_1 = at_c<0>(_val)] << "() {}" << eol
    << indent << "~" << class_name(_r2)[_1 = at_c<0>(_val)] << "();"
    << eol
    ;
  indent = karma::space << karma::space;
  construct_local_stub_function = 
    indent
    << "::morbid::Object_ptr _construct_local_stub(std::string const& host" << eol
    << indent << indent << ", unsigned short port, ::morbid::String_ptr poa_name);"
    << eol
    // << indent << "{" << eol
    // << indent << indent << "return ::morbid::Object_ptr(new local_stub::"
    // << karma::string[_1 = _val] << "(host, port, poa_name, this));" << eol
    // << indent << "}" << eol
    ;
  dispatch_function =
    indent << "void _dispatch(const char*, const char*, const char*, const char*"
           << ", bool little_endian, ::morbid::reply&);" << eol
    ;
    
  start.name("header_poa_stub_generator");
  construct_local_stub_function.name("construct_local_stub_function");
  dispatch_function.name("dispatch_function");
  common_functions.name("common_functions");
  operation.name("operation");
  karma::debug(start);
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
  using karma::_r1; using karma::_r2; using karma::_r3;
  using karma::_r4;
  using karma::eol;
  using phoenix::at_c;

  start = 
    class_name(_r2)[_1 = at_c<0>(_val)] << "::~"
    << class_name(_r2)[_1 = at_c<0>(_val)] << "() {}" << eol
    << eol
    << dispatch_function(_r2, _r1)[_1 = _val] << eol
    << (*(non_user_defined_operations(_val, _r1, _r2) << eol))[_1 = at_c<1>(_val)]
    << construct_local_stub_function(_r2)[_1 = at_c<0>(_val)]
    ;
  dispatch_function =
    "void " << class_name(_r1)[_1 = at_c<0>(_val)]
    << "::_dispatch(const char* name, const char* first" << eol
    << indent << indent << ", const char* rq_first, const char* rq_last"
    << ", bool little_endian, ::morbid::reply& reply)" << eol
    << "{" << eol
    << indent
    << (dispatching_if(1, at_c<0>(_val), _r1, _r2) % (indent << "else "))[_1 = at_c<1>(_val)] << eol
    << "}" << eol
    ;
  dispatching_if =
    "if (!std::strcmp(name, \""
    << karma::string[_1 = at_c<1>(_val)] << "\"))" << eol
    << var_indent(_r1) << "{" << eol
    << var_indent(_r1+1) << "// should call &" << class_name(_r3)[_1 = _r2]
    << "::" << karma::string[_1 = at_c<1>(_val)] << eol
    << var_indent(_r1+1) << "std::cout << \"" << class_name(_r3)[_1 = _r2]
    << "::" << karma::string[_1 = at_c<1>(_val)] << "\" << std::endl;" << eol
    << var_indent(_r1+1)
    << "::morbid::handle_request_body< ::boost::mpl::vector< "
    << -(synchronous_template_args(_r4) % ", ")[_1 = at_c<2>(_val)]
    << " > >(this, &" << class_name(_r3)[_1 = _r2]
    << "::" << karma::string[_1 = at_c<1>(_val)]
    << ", first, rq_first, rq_last, little_endian, reply);" << eol
    << var_indent(_r1) << "}" << eol
    ;
  in_tag = karma::string[_1 = "::morbid::type_tag::in_tag"];
  out_tag = karma::string[_1 = "::morbid::type_tag::out_tag"];
  inout_tag = karma::string[_1 = "::morbid::type_tag::inout_tag"];
  synchronous_template_args = 
    "::morbid::type_tag::value_type_tag< "
    << type_spec_select(_r1)[_1 = _val]
    << ", " << (in_tag | out_tag | inout_tag)[_1 = at_c<0>(_val)]
    << ">"
    ;
  indent = karma::space << karma::space;
  class_name =
    -(karma::eps(_r1) << "POA_")
    << karma::string[_1 = _val]
    ;
  non_user_defined_operations =
    -(
      karma::eps(!at_c<3>(_val))[_a = 0u]
      << type_spec
      (
       at_c<1>(_r2)[at_c<0>(_val)]
      )
      [_1 = at_c<0>(_val)]
      << karma::space << class_name(_r3)[_1 = at_c<0>(_r1)] << "::"
      << karma::string[_1 = at_c<1>(_val)]
      << '(' << -((parameter_select(_r2) << " arg" << karma::lit(++_a)) % ", ")[_1 = at_c<2>(_val)]
      << ')' << eol
      << '{' << eol
      << non_user_defined_implementation(1, _r1)[_1 = _val]
      << '}' << eol
     )
    ;
  parameter_select %= parameter(at_c<1>(_r1)[at_c<1>(_val)]);
  type_spec_select = type_spec(at_c<1>(_r1)[at_c<1>(_val)])[_1 = at_c<1>(_val)];
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
  construct_local_stub_function = 
    indent
    << "::morbid::Object_ptr " << class_name(_r1)[_1 = _val]
    << "::_construct_local_stub(std::string const& host" << eol
    << indent << indent << ", unsigned short port, ::morbid::String_ptr poa_name)"
    << eol
    << indent << "{" << eol
    << indent << indent << "return ::morbid::Object_ptr(new local_stub::"
    << karma::string[_1 = _val] << "(host, port, poa_name, this));" << eol
    << indent << "}" << eol
    ;

  // start.name("cpp_poa_stub_generator");
  // non_user_defined_operations.name("non_user_defined_operations");
  // non_user_defined_implementation.name("non_user_defined_implementation");
  // is_a_impl.name("is_a_impl");
  // class_name.name("class_name");
  // dispatching_if.name("dispatching_if");
  // dispatch_function.name("dispatching_function");
  // is_a_impl_strcmp.name("is_a_impl_strcmp");
  // karma::debug(start);
  // karma::debug(non_user_defined_operations);
  // karma::debug(class_name);
  // karma::debug(dispatching_if);
  // karma::debug(dispatch_function);
  // karma::debug(non_user_defined_implementation);
  // karma::debug(is_a_impl);
  // karma::debug(is_a_impl_strcmp);
}

} } }

#endif
