/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_GIOP_GRAMMARS_SYSTEM_EXCEPTION_REPLY_BODY_HPP
#define MORBID_GIOP_GRAMMARS_SYSTEM_EXCEPTION_REPLY_BODY_HPP

#include <morbid/giop/grammar.hpp>
#include <morbid/giop/grammars/service_context.hpp>

namespace morbid { namespace giop { namespace grammars {

template <typename Domain, typename Iterator, typename Attr>
struct system_exception_reply_body : grammar<Domain, Iterator, Attr(endian)>
{
  system_exception_reply_body() : system_exception_reply_body::base_type(start)
  {
    start =
      string        // exception_id
      & ulong_      // minor_code_value
      & ulong_      // completion_status
      ;
  }

  rule<Domain, Iterator, Attr(endian)> start;
};

} } }

#endif
