/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <tecorb/poa.hpp>
#include <tecorb/poa/connection.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/bind.hpp>

#include <sstream>
#include <string>
#include <cstring>

namespace tecorb { namespace poa {

void POAManager::activate()
{
  poa->activate();
}

POAManager_ptr POA::the_POAManager()
{
  if(!poa_manager)
    poa_manager.reset(new POAManager(shared_from_this()));
  return poa_manager;
}

void POA::activate()
{
  boost::asio::ip::tcp::resolver resolver(acceptor.get_io_service());
  boost::asio::ip::tcp::resolver::query query("localhost", "1888");
  boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
  acceptor.open(endpoint.protocol());
  acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor.bind(endpoint);
  acceptor.listen();

  boost::shared_ptr<connection> c(new connection(acceptor.get_io_service()));
  acceptor.async_accept(c->socket
                        , boost::bind(&POA::handle_accept, shared_from_this()
                                      , c));
}

void POA::handle_accept(boost::shared_ptr<connection> c)
{
  std::cout << "handle_accept" << std::endl;
  

  boost::shared_ptr<connection> new_c(new connection(acceptor.get_io_service()));
  acceptor.async_accept(new_c->socket
                        , boost::bind(&POA::handle_accept, shared_from_this(), new_c));
}

POA::POA(String_ptr name, boost::asio::io_service& io_service)
  : name(name)
  , acceptor(io_service)
{
}

String_ptr POA::activate_object(ServantBase* impl)
{
  object_map.insert(impl);
  std::stringstream stm;
  stm << static_cast<void*>(impl);
  std::string str = stm.str();
  char* p = new char[str.size()+1];
  std::memcpy(p, str.c_str(), str.size()+1);
  String_ptr pp (p);
  return pp;
}

Object_ptr POA::id_to_reference(const char* s)
{
  void* impl_void = 0;
  std::stringstream stm(s);
  stm >> impl_void;
  ServantBase* p = static_cast<ServantBase*>(impl_void);
  return p->_construct_local_stub("localhost", 1888, name);
}

String_ptr create_ior_string(std::string const& host, unsigned short port
                             , String_ptr poa_name, ServantBase* impl)
{
  std::string string;
  std::size_t impl_;
  std::memcpy(&impl_, impl, sizeof(impl));
  namespace karma = boost::spirit::karma;
  karma::generate(std::back_inserter<std::string>(string)
                  , "corbaloc::" << karma::lit(host)
                  << ":" << karma::ushort_(port) << "/" << karma::lit(poa_name.get())
                  << "/" << karma::hex(impl_));
  String_ptr r( new char[string.size()+1] );
  std::strcpy(r.get(), string.c_str());
  return r;
}

String_ptr create_ior_string(std::string const& host, unsigned short port
                             , std::string const& object_key)
{
  std::string string;
  namespace karma = boost::spirit::karma;
  karma::generate(std::back_inserter<std::string>(string)
                  , "corbaloc::" << karma::lit(host)
                  << ":" << karma::ushort_(port) << "/" << karma::lit(object_key));
  String_ptr r( new char[string.size()+1] );
  std::strcpy(r.get(), string.c_str());
  return r;
}

} }
