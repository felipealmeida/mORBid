/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_TYPEDEF_GENERATOR_HPP
#define MORBID_IDL_COMPILER_TYPEDEF_GENERATOR_HPP

#include <morbid/idl_compiler/module.hpp>
#include <morbid/idl_compiler/generator/type_spec.hpp>
#include <morbid/idl_compiler/generator/scoped_name.hpp>

#include <boost/spirit/home/karma.hpp>

#include <string>
#include <ostream>
#include <vector>

namespace morbid { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator, typename Iterator>
struct typedef_generator : karma::grammar
  <OutputIterator, idl_parser::typedef_def(lookuped_type)>
{
  typedef_generator()
    : typedef_generator::base_type(start)
  {
    using phoenix::at_c;
    using karma::_r1;
    using karma::_val;
    using karma::_1;

    wave_string = karma::string;
    start =
      "typedef" << karma::space
                << type_spec(_r1)[_1 = at_c<0>(_val)]
                << karma::space
                << wave_string[_1 = at_c<1>(_val)]
                << ';' << karma::eol
      ;

    // start.name("typedef");
    // karma::debug(start);
  }

  generator::type_spec<OutputIterator, Iterator> type_spec;
  karma::rule<OutputIterator, idl_parser::wave_string()> wave_string;
  karma::rule<OutputIterator, idl_parser::typedef_def(lookuped_type)> start;
};

} } }

#endif
