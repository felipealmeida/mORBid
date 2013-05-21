/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_GIOP_DETAIL_PARAMETERIZED_HPP
#define MORBID_GIOP_DETAIL_PARAMETERIZED_HPP

namespace morbid { namespace giop {

template <typename Domain, typename Subject, typename Params>
struct parameterized_nonterminal
{
  parameterized_nonterminal(Subject const& subject, Params const& params)
    : ref(subject), params(params)
  {
  }

  typedef Domain domain_type;

  boost::reference_wrapper<Subject const> ref;
  Params params;
};

} }

#endif
