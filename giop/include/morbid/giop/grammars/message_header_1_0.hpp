/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_GIOP_GRAMMARS_MESSAGE_HEADER_HPP
#define MORBID_GIOP_GRAMMARS_MESSAGE_HEADER_HPP

#include <morbid/giop/grammar.hpp>

namespace morbid { namespace giop { namespace grammars {

template <typename Domain, typename Iterator, typename Attr>
struct message_header_1_0 : grammar<Domain, Iterator, Attr()>
{
  template <typename Body>
  message_header_1_0(Body const& body)
    : message_header_1_0::base_type(start)
  {
    init(body);
  }

  message_header_1_0()
    : message_header_1_0::base_type(start)
  {
    init(boost::spirit::eps);
  }

  template <typename Body>
  void init(Body const& body)
  {
    start =
      "GIOP"
      & octet           /* GIOP major version */
      & octet           /* GIOP minor version */
      & endianness
      [
       octet            /* flags */
       & ulong_         /*  message size */
       & body
      ]
      ;
  }

  rule<Domain, Iterator, Attr()> start;
};

} } }

#endif
