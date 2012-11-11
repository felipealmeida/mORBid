/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_TYPE_TAG_HPP
#define TECORB_TYPE_TAG_HPP

#include <morbid/in_out_traits.hpp>

namespace morbid { namespace type_tag {

struct in_tag {};
struct out_tag {};
struct inout_tag {};

template <typename T, typename Tag>
struct value_type_tag
{
  typedef T type;
  typedef Tag tag;

  value_type_tag(T value) : value(value) {}

  T value;
};

} }

#endif
