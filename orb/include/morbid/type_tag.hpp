/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_TYPE_TAG_HPP
#define MORBID_TYPE_TAG_HPP

#include <morbid/in_out_traits.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/not.hpp>

namespace morbid { namespace type_tag {

namespace mpl = boost::mpl;

struct in_tag {};
struct out_tag {};
struct inout_tag {};

template <typename T, typename Tag>
struct value_type_tag;

template <typename T>
struct value_type_tag<T, in_tag>
{
  typedef T original_type;
  typedef typename in_traits<T>::type type;
  typedef in_tag tag;

  value_type_tag(type value) : value(value) {}

  type value;
};

template <typename T>
struct value_type_tag<T, out_tag>
{
  typedef T original_type;
  typedef typename out_traits<T>::type type;
  typedef out_tag tag;

  value_type_tag(type value) : value(value) {}

  type value;
};

template <typename T>
struct value_type_tag<T, inout_tag>
{
  typedef T original_type;
  typedef typename inout_traits<T>::type type;
  typedef inout_tag tag;

  value_type_tag(type value) : value(value) {}

  type value;
};

template <typename T>
struct is_in_type_tag : boost::is_same<in_tag, typename T::tag> {};

template <typename T>
struct is_out_type_tag : boost::is_same<out_tag, typename T::tag> {};

template <typename T>
struct is_inout_type_tag : boost::is_same<inout_tag, typename T::tag> {};

template <typename T>
struct is_not_in_type_tag : mpl::not_<boost::is_same<in_tag, typename T::tag> > {};

template <typename T>
struct is_not_out_type_tag : mpl::not_<boost::is_same<out_tag, typename T::tag> > {};

template <typename T>
struct is_not_inout_type_tag : mpl::not_<boost::is_same<inout_tag, typename T::tag> > {};

template <typename T>
struct original_type
{
  typedef typename T::original_type type;
};

template <typename T>
struct tag_type
{
  typedef typename T::tag_type type;
};

template <typename T>
struct value_type
{
  typedef typename T::type type;
};

} }

#endif
