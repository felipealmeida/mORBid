/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IIOP_GENERATOR_STRING_HPP
#define TECORB_IIOP_GENERATOR_STRING_HPP

#include <tecorb/iiop/generator/integer.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace tecorb { namespace iiop { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
struct string : karma::grammar<OutputIterator, std::string(bool)>
{
  string()
    : string::base_type(start)
  {
    namespace phoenix = boost::phoenix;
    using karma::_1;
    using karma::_r1;
    using karma::_val;

    start
      = dword(_r1)[_1 = phoenix::size(_val)+1]
      << karma::eps(_val[phoenix::size(_val)-1] != 0)
      << karma::string[_1 = _val]
      << karma::char_('\0')
      ;
    start.name("string");
    karma::debug(start);
  }

  generator::dword<OutputIterator> dword;
  karma::rule<OutputIterator, std::string(bool)> start;
};

} } }

#endif

