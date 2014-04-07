/* (c) Copyright 2014 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_TYPEDEF_GENERATOR_IPP
#define MORBID_IDL_COMPILER_TYPEDEF_GENERATOR_IPP

#include <morbid/idl_compiler/generator/typedef_generator.hpp>
#include <morbid/idl_compiler/generator/wave_string.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/phoenix.hpp>
#include <boost/phoenix/fusion/at.hpp>

namespace morbid { namespace idlc { namespace generator {

namespace karma = boost::spirit::karma;
namespace phoenix = boost::phoenix;

template <typename OutputIterator, typename Iterator>
typedef_generator<OutputIterator, Iterator>::typedef_generator()
  : typedef_generator::base_type(start)
{
  using phoenix::at_c;
  using karma::_r1;
  using karma::_val;
  using karma::_1;

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsequenced"
#endif

  start =
     "typedef "
    <<
    (
      (
       karma::eps(at_c<2>(_val))
       << "::boost::array< " << type_spec(_r1)[_1 = at_c<0>(_val)]
       << ", " << karma::uint_[_1 = *at_c<2>(_val)]
       << ">"
      )
      |
      type_spec(_r1)[_1 = at_c<0>(_val)]
     )
     << karma::space
     << wave_string[_1 = at_c<1>(_val)]
     << ';' << karma::eol
    ;

  start.name("typedef");
  karma::debug(start);

#ifdef __clang__
#pragma clang diagnostic pop
#endif
}

} } }

#endif
