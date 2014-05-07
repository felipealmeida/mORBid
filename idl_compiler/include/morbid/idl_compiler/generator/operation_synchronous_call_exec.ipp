/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_OPERATION_IPP
#define TECORB_IDL_COMPILER_OPERATION_IPP

#include <morbid/idl_compiler/generator/operation_synchronous_call_exec.hpp>

#include <morbid/idl_compiler/stl_workarounds.hpp>
#include <morbid/idl_compiler/generator/wave_string.hpp>
#include <morbid/idl_compiler/generator/type_spec.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/phoenix.hpp>
#include <boost/phoenix/fusion/at.hpp>

namespace morbid { namespace idlc { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
operation_synchronous_call_exec<OutputIterator>::operation_synchronous_call_exec()
  : operation_synchronous_call_exec::base_type(start)
{
  namespace phoenix = boost::phoenix;
  using karma::_1;
  using karma::_val;
  using karma::_r1;
  using karma::_r2;
  using karma::_r3;
  using karma::_a;
  using karma::eol;

  using phoenix::at_c;
  using phoenix::second;
  using phoenix::find;

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsequenced"
#endif
  start =
    indent << indent << "return ::morbid::synchronous_call::call" << eol
    << indent << indent << indent << "< "
    << type_spec
    (
      second(*find(at_c<1>(_r1), at_c<0>(_val))) // interface_.lookups[type_spec]
    )
    [_1 = at_c<0>(_val)]
    << eol << indent << indent << indent << ">" << eol
    << indent << indent << indent
    << "( *_orb_, \"IDL:"
    << -((wave_string % '/')[_1 = _r2] << '/') << wave_string[_1 = _r3] << ":1.0\""
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
    ;

  type_spec_select %= type_spec(second(*find(at_c<1>(_r1), _val)));

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

  karma::debug(start);

#ifdef __clang__
#pragma clang diagnostic pop
#endif
}

} } }

#endif
