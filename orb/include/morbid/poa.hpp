/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_POA_HPP
#define MORBID_POA_HPP

#include <morbid/var.hpp>
#include <morbid/narrow.hpp>
#include <morbid/object.hpp>
#include <morbid/local_object.hpp>
#include <morbid/string.hpp>
#include <morbid/reply.hpp>
#include <morbid/structured_ior.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <set>

// namespace morbid { namespace poa {

// struct connection;

// struct POA;
// typedef boost::shared_ptr<POA> POA_ptr;
// typedef morbid::var<POA> POA_var;

// struct POAManager;
// typedef boost::shared_ptr<POAManager> POAManager_ptr;
// typedef morbid::var<POAManager> POAManager_var;

// struct ServantBase
// {
//   Object_ptr _this();

//   virtual Object_ptr _construct_local_stub(structured_ior const& ior) = 0;
//   virtual void _dispatch(const char* name
//                          , std::size_t align_offset, const char* request_body_first
//                          , const char* request_body_last
//                          , bool little_endian, reply&) = 0;
//   virtual const char* _get_interface() const = 0;
// };

// struct POAManager : narrow<POAManager, boost::mpl::vector1<LocalObject> >
// {
//   POAManager(POA_ptr poa)
//     : poa(poa) {}
//   void activate();

//   // Object
//   bool _is_a(const char* id) { return true; }
//   const char* _get_interface() const { std::abort(); }
//   static POAManager_ptr _construct_remote_stub(structured_ior const&) { std::abort(); }

//   POA_ptr poa;
// };

// struct connection;

// struct POA : narrow<POA, boost::mpl::vector1<LocalObject> >
//            , boost::enable_shared_from_this<POA>
// {
//   POA(String_ptr name, boost::asio::io_service& io_service);

//   void activate();

//   String_ptr activate_object(ServantBase*);
//   Object_ptr id_to_reference(const char*);

//   POAManager_ptr the_POAManager();
//   // Object
//   bool _is_a(const char* id) { return true; }
//   static POA_ptr _construct_remote_stub(structured_ior const&) { std::abort(); }
//   const char* _get_interface() const { std::abort(); }
// private:
//   friend struct connection;

//   void handle_accept(boost::shared_ptr<connection> c);

//   String_ptr name;
//   POAManager_ptr poa_manager;
//   boost::asio::ip::tcp::acceptor acceptor;
//   boost::asio::ip::tcp::endpoint local_endpoint;
//   std::set<ServantBase*> object_map;
// };

// // String_ptr create_ior_string(std::string const& host, unsigned short port
// //                              , String_ptr poa_name, ServantBase* impl);
// // String_ptr create_ior_string(std::string const& host, unsigned short port
// //                              , std::string const& object_key);

// } }

#endif
