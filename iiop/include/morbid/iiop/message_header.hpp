/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IIOP_MESSAGE_HEADER_HPP
#define TECORB_IIOP_MESSAGE_HEADER_HPP

#include <boost/fusion/include/adapt_struct.hpp>

#include <boost/integer.hpp>

namespace morbid { namespace iiop {

struct message_header
{
  char major_version, minor_version
    , flags, message_type;
  boost::uint_t<32u>::least message_size;
};

} }

BOOST_FUSION_ADAPT_STRUCT(morbid::iiop::message_header
                          , (char, major_version)(char, minor_version)
                          (char, flags)(char, message_type)
                          (boost::uint_t<32u>::least, message_size));

#endif
