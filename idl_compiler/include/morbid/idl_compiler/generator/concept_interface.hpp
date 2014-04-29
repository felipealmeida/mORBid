/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_CONCEPT_INTERFACE_HPP
#define MORBID_IDL_COMPILER_CONCEPT_INTERFACE_HPP

#include <morbid/idl_parser/interface_def.hpp>
#include <morbid/idl_compiler/generator/proxy_reference_generator.hpp>
#include <morbid/idl_compiler/generator/parameter.hpp>
#include <morbid/idl_compiler/generator/return.hpp>
#include <morbid/idl_compiler/generator/type_spec.hpp>
#include <morbid/idl_compiler/interface.hpp>

#include <boost/spirit/home/karma.hpp>

#include <string>
#include <ostream>
#include <vector>

namespace morbid { namespace idlc { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
struct concept_interface : karma::grammar
 <OutputIterator, idl_parser::interface_def(interface_, std::vector<idl_parser::wave_string>)>
{
  concept_interface();
//   : concept_interface::base_type(start)
//   {
//     namespace phoenix = boost::phoenix;
//     using karma::eol;
//     using karma::_1; using karma::_val;
//     using karma::_r1; using karma::_r2; using karma::_r3;
//     using phoenix::at_c;
//     using karma::_a;

// #pragma clang diagnostic push
// #pragma clang diagnostic ignored "-Wunsequenced"

//     wave_string %= karma::string;
//     operation_name = wave_string[_1 = at_c<1>(_val)];
//     attribute_name = "_get_" << wave_string[_1 = at_c<2>(_val)];
//     operation_concept_interface_specialization =
//       "template <class C, class Base>" 
//       << eol
//       << "struct concept_interface< " << -(wave_string % "::")[_1 = _r2]
//       << "::" << wave_string[_1 = _r3] << "_concept::" << operation_name[_1 = _val] << "<C>, Base, C> : Base" << eol
//       << "{" << eol
//       << indent << " typename ::morbid::lazy_eval< "
//       << return_(at_c<1>(_r1)[at_c<0>(_val)])[_1 = at_c<0>(_val)]
//       << ", C>::type" << eol
//       << karma::space << operation_name[_1 = _val]
//       << '('
//       << -(
//            (("typename as_param<Base, " << parameter_select(_r1)
//              << " >::type arg" << karma::lit(++_a)) % ", ")[_1 = at_c<2>(_val)]
//           )
//       << ')' << eol
//       << indent << "{" << eol
//       << indent << indent << "return call( typename ::morbid::lazy_eval< " << -(wave_string % "::")[_1 = _r2]
//       << "::" << wave_string[_1 = _r3] << "_concept::" << operation_name[_1 = _val] << "<C>, C>::type(), *this"
//       << karma::eps[_a = 0]
//       << (*(", " << karma::attr_cast<idl_parser::param_decl>(karma::eps) << "arg" << karma::lit(++_a)))[_1 = at_c<2>(_val)]
//       << ");" << eol
//       << indent << "}" << eol
//       << "};" << eol
//       ;
//     attribute_concept_interface_specialization =
//       "template <class C, class Base>" 
//       << eol
//       << "struct concept_interface< " << -(wave_string % "::")[_1 = _r2]
//       << "::" << wave_string[_1 = _r3] << "_concept::" << attribute_name[_1 = _val] << "<C>, Base, C> : Base" << eol
//       << "{" << eol
//       << indent << " typename ::morbid::lazy_eval< "
//       << return_(at_c<1>(_r1)[at_c<1>(_val)])[_1 = at_c<1>(_val)]
//       << ", C>::type" << eol
//       << karma::space << attribute_name[_1 = _val]
//       << '('
//       << ") const" << eol
//       << indent << "{" << eol
//       << indent << indent << "return call( typename ::morbid::lazy_eval< " << -(wave_string % "::")[_1 = _r2]
//       << "::" << wave_string[_1 = _r3] << "_concept::" << attribute_name[_1 = _val] << "<C>, C>::type(), *this"
//       << ");" << eol
//       << indent << "}" << eol
//       << "};" << eol
//       ;
//     start =
//       (*operation_concept_interface_specialization(_r1, _r2, at_c<0>(_val)))[_1 = at_c<1>(_val)]
//       << (*attribute_concept_interface_specialization(_r1, _r2, at_c<0>(_val)))[_1 = at_c<2>(_val)]
//     ;
//     indent = karma::space << karma::space;
//     parameter_select %= parameter(at_c<1>(_r1)[at_c<1>(_val)]);

// #pragma clang diagnostic pop
//   }

  idlc::generator::type_spec<OutputIterator> type_spec;
  idlc::generator::parameter<OutputIterator> parameter;
  idlc::generator::return_<OutputIterator> return_;
  karma::rule<OutputIterator, idl_parser::param_decl(interface_)> parameter_select;
  karma::rule<OutputIterator> indent;
  karma::rule<OutputIterator
              , idl_parser::op_decl()> operation_name;
  karma::rule<OutputIterator
              , idl_parser::attribute()> attribute_name;
  karma::rule<OutputIterator
              , idl_parser::op_decl(interface_, std::vector<idl_parser::wave_string>, idl_parser::wave_string)
              , karma::locals<unsigned int> > operation_concept_interface_specialization;
  karma::rule<OutputIterator
              , idl_parser::attribute(interface_, std::vector<idl_parser::wave_string>, idl_parser::wave_string)
              , karma::locals<unsigned int> > attribute_concept_interface_specialization;
  karma::rule<OutputIterator
              , idl_parser::interface_def(interface_, std::vector<idl_parser::wave_string>)> start;
};

} } }

#endif
