/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_POA_HPP
#define TECORB_POA_HPP

#include <tecorb/var.hpp>
#include <tecorb/narrow.hpp>
#include <tecorb/object.hpp>
#include <tecorb/local_object.hpp>
#include <tecorb/string.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <set>

namespace tecorb { namespace poa {

struct connection;

struct POA;
typedef boost::shared_ptr<POA> POA_ptr;
typedef tecorb::var<POA> POA_var;

struct POAManager;
typedef boost::shared_ptr<POAManager> POAManager_ptr;
typedef tecorb::var<POAManager> POAManager_var;

struct ServantBase
{
  Object_ptr _this();

  virtual Object_ptr _construct_local_stub(std::string const& host
                                           , unsigned short port
                                           , String_ptr poa_name) = 0;
};

struct POAManager : narrow<POAManager, boost::mpl::vector1<LocalObject> >
{
  POAManager(POA_ptr poa)
    : poa(poa) {}
  void activate();

  // Object
  bool _is_a(const char* id) { return true; }
  static POAManager_ptr _construct_remote_stub(std::string const&, unsigned short
                                               , std::string const&) { std::abort(); }

  POA_ptr poa;
};

struct POA : narrow<POA, boost::mpl::vector1<LocalObject> >
           , boost::enable_shared_from_this<POA>
{
  POA(String_ptr name, boost::asio::io_service& io_service);

  void activate();

  String_ptr activate_object(ServantBase*);
  Object_ptr id_to_reference(const char*);

  POAManager_ptr the_POAManager();
  // Object
  bool _is_a(const char* id) { return true; }
  static POA_ptr _construct_remote_stub(std::string const&, unsigned short
                                        , std::string const&) { std::abort(); }
private:
  void handle_accept(boost::shared_ptr<connection> c);

  String_ptr name;
  POAManager_ptr poa_manager;
  boost::asio::ip::tcp::acceptor acceptor;
  boost::asio::ip::tcp::endpoint local_endpoint;
  std::set<ServantBase*> object_map;
};

String_ptr create_ior_string(std::string const& host, unsigned short port
                             , String_ptr poa_name, ServantBase* impl);
String_ptr create_ior_string(std::string const& host, unsigned short port
                             , std::string const& object_key);

} }

#endif
