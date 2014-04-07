/* (c) Copyright 2014 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_CONCEPT_INTERFACE_IPP
#define MORBID_IDL_COMPILER_CONCEPT_INTERFACE_IPP

#include <morbid/idl_compiler/generator/concept_interface.hpp>
#include <morbid/idl_compiler/generator/wave_string.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/phoenix.hpp>
#include <boost/phoenix/fusion/at.hpp>

namespace morbid { namespace idlc { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
concept_interface<OutputIterator>::concept_interface()
  : concept_interface::base_type(start)
{
  namespace phoenix = boost::phoenix;
  using karma::eol;
  using karma::_1; using karma::_val;
  using karma::_r1; using karma::_r2; using karma::_r3;
  using karma::_a;
  using phoenix::at_c;
  using phoenix::second;
  using phoenix::find;

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsequenced"
#endif

  operation_name = wave_string[_1 = at_c<1>(_val)];
  attribute_name = "_get_" << wave_string[_1 = at_c<2>(_val)];
  operation_concept_interface_specialization =
    "template <class C, class Base>" 
    << eol
    << "struct concept_interface< " << -(wave_string % "::")[_1 = _r2]
    << "::" << wave_string[_1 = _r3] << "_concept::" << operation_name[_1 = _val] << "<C>, Base, C> : Base" << eol
    << "{" << eol
    << indent << " typename ::morbid::lazy_eval< "
    << return_(second(*find(at_c<1>(_r1), at_c<0>(_val))))[_1 = at_c<0>(_val)]
    << ", C>::type" << eol
    << karma::space << operation_name[_1 = _val]
    << '('
    << -(
         (("typename as_param<Base, " << parameter_select(_r1)
           << " >::type arg" << karma::lit(++_a)) % ", ")[_1 = at_c<2>(_val)]
        )
    << ')' << eol
    << indent << "{" << eol
    << indent << indent << "return call( typename ::morbid::lazy_eval< " << -(wave_string % "::")[_1 = _r2]
    << "::" << wave_string[_1 = _r3] << "_concept::" << operation_name[_1 = _val] << "<C>, C>::type(), *this"
    << karma::eps[_a = 0]
    << (*(", " << karma::attr_cast<idl_parser::param_decl>(karma::eps) << "arg" << karma::lit(++_a)))[_1 = at_c<2>(_val)]
    << ");" << eol
    << indent << "}" << eol
    << "};" << eol
    ;
  attribute_concept_interface_specialization =
    "template <class C, class Base>" 
    << eol
    << "struct concept_interface< " << -(wave_string % "::")[_1 = _r2]
    << "::" << wave_string[_1 = _r3] << "_concept::" << attribute_name[_1 = _val] << "<C>, Base, C> : Base" << eol
    << "{" << eol
    << indent << " typename ::morbid::lazy_eval< "
    << return_(second(*find(at_c<1>(_r1), at_c<1>(_val))))[_1 = at_c<1>(_val)]
    << ", C>::type" << eol
    << karma::space << attribute_name[_1 = _val]
    << '('
    << ") const" << eol
    << indent << "{" << eol
    << indent << indent << "return call( typename ::morbid::lazy_eval< " << -(wave_string % "::")[_1 = _r2]
    << "::" << wave_string[_1 = _r3] << "_concept::" << attribute_name[_1 = _val] << "<C>, C>::type(), *this"
    << ");" << eol
    << indent << "}" << eol
    << "};" << eol
    ;
  start =
    (*operation_concept_interface_specialization(_r1, _r2, at_c<0>(_val)))[_1 = at_c<1>(_val)]
    << (*attribute_concept_interface_specialization(_r1, _r2, at_c<0>(_val)))[_1 = at_c<2>(_val)]
  ;
  indent = karma::space << karma::space;
  parameter_select %= parameter(second(*find(at_c<1>(_r1), at_c<1>(_val))));

#ifdef __clang__
#pragma clang diagnostic pop
#endif
}

} } }

#endif
