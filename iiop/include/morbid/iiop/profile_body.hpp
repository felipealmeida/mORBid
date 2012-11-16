/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_PROFILE_BODY_HPP
#define MORBID_IIOP_PROFILE_BODY_HPP

#include <boost/range/iterator_range.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <ostream>

namespace morbid { namespace iiop {

struct profile_body
{
  std::string host;
  unsigned short port;
  std::string object_key;

  profile_body() : port(0u) {}
  profile_body(std::vector<char> const& host, unsigned short port
               , std::vector<char> const& object_key)
    : host(host.begin(), host.end()), port(port)
    , object_key(object_key.begin(), object_key.end())
  {}
};

inline std::ostream& operator<<(std::ostream& os, profile_body const& p)
{
  return os << "[profile_body host: " << boost::make_iterator_range(p.host.begin(), p.host.end())
            << " port: " << p.port << ']';
}

} }

BOOST_FUSION_ADAPT_STRUCT( ::morbid::iiop::profile_body
                           , (std::string, host)
                           (unsigned short, port)
                           (std::string, object_key));

#endif
