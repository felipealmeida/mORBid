/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_LOCAL_STUB_GENERATOR_IPP
#define TECORB_IDL_COMPILER_LOCAL_STUB_GENERATOR_IPP

#include <morbid/idl_compiler/generator/local_stub_generator.hpp>
#include <morbid/idl_compiler/generator/return.hpp>

#include <morbid/idl_parser/interface_def.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace morbid { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator, typename Iterator>
header_local_stub_generator<OutputIterator, Iterator>::header_local_stub_generator()
  : header_local_stub_generator::base_type(start)
{
  namespace phoenix = boost::phoenix;
  using karma::_1;
  using karma::_val;
  using karma::_a;
  using karma::_r1; using karma::_r2; using karma::_r3;
  using karma::eol;
  using phoenix::at_c;

  start = karma::eps
    // "namespace local_stub {"
    // << eol[_a = at_c<0>(_val)]
    // << eol << "class "
    // << karma::string[_1 = _a] << eol
    // << " : public ::" << -((karma::string % "::")[_1 = _r2] << "::")
    // << karma::string[_1 = _a]
    // << eol
    // << "{" << eol
    // << "public:" << eol
    // << common_functions(_r2)[_1 = _val]
    // << indent << "// Start of operations defined in IDL" << eol
    // << (*(operation(_r1) << eol))[_1 = at_c<1>(_val)]
    // << indent << "// End of operations defined in IDL" << eol
    // << ior_function
    // << "private:" << eol
    // << common_members(_r2)[_1 = _a]
    // << "};" << eol << eol
    // << karma::string[_1 = _a] << "::~" << karma::string[_1 = _a] << "() {}" << eol << eol
    // << "}" << eol
    ;
  operation =
    indent
    << return_
    (
     at_c<1>(_r1)[at_c<0>(_val)]
    )[_1 = at_c<0>(_val)]
    << karma::space << karma::string[_1 = at_c<1>(_val)]
    << "("
    << -((parameter_select(_r1) << " arg" << karma::lit(++_a)) % ", ")[_1 = at_c<2>(_val)]
    << ")" << eol
    << indent << "{" << eol
    << karma::eps[_a = 0]
    << (
        indent << indent << "return servant->" << karma::string[_1 = at_c<1>(_val)]
        << "(" << -(args(++_a) % ", ")[_1 = at_c<2>(_val)]
        << ");" << eol
       )
    << indent << "}" << eol
    ;
  args = "arg" << karma::lit(_r1);
  parameter_select %= parameter(at_c<1>(_r1)[at_c<1>(_val)]);
  poa_class_name =
    karma::lit("::") << "POA_"
                     << -((karma::string % "::") << "::")[_1 = _r1]
                     << karma::string[_1 = _val]
    ;

  common_functions =
    indent
    << "// Constructor" << eol
    << indent
    << (
        karma::string[_1 = at_c<0>(_val)]
        << "(::morbid::structured_ior const& ior" << ", " << poa_class_name(_r1)[_1 = at_c<0>(_val)]
        << "* servant)" << eol
        << indent << " : _structured_ior_(ior), servant(servant)" << eol
        << indent << "{}" << eol
       )
    << indent << "~" << karma::string[_1 = at_c<0>(_val)] << "();" << eol
    << indent << "::morbid::structured_ior _structured_ior() const { return _structured_ior_; }" << eol
    ;

  common_members =
    indent
    << "// Members" << eol
    << indent << "::morbid::structured_ior _structured_ior_;" << eol
    << indent << poa_class_name(_r1)[_1 = _val] << "* servant;" << eol
    ;
  indent = karma::space << karma::space;
  ior_function = karma::eps
    ;
}

} } }

#endif
