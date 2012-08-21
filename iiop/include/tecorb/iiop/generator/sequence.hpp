/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IIOP_GENERATOR_SEQUENCE_HPP
#define TECORB_IIOP_GENERATOR_SEQUENCE_HPP

#include <tecorb/iiop/generator/integer.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace tecorb { namespace iiop { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator, typename T>
struct sequence : karma::grammar<OutputIterator, std::vector<T>(bool)>
{
  sequence()
    : sequence::base_type(start)
  {
    namespace phoenix = boost::phoenix;
    using karma::_r1;
    using karma::_1;
    using karma::_val;

    start
      = karma::omit[dword(true)[_1 = phoenix::size(_val)]]
      << (*karma::char_)[_1 = _val]
      ;
    start.name("sequence");
    karma::debug(start);
  }

  generator::dword<OutputIterator> dword;
  karma::rule<OutputIterator, std::vector<T>(bool)> start;
};

} } }

#endif
