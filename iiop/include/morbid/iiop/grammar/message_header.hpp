/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_GRAMMAR_MESSAGE_HEADER_HPP
#define MORBID_IIOP_GRAMMAR_MESSAGE_HEADER_HPP

#include <morbid/iiop/message_header.hpp>

#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace morbid { namespace iiop { namespace grammar {

namespace qi = boost::spirit::qi;

template <typename Iterator>
struct message_header : qi::grammar<Iterator, iiop::message_header()
                      , qi::locals<char> >
{
  message_header()
    : message_header::base_type(start)
  {
    using qi::_a;
    using qi::_1;
    using qi::_val;

    start %=
      qi::lit("GIOP")
      >> qi::char_  /* major */
      >> qi::char_  /* minor */
      >> qi::char_[_a = _1] /* if little endian */
      >> qi::char_  /* flags */
      >> (
          (qi::eps((_a & 1) == 1) >> qi::little_dword) // little endian
          | qi::big_dword // big-endian
         ) /* message size */
      ;
  }

  qi::rule<Iterator, iiop::message_header()
           , qi::locals<char> > start;
};

} } }

#endif
