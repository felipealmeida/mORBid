/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_ORB_INIT_HPP
#define TECORB_ORB_INIT_HPP

#include <morbid/var.hpp>
#include <morbid/poa.hpp>
#include <morbid/object.hpp>
#include <morbid/string.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/asio/io_service.hpp>

namespace morbid {

struct ORB
{
  ORB();

  void run();
  void shutdown(bool wait_for_completion);
  void destroy();

  Object_ptr resolve_initial_references(const char* id);
  String_ptr object_to_string(Object_ptr);
  Object_ptr string_to_object(const char* ref);

private:
  poa::POA_ptr root_poa;
  boost::asio::io_service io_service;
};

typedef boost::shared_ptr<ORB> ORB_ptr;
typedef morbid::var<ORB> ORB_var;

ORB_ptr ORB_init(int argc, const char* argv[], const char* str_op);
inline ORB_ptr ORB_init(int argc, char* argv[], const char* str_op)
{
  return ORB_init(argc, const_cast<const char**>(argv), str_op);
}

}

#endif
