/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_TRANSFORMS_HPP
#define MORBID_TRANSFORMS_HPP

#include <morbid/in_out_traits.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace morbid { namespace transforms {

template <typename T, typename Enable = void>
struct from_unmanaged_to_managed
{
  typedef typename T::original_type type;
};

// template <typename T>
// struct from_unmanaged_to_managed<T, typename boost::disable_if
//                                     <boost::is_same<typename type_kind<typename T::original_type>::type, interface_tag>
//                                      , void
//                                     >::type>
// {
//   typedef typename boost::remove_const<
//     typename boost::remove_reference
//     <typename T::original_type>::type>::type type;
// };

// template <typename T>
// struct from_unmanaged_to_managed<T, typename boost::enable_if
//                                     <boost::is_same<typename type_kind<typename T::original_type>::type, interface_tag>
//                                      , void
//                                     >::type>
// {
//   typedef typename T::original_type type;
// };

// template <>
// struct from_unmanaged_to_managed<type_tag::value_type_tag<morbid::string, type_tag::in_tag>, void>
// {
//   typedef std::string type;
// };

// template <>
// struct from_unmanaged_to_managed<type_tag::value_type_tag<morbid::string, type_tag::inout_tag>, void>
//   : from_unmanaged_to_managed<type_tag::value_type_tag<morbid::string, type_tag::in_tag>, void>
// {};

// template <>
// struct from_unmanaged_to_managed<type_tag::value_type_tag<morbid::string, type_tag::out_tag>, void>
//   : from_unmanaged_to_managed<type_tag::value_type_tag<morbid::string, type_tag::in_tag>, void>
// {};

} }

#endif
