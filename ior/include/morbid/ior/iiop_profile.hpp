/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IOR_IIOP_PROFILE_HPP
#define TECORB_IOR_IIOP_PROFILE_HPP

namespace morbid { namespace ior {

struct iiop_profile
{
  std::string host;
  unsigned short port;
  std::string object_key;

  iiop_profile() : port(0u) {}
  iiop_profile(std::vector<char> const& host, unsigned short port
               , std::vector<char> const& object_key)
    : host(host.begin(), host.end()), port(port)
    , object_key(object_key.begin(), object_key.end())
  {}
};

} }

#endif
