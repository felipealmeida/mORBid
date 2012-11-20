/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_STUB_GENERATOR_IPP
#define TECORB_IDL_COMPILER_STUB_GENERATOR_IPP

#include <morbid/idl_compiler/generator/stub_generator.hpp>

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
header_stub_generator<OutputIterator, Iterator>::header_stub_generator()
  : header_stub_generator::base_type(start)
{
  namespace phoenix = boost::phoenix;
  using karma::_1;
  using karma::_val;
  using karma::_a;
  using karma::_r1;
  using karma::eol;
  using phoenix::at_c;

  start = 
    karma::eps[_a = at_c<0>(_val)]
    << eol
    << "class " << karma::string[_1 = _a] << ';' << eol
    << typedefs[_1 = _a] << eol
    << "class "
    << karma::string[_1 = _a]
    << eol << " : public ::morbid::narrow< "
    << karma::string[_1 = _a]
    << ", ::boost::mpl::vector1< ::morbid::Object> >"
    << eol << "{" << eol
    << "public:" << eol
    << indent << "typedef " << karma::string[_1 = _a] << "_ptr _ptr_type;" << eol
    << indent << "typedef " << karma::string[_1 = _a] << "_var _var_type;" << eol << eol
    << common_functions[_1 = _val] << eol
    << indent << "// Start of operations defined in IDL" << eol
    << (*(operation(_r1) << eol))
    [_1 = at_c<1>(_val)]
    << indent << "// End of operations defined in IDL" << eol
    << public_members << eol
    << indent << "typedef ::morbid::interface_tag _morbid_type_kind;" << eol
    << "};" << eol << eol
    ;
  ;
  operation =
    -(
      karma::eps(at_c<3>(_val))
      << indent << "virtual "
      << return_
      (
       at_c<1>(_r1)[at_c<0>(_val)]
      )
      [_1 = at_c<0>(_val)]
      << karma::space << karma::stream[_1 = at_c<1>(_val)]
      << "("
      << -(parameter_select(_r1)
           % ", ")[_1 = at_c<2>(_val)]
      << ") = 0;"
     )
    ;
  parameter_select %= parameter(at_c<1>(_r1)[at_c<1>(_val)]);

  common_functions =
    indent
    << karma::string[_1 = at_c<0>(_val)] << "() {}" << eol
    << indent << "~" << karma::string[_1 = at_c<0>(_val)] << "();"
    << eol
    << indent << "static boost::shared_ptr<"
    << karma::string[_1 = at_c<0>(_val)] << ">"
    << " _construct_remote_stub" << eol
    << indent << "(::morbid::structured_ior const& ior);" << eol
    << indent << "static _ptr_type _nil() { return _ptr_type(); }" << eol
    << indent << "::morbid::return_traits< ::morbid::string>::type _get_interface() const { return _repository_id; }"
    ;
  public_members = 
    indent
    << "static const char* _repository_id;" << eol
    ;
  typedefs =
    "typedef boost::shared_ptr<"
    << karma::string[_1 = _val]
    << "> " << karma::string[_1 = _val] << "_ptr;" << eol
    << "typedef ::morbid::var<" << karma::string[_1 = _val] << "> "
    << karma::string[_1 = _val] << "_var;" << eol;
  indent = karma::space << karma::space;

  // start.name("header_stub_generator");
  // karma::debug(start);
  // common_functions.name("common_functions");
  // karma::debug(common_functions);
}

template <typename OutputIterator, typename Iterator>
cpp_stub_generator<OutputIterator, Iterator>::cpp_stub_generator()
  : cpp_stub_generator::base_type(start)
{
  namespace phoenix = boost::phoenix;
  using karma::_a;
  using karma::_val;
  using karma::_1; using karma::_r2; using karma::_r1;
  using karma::eol;
  using phoenix::at_c;
  
  start = 
    karma::string[_1 = at_c<0>(_val)]
    << "::~" << karma::string[_1 = at_c<0>(_val)] << "() {}" << eol
    << eol
    << construct_remote_stub[_1 = at_c<0>(_val)] << eol
    << members(_r2)[_1 = at_c<0>(_val)] << eol
    ;
  construct_remote_stub =
    "boost::shared_ptr<"
    << karma::string[_1 = _val] << "> "
    << karma::string[_1 = _val]
    << "::_construct_remote_stub" << eol
    << indent << "(::morbid::structured_ior const& ior)" << eol
    << "{" << eol
    << indent << "return " << karma::string[_1 = _val]
    << "_ptr(new remote_stub::" << karma::string[_1 = _val]
    << "(ior));" << eol
    << "}" << eol
    ;
  members =
    "const char* "
    << karma::string[_1 = _val] << "::_repository_id = \"IDL:"
    << (karma::string % '/')[_1 = _r1] << ":1.0\";" << eol
    ;
  indent = karma::space << karma::space;
}

} } }

#endif
