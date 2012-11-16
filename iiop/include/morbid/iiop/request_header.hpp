/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_REQUEST_HEADER_HPP
#define MORBID_IIOP_REQUEST_HEADER_HPP

#include <morbid/iiop/service_context_list.hpp>

#include <boost/integer.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace morbid { namespace iiop {

struct request_header
{
  iiop::service_context_list service_context_list;
  boost::uint_t<32u>::least request_id;
  bool response_expected;
  std::vector<char> object_key;
  std::string operation;
  std::vector<char> requesting_principal;
};

} }

BOOST_FUSION_ADAPT_STRUCT(morbid::iiop::request_header
                          , (morbid::iiop::service_context_list, service_context_list)
                          (boost::uint_t<32u>::least, request_id)
                          (bool, response_expected)(std::vector<char>, object_key)
                          (std::string, operation)(std::vector<char>, requesting_principal));

#endif
