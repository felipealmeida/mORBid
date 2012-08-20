/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_TYPE_TAG_HPP
#define TECORB_TYPE_TAG_HPP

namespace tecorb { namespace type_tag {

struct in_tag {};
struct out_tag {};
struct inout_tag {};

template <typename T>
struct in_type_traits
{
  typedef T type;
};

template <>
struct in_type_traits<char*>
{
  typedef const char* type;
};

template <typename T, typename Tag>
struct value_type_tag;

template <typename T>
struct value_type_tag<T, in_tag>
{
  typedef T type;
  typedef in_tag tag;

  value_type_tag(typename in_type_traits<type>::type value) : value(value) {}

  typename in_type_traits<type>::type value;
};

} }

#endif
