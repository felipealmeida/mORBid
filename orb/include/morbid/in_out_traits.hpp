/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IN_OUT_TRAITS_HPP
#define MORBID_IN_OUT_TRAITS_HPP

#include <morbid/primitive_types.hpp>
#include <morbid/reference.hpp>
#include <morbid/tags.hpp>

#include <boost/integer.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_scalar.hpp>

namespace morbid {

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

// template <typename T>
// struct in_traits<T, typename boost::enable_if
//                  <boost::is_same<typename T::_morbid_type_kind, struct_tag>
//                  , void>::type>
// {
//   typedef T const& type;
// };

// template <typename T>
// struct out_traits<T, typename boost::enable_if
//                   <boost::is_same<typename T::_morbid_type_kind, struct_tag>
//                   , void>::type>
// {
//   typedef T& type;
// };

// template <typename T>
// struct inout_traits<T, typename boost::enable_if
//                     <boost::is_same<typename T::_morbid_type_kind, struct_tag>
//                     , void>::type>
// {
//   typedef T& type;
// };

// template <typename T>
// struct return_traits<T, typename boost::enable_if
//                      <boost::is_same<typename T::_morbid_type_kind, struct_tag>
//                       , void>::type>
// {
//   typedef T type;
// };

template <typename C>
struct in_traits<reference<C> >
{
  typedef reference<C> type;
};

template <typename C>
struct out_traits<reference<C> >
{
  typedef reference<C>& type;
};

template <typename C>
struct inout_traits<reference<C> >
{
  typedef reference<C>& type;
};

template <typename C>
struct return_traits<reference<C> >
{
  typedef reference<C> type;
};

template <>
struct in_traits<std::string>
{
  typedef std::string type;
};

template <>
struct in_traits<bool>
{
  typedef bool type;
};

template <>
struct in_traits<unsigned_short>
{
  typedef unsigned_short type;
};

template <>
struct in_traits<short_>
{
  typedef short_ type;
};

template <>
struct in_traits<unsigned_long>
{
  typedef unsigned_long type;
};

template <>
struct in_traits<long_>
{
  typedef long_ type;
};

template <>
struct in_traits<unsigned_long_long>
{
  typedef unsigned_long_long type;
};

template <>
struct in_traits<long_long>
{
  typedef long_long type;
};

template <>
struct in_traits<double>
{
  typedef double type;
};

template <>
struct in_traits<float>
{
  typedef float type;
};

template <>
struct in_traits<char>
{
  typedef char type;
};

template <>
struct in_traits<wchar_t>
{
  typedef wchar_t type;
};

template <>
struct in_traits<octet>
{
  typedef octet type;
};

template <typename T>
struct in_traits<std::vector<T> >
{
  typedef std::vector<T>const& type;
};

template <>
struct out_traits<std::string>
{
  typedef std::string& type;
};

template <>
struct out_traits<bool>
{
  typedef bool& type;
};

template <>
struct out_traits<unsigned_short>
{
  typedef unsigned_short& type;
};

template <>
struct out_traits<short_>
{
  typedef short_& type;
};

template <>
struct out_traits<unsigned_long>
{
  typedef unsigned_long& type;
};

template <>
struct out_traits<long_>
{
  typedef long_& type;
};

template <>
struct out_traits<unsigned_long_long>
{
  typedef unsigned_long_long& type;
};

template <>
struct out_traits<long_long>
{
  typedef long_long& type;
};

template <>
struct out_traits<double>
{
  typedef double& type;
};

template <>
struct out_traits<float>
{
  typedef float& type;
};

template <>
struct out_traits<char>
{
  typedef char& type;
};

template <>
struct out_traits<wchar_t>
{
  typedef wchar_t& type;
};

template <>
struct out_traits<octet>
{
  typedef octet& type;
};

template <typename T>
struct out_traits<std::vector<T> >
{
  typedef std::vector<T>& type;
};

template <>
struct inout_traits<std::string>
{
  typedef std::string& type;
};

template <>
struct inout_traits<bool> : out_traits<bool> {};

template <>
struct inout_traits<unsigned_short> : out_traits<unsigned_short> {};

template <>
struct inout_traits<short_> : out_traits<short_> {};

template <>
struct inout_traits<unsigned_long> : out_traits<unsigned_long> {};

template <>
struct inout_traits<long_> : out_traits<long_> {};

template <>
struct inout_traits<unsigned_long_long> : out_traits<unsigned_long_long> {};

template <>
struct inout_traits<long_long> : out_traits<long_long> {};

template <>
struct inout_traits<double> : out_traits<double> {};

template <>
struct inout_traits<float> : out_traits<float> {};

template <>
struct inout_traits<char> : out_traits<char> {};

template <>
struct inout_traits<wchar_t> : out_traits<wchar_t> {};

template <>
struct inout_traits<octet> : out_traits<octet> {};

template <typename T>
struct inout_traits<std::vector<T> > : out_traits<std::vector<T> > {};

typedef out_traits<bool>::type Boolean_out;
typedef out_traits<unsigned_short>::type UShort_out;
typedef out_traits<short_>::type Short_out;
typedef out_traits<unsigned_long>::type ULong_out;
typedef out_traits<long_>::type Long_out;
typedef out_traits<unsigned_long_long>::type ULongLong_out;
typedef out_traits<long_long>::type LongLong_out;
typedef out_traits<double>::type Double_out;
typedef out_traits<float>::type Float_out;
typedef out_traits<char>::type Char_out;
typedef out_traits<wchar_t>::type WChar_out;
typedef out_traits<octet>::type Octet_out;

}

#endif
