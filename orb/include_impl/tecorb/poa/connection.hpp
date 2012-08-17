/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

namespace tecorb { namespace poa {

struct connection
{
  connection(boost::asio::io_service& io_service)
    : socket(io_service) {}

  boost::asio::ip::tcp::socket socket;
};

} }
