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
  using karma::eol;
  using phoenix::at_c;

  start = 
    karma::eps[_a = phoenix::at_c<0>(_val)]
    << eol << "class "
    << karma::string[_1 = _a]
    << eol << " : public ::morbid::narrow< "
    << karma::string[_1 = _a]
    << ", ::boost::mpl::vector1< ::morbid::Object> >"
    << eol << "{" << eol
    << "public:" << eol
    << common_functions[_1 = _val] << eol
    << indent << "// Start of operations defined in IDL" << eol
    << (*(operation << eol))
    [_1 = phoenix::at_c<1>(_val)]
    << indent << "// End of operations defined in IDL" << eol
    << public_members << eol
    << "};" << eol << eol
    << typedefs[_1 = _a] << eol;
  ;
  operation =
    -(
      karma::eps(phoenix::at_c<3>(_val))
      << indent << "virtual "
      << karma::string[_1 = phoenix::at_c<0>(_val)]
      << karma::space << karma::stream[_1 = phoenix::at_c<1>(_val)]
      << "("
      << -(parameter % ", ")[_1 = at_c<2>(_val)]
      << ") = 0;"
     )
    ;

  common_functions =
    indent
    << karma::string[_1 = phoenix::at_c<0>(_val)] << "() {}" << eol
    << indent << "~" << karma::string[_1 = phoenix::at_c<0>(_val)] << "();"
    << eol
    << indent << "static boost::shared_ptr<"
    << karma::string[_1 = phoenix::at_c<0>(_val)] << ">"
    << " _construct_remote_stub" << eol
    << indent << "(std::string const& host, unsigned short port" << eol
    << indent << indent << indent << ", std::string const& object_key);" << eol
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
}

template <typename OutputIterator, typename Iterator>
cpp_stub_generator<OutputIterator, Iterator>::cpp_stub_generator()
  : cpp_stub_generator::base_type(start)
{
  namespace phoenix = boost::phoenix;
  using karma::_a;
  using karma::_val;
  using karma::_1;
  using karma::eol;
  
  start = 
    karma::eps[_a = phoenix::at_c<0>(_val)]
    << karma::string[_1 = _a] << "::~" << karma::string[_1 = _a] << "() {}" << eol
    << eol
    << construct_remote_stub[_1 = _a] << eol
    << members[_1 = _a] << eol
    ;
  construct_remote_stub =
    "boost::shared_ptr<"
    << karma::string[_1 = _val] << "> "
    << karma::string[_1 = _val]
    << "::_construct_remote_stub" << eol
    << indent << "(std::string const& host, unsigned short port" << eol
    << indent << indent << ", std::string const& object_key)" << eol
    << "{" << eol
    << indent << "return " << karma::string[_1 = _val]
    << "_ptr(new ::morbid::remote_stub::" << karma::string[_1 = _val]
    << "(host, port, object_key));" << eol
    << "}" << eol
    ;
  members =
    "const char* "
    << karma::string[_1 = _val] << "::_repository_id = \"IDL:"
    << karma::string[_1 = _val] << ":1.0\";" << eol
    ;
  indent = karma::space << karma::space;
}

} } }

#endif
