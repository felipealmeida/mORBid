/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_PRIMITIVE_TYPES_H
#define MORBID_PRIMITIVE_TYPES_H

#include <boost/integer.hpp>
#include <string>

namespace morbid {

typedef boost::uint_t<16u>::least unsigned_short;
typedef boost::int_t<16u>::least short_;
typedef boost::uint_t<32u>::least unsigned_long;
typedef boost::int_t<32u>::least long_;
typedef boost::uint_t<64u>::least unsigned_long_long;
typedef boost::int_t<64u>::least long_long;
typedef unsigned char octet;
typedef std::string string;

}

#endif
