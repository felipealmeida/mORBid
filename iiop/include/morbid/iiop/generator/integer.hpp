/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_GENERATOR_INTEGER_HPP
#define MORBID_IIOP_GENERATOR_INTEGER_HPP

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace morbid { namespace iiop { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
struct word : karma::grammar<OutputIterator, boost::uint_t<16u>::least(bool)>
{
  word() : word::base_type(start)
  {
    using karma::_r1;
    start %= (karma::eps(_r1) << karma::little_word)
      | karma::big_word;
    start.name("word");
    karma::debug(start);
  }

  karma::rule<OutputIterator, boost::uint_t<16u>::least(bool)> start;
};

template <typename OutputIterator>
struct dword : karma::grammar<OutputIterator, boost::uint_t<32u>::least(bool)>
{
  dword() : dword::base_type(start)
  {
    using karma::_r1;
    start %= (karma::eps(_r1) << karma::little_dword)
      | karma::big_dword;
    start.name("dword");
    karma::debug(start);
  }

  karma::rule<OutputIterator, boost::uint_t<32u>::least(bool)> start;
};

template <typename OutputIterator>
struct qword : karma::grammar<OutputIterator, boost::uint_t<64u>::least(bool)>
{
  qword() : qword::base_type(start)
  {
    using karma::_r1;
    start %= (karma::eps(_r1) << karma::little_qword)
      | karma::big_qword;
    start.name("qword");
    karma::debug(start);
  }

  karma::rule<OutputIterator, boost::uint_t<64u>::least(bool)> start;
};

} } }

#endif
