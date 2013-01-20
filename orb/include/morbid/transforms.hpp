/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_TRANSFORMS_HPP
#define MORBID_TRANSFORMS_HPP

namespace morbid { namespace transforms {

template <typename T, typename Enable = void>
struct from_unmanaged_to_managed;

template <typename T>
struct from_unmanaged_to_managed<T, typename boost::disable_if
                                  <boost::is_same
                                   <typename T::original_type::_morbid_type_kind, interface_tag>
                                   >::type>
{
  typedef typename boost::remove_const<
    typename boost::remove_reference
    <typename T::original_type>::type>::type type;
};

template <typename T>
struct from_unmanaged_to_managed<T, typename boost::enable_if
                                  <boost::is_same
                                   <typename T::original_type::_morbid_type_kind
                                    , interface_tag>
                                   >::type>
{
  typedef typename T::original_type::_ptr_type type;
};

template <typename Tag>
struct from_unmanaged_to_managed<type_tag::value_type_tag<morbid::string, Tag> >
{
  typedef std::string type;
};



} }

#endif
