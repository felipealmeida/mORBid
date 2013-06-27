/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/orb.hpp>
#include <morbid/giop/forward_back_insert_iterator.hpp>
#include <morbid/ior/grammar/corbaloc.hpp>
#include <morbid/ior/grammar/ior.hpp>
#include <morbid/ior/grammar/tagged_profile.hpp>
#include <morbid/ior/grammar/generic_tagged_profile.hpp>
#include <morbid/ior/hex_directive.hpp>
#include <morbid/iiop/all.hpp>
#include <morbid/iiop/grammars/profile_body_1_1.hpp>
#include <morbid/poa/connection.hpp>

#include <boost/algorithm/hex.hpp>
#include <boost/bind.hpp>

#include <cstring>

namespace std {

std::ostream& operator<<(std::ostream& os, std::vector<char>::iterator i)
{
  return os << "[vector Iterator]";
}

}

namespace morbid {

orb_impl::orb_impl()
  : last_id(0u), acceptor(io_service)
{
  boost::asio::ip::tcp::endpoint endpoint;
  acceptor.open(endpoint.protocol());
  acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor.bind(endpoint);

  local_endpoint = acceptor.local_endpoint();

  std::cout << "port " << local_endpoint.port() << std::endl;

  acceptor.listen();
}

void orb_impl::stop()
{
  io_service.stop();
}

void orb_impl::handle_accept(boost::shared_ptr<poa::connection> c)
{
  std::cout << "handle_accept" << std::endl;
  c->start();

  boost::shared_ptr<poa::connection> new_c
    (new poa::connection(acceptor.get_io_service(), shared_from_this()));
  new_c->start();
  acceptor.async_accept(new_c->socket
                        , boost::bind(&orb_impl::handle_accept, shared_from_this(), new_c));
}

void orb_impl::run()
{
  boost::shared_ptr<poa::connection> c
    (new poa::connection(acceptor.get_io_service(), shared_from_this()));
  acceptor.async_accept(c->socket
                        , boost::bind(&orb_impl::handle_accept, shared_from_this(), c));

  boost::asio::io_service::work w(io_service);
  io_service.reset();
  std::cout << "io_service::run" << std::endl;
  io_service.run();
}

}

