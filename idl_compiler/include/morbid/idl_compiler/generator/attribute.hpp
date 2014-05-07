/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_ATTRIBUTE_HPP
#define MORBID_IDL_COMPILER_ATTRIBUTE_HPP

#include <morbid/idl_compiler/interface.hpp>
#include <morbid/idl_compiler/generator/indent.hpp>
#include <morbid/idl_compiler/generator/type_spec.hpp>
#include <morbid/idl_compiler/generator/return_lookuped.hpp>
#include <morbid/idl_parser/interface_def.hpp>

#include <boost/spirit/home/karma.hpp>

namespace morbid { namespace idlc { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
struct attribute : karma::grammar
  <OutputIterator
  , idl_parser::attribute(interface_, std::vector<idl_parser::wave_string>, idl_parser::wave_string)
  , karma::locals<unsigned int> >
{
  attribute();

  idlc::generator::indent<OutputIterator> indent;
  idlc::generator::type_spec<OutputIterator> type_spec;
  idlc::generator::return_lookuped<OutputIterator> return_;
  
  karma::rule<OutputIterator
              , idl_parser::attribute(interface_, std::vector<idl_parser::wave_string>, idl_parser::wave_string)
              , karma::locals<unsigned int> >
    start;
};

} } }

#endif
