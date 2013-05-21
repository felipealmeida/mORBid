/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_EMPTY_REFERENCE_GENERATOR_IPP
#define TECORB_IDL_COMPILER_EMPTY_REFERENCE_GENERATOR_IPP

#include <morbid/idl_compiler/generator/empty_reference_generator.hpp>
#include <morbid/idl_parser/interface_def.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace std {

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, std::pair<T, U> p)
{
  return os << "[pair first: " << p.first << " second: " << p.second << ']';
}

}

namespace morbid { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
empty_reference_generator<OutputIterator>::empty_reference_generator()
  : empty_reference_generator::base_type(start)
{
  namespace phoenix = boost::phoenix;
  using karma::_1;
  using karma::_val;
  using karma::_a; using karma::_r1;
  using karma::eol;
  using phoenix::at_c;

  start = eol
    << "struct empty_reference" << eol
    << "{" << eol
    << indent << "// Start of operations defined in IDL" << eol
    << (*(operation(_r1) << eol))[_1 = at_c<1>(_val)]
    << indent << "// End of operations defined in IDL" << eol
    << "}" << eol << eol
    ;
  operation =
    indent
    << return_
    (
     at_c<1>(_r1)[at_c<0>(_val)] // interface_.lookups[type_spec]
    )
    [_1 = at_c<0>(_val)]
    << karma::space
    << wave_string[_1 = at_c<1>(_val)]
    << "("
    << -(parameter_select(_r1) % ", ")[_1 = at_c<2>(_val)]
    << ")" << eol
    << indent << "{" << eol
    << indent << indent << "assert(false && \"Called for empty reference, method: \\\""
    << wave_string[_1 = at_c<1>(_val)] << "\\\"\");" << eol
    << indent << "}" << eol
    << ";"
    ;
  parameter_select %= parameter(at_c<1>(_r1)[at_c<1>(_val)]);
  indent = karma::space << karma::space;
  wave_string %= karma::string;
    
  start.name("empty_reference_generator");
  operation.name("operation");
  karma::debug(start);
  karma::debug(operation);
}

} } }

#endif
