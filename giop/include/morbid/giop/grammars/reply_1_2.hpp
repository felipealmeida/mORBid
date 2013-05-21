/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_GIOP_GRAMMARS_REPLY_HEADER_1_2_HPP
#define MORBID_GIOP_GRAMMARS_REPLY_HEADER_1_2_HPP

#include <morbid/giop/grammar.hpp>
#include <morbid/giop/grammars/service_context.hpp>

namespace morbid { namespace giop { namespace grammars {

template <typename Domain, typename Iterator, typename Attr>
struct reply_header_1_2 : grammar<Domain, Iterator, Attr()>
{
  reply_header_1_2() : reply_header_1_2::base_type(start)
  {
    start =
      ulong_            // request_id
      & ulong_          // reply_status
      & sequence[service_context]
      ;
  }

  grammars::service_context<Domain, Iterator
                            , typename fusion::result_of::at_c<0u, Attr>::type
                            ()>
    service_context;
  rule<Domain, Iterator, Attr()> start;
};

} } }

#endif
