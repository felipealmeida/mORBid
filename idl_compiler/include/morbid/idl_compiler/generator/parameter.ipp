/* (c) Copyright 2014 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_PARAMETER_IPP
#define MORBID_IDL_COMPILER_PARAMETER_IPP

#include <morbid/idl_compiler/generator/parameter.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/phoenix.hpp>
#include <boost/phoenix/fusion/at.hpp>

namespace morbid { namespace idlc { namespace generator {

namespace karma = boost::spirit::karma;
namespace phoenix = boost::phoenix;

template <typename OutputIterator>
parameter<OutputIterator>::parameter()
  : parameter::base_type(start, "parameter")
{
  using karma::_1;
  using karma::_val;
  using karma::_r1; using karma::_r2;
  using phoenix::at_c;
  namespace types = idl_parser::types;

  in_traits = karma::string[_1 = "::morbid::in_traits< "];
  out_traits = karma::string[_1 = "::morbid::out_traits< "];
  inout_traits = karma::string[_1 = "::morbid::inout_traits< "];
  start %= 
    (out_traits | in_traits | inout_traits)
    << type_spec(_r1)
    << ">::type"
    ;

  karma::debug(start);
}

} } }

#endif
