/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_OPERATION_IPP
#define TECORB_IDL_COMPILER_OPERATION_IPP

#include <morbid/idl_compiler/generator/operation.hpp>

#include <morbid/idl_compiler/stl_workarounds.hpp>
#include <morbid/idl_compiler/generator/wave_string.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/phoenix.hpp>
#include <boost/phoenix/fusion/at.hpp>

namespace morbid { namespace idlc { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
operation<OutputIterator>::operation()
  : operation::base_type(start)
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
     second(*find(at_c<1>(_r1), at_c<0>(_val))) // interface_.lookups[type_spec]
     , std::string("Dummy")
    )
    [_1 = at_c<0>(_val)]
    << karma::space << wave_string[_1 = at_c<1>(_val)]
    << "("
    << -((parameter_select(_r1) << " arg" << karma::lit(++_a)) % ", ")[_1 = at_c<2>(_val)]
    << ")" << eol
    << indent << "{" << eol
    << karma::eps[_a = 0]
    << (
        // indent << indent << "BOOST_MPL_ASSERT((boost::is_same<Dummy, int>));" << eol
        // <<
        indent << indent << "assert(!!_orb_);" << eol
        << operation_synchronous_call_exec(_r1, _r2, _r3)[_1 = _val]
       )
    << indent << "}" << eol
    ;

  // parameter_select %= parameter(at_c<1>(_r1)[at_c<1>(_val)]);
  parameter_select %= "typename ::morbid::lazy_eval< "
    << parameter(second(*find(at_c<1>(_r1), at_c<1>(_val)))) << ", Dummy>::type";

  karma::debug(start);

#ifdef __clang__
#pragma clang diagnostic pop
#endif
}

} } }

#endif