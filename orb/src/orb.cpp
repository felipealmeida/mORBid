/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/orb.hpp>
#include <morbid/ior/grammar/corbaloc.hpp>

#include <cstring>

namespace morbid {

ORB_ptr ORB_init(int argc, const char* argv[], const char* str_op)
{
  ORB_ptr orb(new ORB);
  return orb;
}

ORB::ORB()
{
  const char rootpoa[] = {"RootPOA"};
  String_ptr p(new char[sizeof(rootpoa)-1]);
  std::memcpy(p.get(), rootpoa, sizeof(rootpoa)-1);
  root_poa.reset(new poa::POA(p, io_service));
}

Object_ptr ORB::resolve_initial_references(const char* id)
{
  if(!std::strcmp(id, "RootPOA"))
    return Object_ptr(root_poa);
  else
    return Object_ptr();
}

String_ptr ORB::object_to_string(Object_ptr p)
{
  return p->ior();
}

Object_ptr ORB::string_to_object(const char* ref)
{
  const char *first = ref
    , *last = ref + std::strlen(ref);
  morbid::ior::grammar::corbaloc<const char*> corbaloc_grammar;
  morbid::ior::iiop_profile iiop_profile;
  if(boost::spirit::qi::parse(first, last, corbaloc_grammar, iiop_profile))
  {
    std::cout << "parsed successfully corbaloc" << std::endl;
    std::cout << "host: " << iiop_profile.host
              << " port: " << iiop_profile.port
              << " objectkey: " << iiop_profile.object_key << std::endl;
    return Object_ptr
      (new ::morbid::remote_stub::Object(iiop_profile.host, iiop_profile.port
                                         , iiop_profile.object_key));
  }
  else
  {
    std::cout << "Couldn't handle corbaloc (should throw INVALID_PARAM)" << std::endl;
    return Object_ptr();
  }
}

void ORB::shutdown(bool wait_for_completion)
{
  io_service.stop();
}

void ORB::run()
{
  boost::asio::io_service::work w(io_service);
  io_service.run();
}

}

