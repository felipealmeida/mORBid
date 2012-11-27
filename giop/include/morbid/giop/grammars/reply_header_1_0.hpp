/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_GIOP_GRAMMARS_REPLY_HEADER_1_0_HPP
#define MORBID_GIOP_GRAMMARS_REPLY_HEADER_1_0_HPP

#include <morbid/giop/grammar.hpp>
#include <morbid/giop/grammars/service_context.hpp>

namespace morbid { namespace giop { namespace grammars {

template <typename Domain, typename Iterator, typename Attr>
struct reply_header_1_0 : grammar<Domain, Iterator, Attr(endian)>
{
  reply_header_1_0() : reply_header_1_0::base_type(start)
  {
    start =
      sequence[service_context]
      & ulong_          // request_id
      & ulong_          // reply_status
      ;
  }

  grammars::service_context<Domain, Iterator
                            , typename boost::remove_reference
                            <typename fusion::result_of::at_c<Attr, 0u>::type>::type>
    service_context;
  rule<Domain, Iterator, Attr(endian)> start;
};

} } }

#endif
