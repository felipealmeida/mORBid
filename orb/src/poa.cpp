/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <tecorb/poa.hpp>

#include <sstream>
#include <string>
#include <cstring>

namespace tecorb { namespace poa {

void POAManager::activate()
{
}

POAManager_ptr POA::the_POAManager()
{
  return poa_manager;
}

POA::POA(String_ptr name)
  : name(name)
  , poa_manager(new POAManager)
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
  return p->_construct_local_stub("localhost", 888, name);
}

} }
