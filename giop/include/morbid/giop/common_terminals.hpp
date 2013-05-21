/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_GIOP_COMMON_TERMINALS_HPP
#define MORBID_GIOP_COMMON_TERMINALS_HPP

#include <boost/spirit/home/support.hpp>
#include <boost/spirit/home/support/common_terminals.hpp>

namespace morbid { namespace giop {

using ::boost::spirit::bool_;
using ::boost::spirit::ushort_;
using ::boost::spirit::short_;
using ::boost::spirit::ulong_;
using ::boost::spirit::long_;
using ::boost::spirit::ulong_long;
using ::boost::spirit::long_long;
using ::boost::spirit::float_;
using ::boost::spirit::double_;
using ::boost::spirit::lit;
BOOST_SPIRIT_TERMINAL_EX(long_double);
BOOST_SPIRIT_TERMINAL_EX(string);
BOOST_SPIRIT_TERMINAL_EX(octet);
BOOST_SPIRIT_TERMINAL_EX(endianness);
BOOST_SPIRIT_TERMINAL_EX(sequence);
BOOST_SPIRIT_TERMINAL_EX(raw_size);

} }

#endif
