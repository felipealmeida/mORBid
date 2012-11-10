/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_PRIMITIVE_TYPES_H
#define TECORB_PRIMITIVE_TYPES_H

#include <boost/integer.hpp>

namespace morbid {

typedef bool Boolean;
typedef boost::uint_t<16u>::least UShort;
typedef boost::int_t<16u>::least Short;
typedef boost::uint_t<32u>::least ULong;
typedef boost::int_t<32u>::least Long;
typedef boost::uint_t<64u>::least ULongLong;
typedef boost::int_t<64u>::least LongLong;
typedef double Double;
typedef float Float;
typedef char Char;
typedef wchar_t WChar;
typedef unsigned char Octet;

}

#endif
