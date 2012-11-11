/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IN_OUT_TRAITS_HPP
#define TECORB_IN_OUT_TRAITS_HPP

#include <morbid/primitive_types.hpp>
#include <morbid/string.hpp>
#include <boost/integer.hpp>

namespace morbid {

template <typename T, typename Enable = void>
struct in_traits;

template <typename T, typename Enable = void>
struct out_traits;

template <typename T, typename Enable = void>
struct inout_traits;

template <>
struct in_traits<String>
{
  typedef const char* type;
};

template <>
struct in_traits<Boolean>
{
  typedef Boolean type;
};

template <>
struct in_traits<UShort>
{
  typedef UShort type;
};

template <>
struct in_traits<Short>
{
  typedef Short type;
};

template <>
struct in_traits<ULong>
{
  typedef ULong type;
};

template <>
struct in_traits<Long>
{
  typedef Long type;
};

template <>
struct in_traits<ULongLong>
{
  typedef ULongLong type;
};

template <>
struct in_traits<LongLong>
{
  typedef LongLong type;
};

template <>
struct in_traits<Double>
{
  typedef Double type;
};

template <>
struct in_traits<Float>
{
  typedef Float type;
};

template <>
struct in_traits<Char>
{
  typedef Char type;
};

template <>
struct in_traits<WChar>
{
  typedef WChar type;
};

template <>
struct in_traits<Octet>
{
  typedef Octet type;
};

}

#endif
