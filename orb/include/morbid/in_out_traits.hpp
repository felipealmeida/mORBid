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
struct in_traits
{
  typedef T type;
};

template <typename T, typename Enable = void>
struct out_traits
{
  typedef T& type;
};

template <typename T, typename Enable = void>
struct inout_traits : out_traits<T, Enable> {};

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
struct in_traits<std::vector<T> >
{
  typedef std::vector<T>const& type;
};

template <typename T>
struct return_traits<std::vector<T> >
{
  typedef std::vector<T> type;
};

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
