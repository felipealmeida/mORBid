/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_CONCEPT_GENERATOR_IPP
#define TECORB_IDL_COMPILER_CONCEPT_GENERATOR_IPP

// #include <morbid/idl_compiler/generator/empty_reference_generator.ipp>
#include <morbid/idl_compiler/generator/proxy_reference_generator.ipp>
#include <morbid/idl_compiler/generator/concept_generator.hpp>

#include <morbid/idl_parser/interface_def.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/phoenix.hpp>

// namespace std {

// template <typename T, typename U>
// std::ostream& operator<<(std::ostream& os, std::pair<T, U> p)
// {
//   return os << "[pair first: " << p.first << " second: " << p.second << ']';
// }

// }

namespace morbid { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
header_concept_generator<OutputIterator>::header_concept_generator()
  : header_concept_generator::base_type(start)
{
  namespace phoenix = boost::phoenix;
  using karma::_1;
  using karma::_val;
  using karma::_a;
  using karma::_r1; using karma::_r2; using karma::_r3;
  using karma::eol;
  using phoenix::at_c;

  concept_class =
    karma::eps[_a = at_c<0>(_val)]
    << eol
    << "struct " << wave_string[_1 = _a] << "_concept" << eol
    << eol << "{" << eol
    << indent << "// Start of operations defined in IDL" << eol
    << (*(operation(_r1) << eol))
    [_1 = at_c<1>(_val)]
    << indent << "// End of operations defined in IDL" << eol
    << requirements[_1 = at_c<1>(_val)] << eol
    << public_members(_r2, _a)[_1 = at_c<0>(_val)] << eol
    << indent << "typedef ::morbid::interface_tag _morbid_type_kind;" << eol
    // << indent << empty_reference(_r1)[_1 = _val] << ";" << eol
    // << indent << proxy_reference(_r1)[_1 = _val] << ";" << eol
    << indent << "template <typename T> struct proxy_reference;" << eol
    << indent << "typedef " << wave_string[_1 = _a] << "_ref remote_reference;" << eol
    << "};" << eol << eol
    ;
  start =
    karma::eps[_a = at_c<0>(_val)]
    << "struct " << wave_string[_1 = _a] << "_concept;" << eol
    << "struct " << wave_string[_1 = _a] << "_ref;" << eol
    << "typedef ::morbid::reference< " << wave_string[_1 = _a] << "_concept> " << wave_string[_1 = _a] << ';' << eol
    << concept_class(_r1, _r2)[_1 = _val]
    ;
  operation_name = karma::stream[_1 = at_c<1>(_val)];
  args = "arg" << karma::lit(_r1);
  in_tag = karma::string[_1 = " ::morbid::type_tag::in_tag"];
  out_tag = karma::string[_1 = " ::morbid::type_tag::out_tag"];
  inout_tag = karma::string[_1 = " ::morbid::type_tag::inout_tag"];
  arguments = " ::morbid::type_tag::value_type_tag< "
    << type_spec
    (
      at_c<1>(_r1)[at_c<1>(_val)]
    )
    [_1 = at_c<1>(_val)]
    << " , " << (in_tag | out_tag | inout_tag)[_1 = at_c<0>(_val)]
    << " >"
    ;
  operation =
    -(
      // karma::eps(at_c<3>(_val))
      indent << "template <typename C = ::boost::type_erasure::_self, typename R = "
      << return_
      (
       at_c<1>(_r1)[at_c<0>(_val)]
      )
      [_1 = at_c<0>(_val)]
      << (*(", typename A" << karma::lit(_a++) << " = "
            << parameter_select(_r1)))[_1 = at_c<2>(_val)]
      << " >" << eol
      << indent << "struct " << operation_name[_1 = _val] << eol
      << indent << '{' << eol
      << indent << indent << "typedef R result_type;" << eol
      // apply
      << indent << indent << "static result_type apply(C& self"
      << karma::eps[_a = 0]
      << -(
           (*(", A"
              << karma::lit(_a)
              << karma::attr_cast<idl_parser::param_decl>(karma::eps)
              << " arg" << karma::lit(++_a)))[_1 = at_c<2>(_val)]
          )
      << ")" << eol
      << indent << indent << "{" << eol
      << indent << indent << indent << "return self." << karma::stream[_1 = at_c<1>(_val)]
      << "(" << karma::eps[_a = 0] << -(args(++_a) % ", ")[_1 = at_c<2>(_val)] << ");" << eol
      << indent << indent << "}" << eol
      // operator()
      << karma::eps[_a = 0]
      << indent << indent << "template <typename This_>" << eol
      << indent << indent << "result_type operator()(This_* self"
      << -(
           (*(", A"
              << karma::lit(_a)
              << karma::attr_cast<idl_parser::param_decl>(karma::eps)
              << " arg" << karma::lit(++_a)))[_1 = at_c<2>(_val)]
           //           (*(", " << parameter_select(_r1) << " arg" << karma::lit(++_a)))[_1 = at_c<2>(_val)]
          )
      << ")" << eol
      << indent << indent << "{" << eol
      << indent << indent << indent << "return self->" << karma::stream[_1 = at_c<1>(_val)]
      << "(" << karma::eps[_a = 0] << -(args(++_a) % ", ")[_1 = at_c<2>(_val)] << ");" << eol
      << indent << indent << "}" << eol
      // end of operator()
      << indent << indent << "inline static const char* name() { return \"" << operation_name[_1 = _val] << "\"; }" << eol
      << indent << indent << "typedef ::boost::mpl::vector< "
      << -(arguments(_r1) % ", ")[_1 = at_c<2>(_val)] << " > arguments;" << eol
      << indent << "};" << eol
     )
    ;
  requirements = indent
    << "typedef boost::mpl::vector< "
    << -((operation_name << "<>") % ", ")[_1 = _val]
    << " >" << eol
    << indent << indent << "requirements;" << eol
    << indent
    << "typedef boost::mpl::vector< ::boost::type_erasure::copy_constructible<>, ::boost::type_erasure::relaxed"
    << (*(", " << operation_name << "<>"))[_1 = _val]
    << " >" << eol
    << indent << indent << "regular_requirements;" << eol
    ;
  parameter_select %= parameter(at_c<1>(_r1)[at_c<1>(_val)]);

  public_members = 
    indent
    << "inline static const char* type_id() { return \"IDL:"
    << (*(wave_string << '/'))[_1 = _r1] << wave_string[_1 = _r2] << ":1.0\"; }" << eol
    ;
  indent = karma::space << karma::space;
  wave_string %= karma::string;
}

} } }

#endif
