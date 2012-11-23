/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_OUTPUT_ITERATOR_HPP
#define MORBID_IIOP_OUTPUT_ITERATOR_HPP

#include <boost/spirit/home/karma.hpp>

namespace morbid { namespace iiop {

namespace spirit = boost::spirit;
namespace karma = spirit::karma;

template <typename O>
struct output_iterator;

template <typename I, typename T1, typename T2>
struct output_iterator<karma::detail::output_iterator<I, T1, T2> >
{
  typedef I type;
};

} }

#endif
