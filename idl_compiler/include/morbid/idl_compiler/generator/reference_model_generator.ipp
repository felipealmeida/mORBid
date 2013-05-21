/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_REFERENCE_MODEL_GENERATOR_IPP
#define TECORB_IDL_COMPILER_REFERENCE_MODEL_GENERATOR_IPP

#include <morbid/idl_compiler/generator/parameter.hpp>
#include <morbid/idl_compiler/generator/reference_model_generator.hpp>

#include <morbid/idl_parser/interface_def.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace std {

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, std::pair<T, U> p)
{
  return os << "[pair first: " << p.first << " second: " << p.second << ']';
}

template <typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> p)
{
  return os << "[vector size: " << p.size() << ']';
}

}

namespace morbid { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
header_reference_model_generator<OutputIterator>::header_reference_model_generator()
  : header_reference_model_generator::base_type(start)
{
  namespace phoenix = boost::phoenix;
  using karma::_1;
  using karma::_val; using karma::_r1; using karma::_r2; using karma::_r3;
  using karma::_a; using karma::eol;
  using phoenix::at_c;

  wave_string %= karma::string;
  class_name %= karma::string << "_ref";
  // specialization =
  //   (*(karma::skip[wave_string] << karma::lit("}")))[_1 = _r2] << eol
  //   << "namespace morbid {" << eol
  //   <<  "template <>" << eol
  //   << "struct is_remote_reference< "
  //   << (+("::" << wave_string))[_1 = _r1]
  //   << "_ref > : ::boost::mpl::true_ {};" << eol
  //   << "}" << eol
  //   << (*("namespace " << wave_string << " { "))[_1 = _r2] << eol
  //   ;
  start = 
    "struct "
    << class_name[_1 = at_c<0>(_val)]
    << "{" << eol
    << common_functions[_1 = _val]
    << indent << "// Start of operations defined in IDL" << eol
    << (*(operation(_r1, _r2, at_c<0>(_val)) << eol))[_1 = at_c<1>(_val)]
    << indent << "// End of operations defined in IDL" << eol
    << ior_function
    << "private:" << eol
    << common_members
    << "};" << eol << eol
    ;
  operation =
    indent
    << return_
    (
     at_c<1>(_r1)[at_c<0>(_val)] // interface_.lookups[type_spec]
    )
    [_1 = at_c<0>(_val)]
    << karma::space << wave_string[_1 = at_c<1>(_val)]
    << "("
    << -((parameter_select(_r1) << " arg" << karma::lit(++_a)) % ", ")[_1 = at_c<2>(_val)]
    << ")" << eol
    << indent << "{" << eol
    << karma::eps[_a = 0]
    << (
        // indent << indent << "std::cout << \"Called " << wave_string[_1 = phoenix::at_c<1>(_val)]
        // << " was called\" << std::endl;" << eol
        indent << indent << "return ::morbid::synchronous_call::call" << eol
        << indent << indent << indent << "< "
        << type_spec
        (
         at_c<1>(_r1)[at_c<0>(_val)] // interface_.lookups[type_spec]
        )
        [_1 = at_c<0>(_val)]
        // << (*(eol << indent << indent << indent << ", "
        //       << synchronous_template_args(_r1)))[_1 = at_c<2>(_val)]
        << eol << indent << indent << indent << ">" << eol
        << indent << indent << indent
        << "( _orb_, \"IDL:"
        << (-(wave_string % '/')[_1 = _r2] << '/') << wave_string[_1 = _r3] << ":1.0\""
        << ", \"" << wave_string[_1 = at_c<1>(_val)]
        << "\", _structured_ior_"
        << eol << indent << indent << indent << indent << ", "
        << "boost::fusion::vector"
        << eol << indent << indent << indent << indent << '<'
        << eol << indent << indent << indent << indent << indent
        << -(synchronous_template_args(_r1) % (eol << indent << indent << indent << indent << ", "))[_1 = at_c<2>(_val)]
        << eol << indent << indent << indent << indent << '>'
        << "("
        << -(synchronous_args(_r1, ++_a) % (eol << indent << indent << indent << indent << ", "))[_1 = at_c<2>(_val)]
        << "));" << eol
       )
    << indent << "}" << eol
    ;
  parameter_select %= parameter(at_c<1>(_r1)[at_c<1>(_val)]);
  type_spec_select %= type_spec(at_c<1>(_r1)[_val]);
  in_tag = wave_string[_1 = "::morbid::type_tag::in_tag"];
  out_tag = wave_string[_1 = "::morbid::type_tag::out_tag"];
  inout_tag = wave_string[_1 = "::morbid::type_tag::inout_tag"];
  synchronous_template_args = 
    wave_string[_1 = "::morbid::type_tag::value_type_tag< "]
    << type_spec_select(_r1)[_1 = at_c<1>(_val)]
    << ", " << (in_tag | out_tag | inout_tag)[_1 = at_c<0>(_val)]
    << ">"
    ;
  synchronous_args %=
    synchronous_template_args(_r1)
    << "(arg" << karma::lit(_r2) << ")";
  
  common_functions =
    indent
    << "// Constructors" << eol
    << indent << 
       (
        class_name[_1 = at_c<0>(_val)]
        << "( ::morbid::orb orb, ::morbid::structured_ior const& structured_ior)" << eol
        << indent << indent << " : _orb_(orb), _structured_ior_(structured_ior)" << eol
        << indent << "{}" << eol
       )
    << indent << 
      (
        class_name[_1 = at_c<0>(_val)]
        << "( ::morbid::orb orb, std::string const& ior)" << eol
        << indent << indent << " : _orb_(orb), _structured_ior_( ::morbid::string_to_structured_ior(ior.begin(), ior.end()))" << eol
        << indent << "{}" << eol
       )
    // << indent << "~" << class_name[_1 = _val] << "();" << eol
    ;

  common_members =
    indent
    << "// Members" << eol
    << indent << "::morbid::orb _orb_;" << eol
    << indent << "::morbid::structured_ior _structured_ior_;" << eol
    // << indent << "static const char* _repository_id;" << eol
    ;
  indent = karma::space << karma::space;
  ior_function =
    indent << "::morbid::structured_ior _structured_ior() const { return _structured_ior_; }" << eol
    ;

  start.name("header_reference_model_generator");
  karma::debug(start);
}

} } }

#endif
