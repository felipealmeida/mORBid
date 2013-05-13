/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_PROXY_REFERENCE_GENERATOR_IPP
#define TECORB_IDL_COMPILER_PROXY_REFERENCE_GENERATOR_IPP

#include <morbid/idl_compiler/generator/proxy_reference_generator.hpp>
#include <morbid/idl_parser/interface_def.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace morbid { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator, typename Iterator>
proxy_reference_generator<OutputIterator, Iterator>::proxy_reference_generator()
  : proxy_reference_generator::base_type(start)
{
  namespace phoenix = boost::phoenix;
  using karma::_1;
  using karma::_val;
  using karma::_a; using karma::_r1;
  using karma::eol;
  using phoenix::at_c;

  start = eol
    << "template <typename T>" << eol
    << "struct proxy_reference" << eol
    << "{" << eol
    << indent << "proxy_reference(boost::reference_wrapper<T> ref) : _ptr(&boost::unwrap_ref(ref)) {}" << eol
    << indent << "// Start of operations defined in IDL" << eol
    << (*(operation(_r1) << eol))[_1 = at_c<1>(_val)]
    << indent << "// End of operations defined in IDL" << eol
    << indent << "T* _ptr;" << eol
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
    << -((parameter_select(_r1) << " arg" << karma::lit(++_a)) % ", ")[_1 = at_c<2>(_val)]
    << ")" << eol
    << indent << "{" << eol
    << indent << indent << "return _ptr->" << wave_string[_1 = at_c<1>(_val)]
    << "("
    << karma::eps[_a = 0]
    << -(synchronous_args(++_a) % (eol << indent << indent << indent << indent << ", "))[_1 = at_c<2>(_val)]
    << ");"
    << eol
    << indent << "}" << eol
    << ";"
    ;
  synchronous_args %= "arg" << karma::lit(_r1);
  parameter_select %= parameter(at_c<1>(_r1)[at_c<1>(_val)]);
  indent = karma::space << karma::space;
  wave_string %= karma::string;

  start.name("proxy_reference_generator");
  operation.name("operation");
  karma::debug(start);
  karma::debug(operation);
}

} } }

#endif
