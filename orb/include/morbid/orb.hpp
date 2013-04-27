/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_ORB_INIT_HPP
#define MORBID_ORB_INIT_HPP

#include <morbid/var.hpp>
#include <morbid/poa.hpp>
#include <morbid/object.hpp>
#include <morbid/string.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace morbid { namespace poa {

    struct connection;

}

struct orb_impl : boost::enable_shared_from_this<orb_impl>
{
  orb_impl();

  void run();
  void stop();
  void handle_accept(boost::shared_ptr<poa::connection> c);

  boost::asio::io_service io_service;
  boost::asio::ip::tcp::acceptor acceptor;
  boost::asio::ip::tcp::endpoint local_endpoint;
};

struct orb
{
  orb() : impl(new orb_impl) {}

  void run() { impl->run(); }
  void stop() { impl->stop(); }
private:
  boost::shared_ptr<orb_impl> impl;
};

}

#endif
