/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_REPLY_HEADER_HPP
#define MORBID_IIOP_REPLY_HEADER_HPP

#include <morbid/iiop/service_context_list.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

#include <boost/integer.hpp>
#include <ostream>

namespace morbid { namespace iiop {

struct reply_header
{
  iiop::service_context_list service_context_list;
  boost::uint_t<32u>::least request_id;
  boost::uint_t<32u>::least reply_status;
};

inline std::ostream& operator<<(std::ostream& os, reply_header const& r)
{
  return os << "[ rid: " << r.request_id << " reply status: " << r.reply_status << "]";
}

} }

BOOST_FUSION_ADAPT_STRUCT(morbid::iiop::reply_header
                          , (morbid::iiop::service_context_list, service_context_list)
                          (boost::uint_t<32u>::least, request_id)
                          (boost::uint_t<32u>::least, reply_status));

#endif
