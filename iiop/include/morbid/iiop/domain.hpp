/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_DOMAIN_HPP
#define MORBID_IIOP_DOMAIN_HPP

#include <boost/spirit/home/support.hpp>
#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/qi.hpp>

namespace morbid { namespace iiop {

struct generator_compiler;

namespace karma = boost::spirit::karma;

struct generator_domain
{
  typedef generator_compiler compiler;

  template <typename Iterator, typename T1 = void, typename T2 = void, typename T3 = void, typename T4 = void>
  struct rule
  {
    typedef karma::rule<Iterator, T1, T2, T3, T4> type;
  };
  template <typename Iterator, typename T1, typename T2, typename T3>
  struct rule<Iterator, T1, T2, T3, void>
  {
    typedef karma::rule<Iterator, T1, T2, T3> type;
  };
  template <typename Iterator, typename T1, typename T2>
  struct rule<Iterator, T1, T2, void, void>
  {
    typedef karma::rule<Iterator, T1, T2> type;
  };
  template <typename Iterator, typename T1>
  struct rule<Iterator, T1, void, void, void>
  {
    typedef karma::rule<Iterator, T1> type;
  };
  template <typename Iterator>
  struct rule<Iterator, void, void, void, void>
  {
    typedef karma::rule<Iterator> type;
  };
};

struct parser_domain {};

} }

#endif

