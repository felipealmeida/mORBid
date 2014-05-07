/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_ATTRIBUTE_IPP
#define TECORB_IDL_COMPILER_ATTRIBUTE_IPP

#include <morbid/idl_compiler/generator/attribute.hpp>

#include <morbid/idl_compiler/stl_workarounds.hpp>
#include <morbid/idl_compiler/generator/wave_string.hpp>

#include <morbid/idl_parser/interface_def.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/phoenix.hpp>
#include <boost/phoenix/fusion/at.hpp>

namespace morbid { namespace idlc { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
attribute<OutputIterator>::attribute()
: attribute::base_type(start)
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
    indent
    << return_
    (
     second(*find(at_c<1>(_r1), at_c<1>(_val)))
     , std::string("Dummy")
    )[_1 = at_c<1>(_val)]
    << karma::space << "_get_" << wave_string[_1 = at_c<2>(_val)] << "() const" << eol
    << indent << "{" << eol
    << (
        indent << indent << "assert(!!_orb_);" << eol
        << indent << indent << "return ::morbid::synchronous_call::call" << eol
        << indent << indent << indent << "< "
        << type_spec
        (
         second(*find(at_c<1>(_r1), at_c<1>(_val))) // interface_.lookups[type_spec]
        )
        [_1 = at_c<1>(_val)]
        << eol << indent << indent << indent << ">" << eol
        << indent << indent << indent
        << "( *_orb_, \"IDL:"
        << -((wave_string % '/')[_1 = _r2] << '/') << wave_string[_1 = _r3] << ":1.0\""
        << ", \"_get_" << wave_string[_1 = at_c<2>(_val)]
        << "\", _structured_ior_"
        << eol << indent << indent << indent << indent << ", "
        << "boost::fusion::vector0<>());" << eol
       )
    << indent << "}" << eol
    ;

  karma::debug(start);

#ifdef __clang__
#pragma clang diagnostic pop
#endif
}

} } }

#endif
