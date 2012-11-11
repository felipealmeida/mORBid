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

template <>
struct out_traits<String>
{
  typedef char* type;
};

template <>
struct out_traits<Boolean>
{
  typedef Boolean& type;
};

template <>
struct out_traits<UShort>
{
  typedef UShort& type;
};

template <>
struct out_traits<Short>
{
  typedef Short& type;
};

template <>
struct out_traits<ULong>
{
  typedef ULong& type;
};

template <>
struct out_traits<Long>
{
  typedef Long& type;
};

template <>
struct out_traits<ULongLong>
{
  typedef ULongLong& type;
};

template <>
struct out_traits<LongLong>
{
  typedef LongLong& type;
};

template <>
struct out_traits<Double>
{
  typedef Double& type;
};

template <>
struct out_traits<Float>
{
  typedef Float& type;
};

template <>
struct out_traits<Char>
{
  typedef Char& type;
};

template <>
struct out_traits<WChar>
{
  typedef WChar& type;
};

template <>
struct out_traits<Octet>
{
  typedef Octet& type;
};

typedef out_traits<String>::type String_out;
typedef out_traits<Boolean>::type Boolean_out;
typedef out_traits<UShort>::type UShort_out;
typedef out_traits<Short>::type Short_out;
typedef out_traits<ULong>::type ULong_out;
typedef out_traits<Long>::type Long_out;
typedef out_traits<ULongLong>::type ULongLong_out;
typedef out_traits<LongLong>::type LongLong_out;
typedef out_traits<Double>::type Double_out;
typedef out_traits<Float>::type Float_out;
typedef out_traits<Char>::type Char_out;
typedef out_traits<WChar>::type WChar_out;
typedef out_traits<Octet>::type Octet_out;

}

#endif
