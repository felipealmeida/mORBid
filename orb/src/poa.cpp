/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/poa.hpp>
#include <morbid/poa/connection.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/bind.hpp>

#include <sstream>
#include <string>
#include <cstring>

namespace morbid { namespace poa {

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
  acceptor.listen();

  boost::shared_ptr<connection> c(new connection(acceptor.get_io_service(), shared_from_this()));
  acceptor.async_accept(c->socket
                        , boost::bind(&POA::handle_accept, shared_from_this(), c));
}

void POA::handle_accept(boost::shared_ptr<connection> c)
{
  std::cout << "handle_accept" << std::endl;

  c->start();

  boost::shared_ptr<connection> new_c(new connection(acceptor.get_io_service(), shared_from_this()));
  acceptor.async_accept(new_c->socket
                        , boost::bind(&POA::handle_accept, shared_from_this(), new_c));
}

POA::POA(String_ptr name, boost::asio::io_service& io_service)
  : name(name)
  , acceptor(io_service)
{
  boost::asio::ip::tcp::endpoint endpoint;
  acceptor.open(endpoint.protocol());
  acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor.bind(endpoint);

  local_endpoint = acceptor.local_endpoint();

  std::cout << "port " << local_endpoint.port() << std::endl;
}

String_ptr POA::activate_object(ServantBase* impl)
{
  std::cout << "Activating " << impl << std::endl;
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
  structured_ior ior;
  const char* interface_ = p->_get_interface();
  ior.type_id.insert(ior.type_id.end(), interface_, interface_ + std::strlen(interface_));
  iiop::profile_body profile_body;
  const char* localhost = "localhost";
  profile_body.host.insert(profile_body.host.end(), localhost, localhost + std::strlen(localhost));
  profile_body.port = local_endpoint.port();

  std::size_t impl_;
  std::memcpy(&impl_, &impl_void, sizeof(impl_void));
  assert(sizeof(std::size_t) >= sizeof(impl_void));
  namespace karma = boost::spirit::karma;
  std::vector<char> object_key;
  karma::generate(std::back_inserter(object_key)
                  , karma::lit(name) << "/" << karma::uint_generator<std::size_t, 16u>()(impl_));

  profile_body.object_key = object_key;
  ior.structured_profiles.push_back(profile_body);
  return p->_construct_local_stub(ior);
}

// String_ptr create_ior_string(std::string const& host, unsigned short port
//                              , String_ptr poa_name, ServantBase* impl)
// {
//   std::string string;
//   std::size_t impl_;
//   std::memcpy(&impl_, &impl, sizeof(impl));
//   assert(sizeof(std::size_t) >= sizeof(ServantBase*));
//   namespace karma = boost::spirit::karma;
//   karma::generate(std::back_inserter<std::string>(string)
//                   , "corbaloc::" << karma::lit(host)
//                   << ":" << karma::ushort_(port) << "/" << karma::lit(poa_name/*.get()*/)
//                   << "/" << karma::uint_generator<std::size_t, 16u>()(impl_));
//   String_ptr r( new char[string.size()+1] );
//   std::strcpy(r/*.get()*/, string.c_str());
//   return r;
// }

// String_ptr create_ior_string(std::string const& host, unsigned short port
//                              , std::string const& object_key)
// {
//   std::string string;
//   namespace karma = boost::spirit::karma;
//   karma::generate(std::back_inserter<std::string>(string)
//                   , "corbaloc::" << karma::lit(host)
//                   << ":" << karma::ushort_(port) << "/" << karma::lit(object_key));
//   String_ptr r( new char[string.size()+1] );
//   std::strcpy(r/*.get()*/, string.c_str());
//   return r;
// }

} }
