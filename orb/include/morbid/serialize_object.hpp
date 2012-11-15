/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_SERIALIZE_OBJECT_HPP
#define MORBID_SERIALIZE_OBJECT_HPP

#include <morbid/type_tag.hpp>
#include <morbid/in_out_traits.hpp>
#include <morbid/iiop/serialize_object.hpp>

#include <boost/utility/enable_if.hpp>

namespace morbid { namespace serialization {

template <typename T, typename Enabler = void>
struct serialize_object_aux
{
  template <typename OutputIterator>
  static void call(OutputIterator& iterator, T a)
  {
    iiop::serialize_object(iterator, true, a);
  }
};

template <typename T>
struct serialize_object_aux<T, typename boost::enable_if
                            <boost::is_same<typename T::_morbid_type_kind, struct_tag>
                             , void>::type>
{
  template <typename OutputIterator>
  static void call(OutputIterator& iterator, T a)
  {
    iiop::serialize_object<typename T::template _morbid_generator<OutputIterator> >
      (iterator, true, a);
  }
};

template <typename OutputIterator, typename T>
void serialize_object(OutputIterator& iterator, T a)
{
  serialize_object_aux<T>::call(iterator, a);
}

} }

#endif
