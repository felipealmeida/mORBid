/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IIOP_GENERATOR_MESSAGE_HEADER_HPP
#define TECORB_IIOP_GENERATOR_MESSAGE_HEADER_HPP

#include <tecorb/iiop/message_header.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace tecorb { namespace iiop { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
struct message_header : karma::grammar<OutputIterator, void(bool, char, unsigned int)>
{
  message_header()
    : message_header::base_type(start)
  {
    using karma::_r1;
    using karma::_r2;
    using karma::_r3;

    start =
      karma::lit("GIOP")
      << '\1' << '\0'
      << (
          (
           karma::eps(_r1) << '\1'
           << karma::char_(_r2)
           << karma::little_dword(_r3 - 12)
          )
          | '\0'
          << karma::char_(_r2)
          << karma::big_dword(_r3 - 12)
         )
      ;
  }

  karma::rule<OutputIterator, void(bool, char, unsigned int)> start;
};

} } }

#endif
