/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IN_OUT_TRAITS_HPP
#define MORBID_IN_OUT_TRAITS_HPP

#include <morbid/primitive_types.hpp>
#include <morbid/string.hpp>
#include <boost/integer.hpp>

namespace morbid {

struct primitive_tag {};
struct struct_tag {};
struct interface_tag {};

template <typename T, typename Enable = void>
struct in_traits;

template <typename T, typename Enable = void>
struct out_traits;

template <typename T, typename Enable = void>
struct inout_traits;

template <typename T, typename Enable = void>
struct return_traits : in_traits<T, Enable> {};

template <typename Enable>
struct return_traits<void, Enable>
{
  typedef void type;
};

template <typename T, typename Enable = void>
struct type_kind;

template <typename T>
struct type_kind<T, typename boost::enable_if<boost::is_scalar<T> >::type>
{
  typedef primitive_tag type;
};

template <typename T>
struct type_kind<T, typename boost::disable_if<boost::is_scalar<T> >::type>
{
  typedef typename T::_morbid_type_kind type;
};

template <typename T>
struct in_traits<T, typename boost::enable_if
                 <boost::is_same<typename T::_morbid_type_kind, struct_tag>
                 , void>::type>
{
  typedef T const& type;
};

template <typename T>
struct out_traits<T, typename boost::enable_if
                  <boost::is_same<typename T::_morbid_type_kind, struct_tag>
                  , void>::type>
{
  typedef T& type;
};

template <typename T>
struct inout_traits<T, typename boost::enable_if
                    <boost::is_same<typename T::_morbid_type_kind, struct_tag>
                    , void>::type>
{
  typedef T& type;
};

template <typename T>
struct return_traits<T, typename boost::enable_if
                     <boost::is_same<typename T::_morbid_type_kind, struct_tag>
                      , void>::type>
{
  typedef T type;
};

template <typename T>
struct in_traits<T, typename boost::enable_if
                 <boost::is_same<typename T::_morbid_type_kind, interface_tag>
                 , void>::type>
{
  typedef typename T::_ptr_type type;
};

template <typename T>
struct out_traits<T, typename boost::enable_if
                  <boost::is_same<typename T::_morbid_type_kind, interface_tag>
                  , void>::type>
{
  typedef typename T::_ptr_type& type;
};

template <typename T>
struct inout_traits<T, typename boost::enable_if
                    <boost::is_same<typename T::_morbid_type_kind, interface_tag>
                    , void>::type>
{
  typedef typename T::_ptr_type& type;
};

template <typename T>
struct return_traits<T, typename boost::enable_if
                     <boost::is_same<typename T::_morbid_type_kind, interface_tag>
                      , void>::type>
{
  typedef typename T::_ptr_type type;
};

template <>
struct in_traits<String>
{
  typedef const char* type;
};

template <>
struct in_traits<std::string>
{
  typedef std::string type;
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
  typedef String_out type;
};

template <>
struct out_traits<std::string>
{
  typedef std::string& type;
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

template <>
struct inout_traits<String>
{
  typedef String_out type;
};

template <>
struct inout_traits<std::string>
{
  typedef std::string& type;
};

template <>
struct inout_traits<Boolean> : out_traits<Boolean> {};

template <>
struct inout_traits<UShort> : out_traits<UShort> {};

template <>
struct inout_traits<Short> : out_traits<Short> {};

template <>
struct inout_traits<ULong> : out_traits<ULong> {};

template <>
struct inout_traits<Long> : out_traits<Long> {};

template <>
struct inout_traits<ULongLong> : out_traits<ULongLong> {};

template <>
struct inout_traits<LongLong> : out_traits<LongLong> {};

template <>
struct inout_traits<Double> : out_traits<Double> {};

template <>
struct inout_traits<Float> : out_traits<Float> {};

template <>
struct inout_traits<Char> : out_traits<Char> {};

template <>
struct inout_traits<WChar> : out_traits<WChar> {};

template <>
struct inout_traits<Octet> : out_traits<Octet> {};

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
