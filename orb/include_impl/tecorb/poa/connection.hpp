/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_POA_CONNECTION_HPP
#define TECORB_POA_CONNECTION_HPP

#include <tecorb/poa.hpp>

#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <boost/system/error_code.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace tecorb { namespace poa {

struct connection : boost::enable_shared_from_this<connection>
{
  connection(boost::asio::io_service& io_service
             , boost::weak_ptr<POA> poa_)
    : socket(io_service), poa_(poa_) {}

  void start();

  bool handle_request(std::vector<char>::const_iterator first
                      , std::vector<char>::const_iterator last
                      , bool little_endian);
  void handle_read(boost::system::error_code const& ec
                   , std::size_t bytes_read);
  void process_input();

  std::vector<char> processing_buffer;
  boost::array<char, 512*1024> read_buffer;
  boost::asio::ip::tcp::socket socket;
  boost::weak_ptr<POA> poa_;
};

} }

#endif
