/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_REFERENCE_MODEL_GENERATOR_IPP
#define TECORB_IDL_COMPILER_REFERENCE_MODEL_GENERATOR_IPP

#include <morbid/idl_compiler/generator/reference_model_generator.hpp>

#include <morbid/idl_compiler/stl_workarounds.hpp>
#include <morbid/idl_compiler/generator/wave_string.hpp>

#include <morbid/idl_parser/interface_def.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/phoenix.hpp>
#include <boost/phoenix/fusion/at.hpp>

namespace morbid { namespace idlc { namespace generator {

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
  using phoenix::second;
  using phoenix::find;

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsequenced"
#endif
  class_name %= wave_string << "_ref_impl";
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
    "template <typename Dummy> struct "
    << class_name[_1 = at_c<0>(_val)]
    << -karma::buffer[(" : " << (base_spec(_r1) % ", ") [_1 = phoenix::at_c<6>(_val)])]
    << "{" << eol
    << common_functions(_r1)[_1 = _val]
    << indent << "// Start of operations defined in IDL" << eol
    << (*(operation(_r1, _r2, at_c<0>(_val)) << eol))[_1 = at_c<1>(_val)]
    << indent << "// End of operations defined in IDL" << eol
    << indent << "// Start of attributes defined in IDL" << eol
    << (*(attribute(_r1, _r2, at_c<0>(_val)) << eol))[_1 = at_c<2>(_val)]
    << indent << "// End of attributes defined in IDL" << eol
    << ior_function
    << indent << "typedef " << wave_string[_1 = at_c<0>(_val)] << "_concept concept_class;" << eol
    << "private:" << eol
    << common_members
    << "};" << eol << eol
    ;

  base_spec %=
    type_spec(second(*find(at_c<1>(_r1),_val)))
    << "_ref"
    ;

  common_functions =
    indent
    << "// Constructors" << eol
    << indent << 
       (
        class_name[_1 = at_c<0>(_val)] << "()" << eol
        << indent << "{}" << eol
       )
    << indent << 
       (
        class_name[_1 = at_c<0>(_val)]
        << "( ::morbid::orb orb, ::morbid::structured_ior const& structured_ior)" << eol
        << indent << indent << " : "
        << -karma::buffer
        [
         ((base_spec(_r1) << "(orb, structured_ior)") % ", ") [_1 = phoenix::at_c<6>(_val)] << "," << eol
        ]
        << indent << indent << " _orb_(orb), _structured_ior_(structured_ior)" << eol
        << indent << "{}" << eol
       )
    << indent << 
      (
        class_name[_1 = at_c<0>(_val)]
        << "( ::morbid::orb orb, std::string const& ior)" << eol
        << indent << indent << " : "
        << -karma::buffer
        [
         ((base_spec(_r1) << "(orb, ior)") % ", ") [_1 = phoenix::at_c<6>(_val)] << "," << eol
        ]
        << indent << indent << "_orb_(orb), _structured_ior_( ::morbid::string_to_structured_ior(ior.begin(), ior.end()))" << eol
        << indent << "{}" << eol
       )
    ;

  common_members =
    indent
    << "// Members" << eol
    << indent << "::boost::optional< ::morbid::orb> _orb_;" << eol
    << indent << "::morbid::structured_ior _structured_ior_;" << eol
    // << indent << "static const char* _repository_id;" << eol
    ;

  ior_function =
    indent
    << "::morbid::structured_ior _structured_ior() const { return _structured_ior_; }" << eol
    << indent << "::boost::optional< ::morbid::orb> _orb() const { return _orb_; }" << eol
    ;

  start.name("header_reference_model_generator");
  karma::debug(start);

#ifdef __clang__
#pragma clang diagnostic pop
#endif
}

} } }

#endif
