/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_GIOP_GRAMMARS_SERVICE_CONTEXT_HPP
#define MORBID_GIOP_GRAMMARS_SERVICE_CONTEXT_HPP

#include <morbid/giop/grammar.hpp>

namespace morbid { namespace giop { namespace grammars {

template <typename Domain, typename Iterator, typename Attr>
struct service_context : grammar<Domain, Iterator, Attr()>
{
  service_context() : service_context::base_type(start)
  {
    start = ulong_ & sequence[octet]
      ;
  }

  rule<Domain, Iterator, Attr()> start;
};

} } }

#endif
