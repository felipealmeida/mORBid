/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/orb.hpp>
#include <morbid/ior/grammar/corbaloc.hpp>
#include <morbid/ior/grammar/ior.hpp>
#include <morbid/iiop/grammar/profile_body_1_1.hpp>
#include <morbid/ior/generator/structured_ior.hpp>
#include <morbid/iiop/generator/align.hpp>

#include <boost/algorithm/hex.hpp>

#include <cstring>

namespace std {

std::ostream& operator<<(std::ostream& os, std::vector<char>::iterator i)
{
  return os << "[vector Iterator]";
}

}

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
  std::memcpy(p/*.get()*/, rootpoa, sizeof(rootpoa)-1);
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
  std::cout << "Object to string" << std::endl;
  structured_ior sior = p->_structured_ior();
  
  typedef std::back_insert_iterator<std::vector<char> > iterator_type;
  ior::generator::structured_ior_generator<iterator_type
                                           , structured_ior>
    structured_ior_generator;
  std::vector<char> ior;
  namespace karma = boost::spirit::karma;
  iterator_type iterator(ior);
  if(karma::generate(iterator, structured_ior_generator(true /*little endian*/), sior))
  {
    std::cout << "Success" << std::endl;
    const char IOR_str[] = "IOR:";
    std::vector<char> final_string;
    final_string.insert(final_string.end(), IOR_str, &IOR_str[sizeof(IOR_str)-1]);

    boost::algorithm::hex(ior.begin(), ior.end(), std::back_inserter(final_string));

    std::cout << "Size: " << final_string.size() << std::endl;

    char* string = new char[final_string.size()+1];
    string[final_string.size()] = 0;
    std::copy(final_string.begin(), final_string.end(), string);
    std::cout << "final string: " << '|' << string << '|' << std::endl;
    return string;
  }
  else
  {
    std::cout << "Failed generating string for object reference" << std::endl;
    throw std::runtime_error("Failed generating string for object reference");
  }
}

Object_ptr ORB::string_to_object(const char* ref)
{
  const char *first = ref
    , *last = ref + std::strlen(ref);
  morbid::ior::grammar::corbaloc<const char*> corbaloc_grammar;
  morbid::iiop::profile_body profile_body;
  if(boost::spirit::qi::parse(first, last, corbaloc_grammar, profile_body))
  {
    std::cout << "parsed successfully corbaloc" << std::endl;
    std::cout << "host: " << profile_body.host
              << " port: " << profile_body.port
              << " objectkey: " << boost::make_iterator_range(profile_body.object_key.begin()
                                                              , profile_body.object_key.end()) << std::endl;
    structured_ior ior;
    ior.type_id.insert(ior.type_id.end(), Object::_repository_id
                       , Object::_repository_id + std::strlen(Object::_repository_id));
    ior.structured_profiles.push_back(profile_body);
    return Object_ptr
      (new ::morbid::remote_stub::Object(ior));
  }
  else if(boost::spirit::qi::parse(first, last, "IOR:"))
  {
    std::cout << "Is a IOR " << boost::make_iterator_range(first, last) << std::endl;
    std::vector<char> binary;
    boost::algorithm::unhex(first, last, std::back_insert_iterator<std::vector<char> >(binary));
    ior::grammar::ior<std::vector<char>::iterator> ior_grammar;
    ior::ior ior_;
    using boost::phoenix::val;
    if(boost::spirit::qi::parse(binary.begin(), binary.end(), ior_grammar(val(binary.begin())), ior_))
    {
      std::cout << "Success ior_grammar" << std::endl;
      std::cout << "IOR: " << ior_ << std::endl;

      iiop::grammar::profile_body_1_1<std::vector<char>::const_iterator> profile_body_grammar;
      bool found = false;
      for(std::vector<ior::tagged_profile>::const_iterator
            first = ior_.profiles.begin()
            , last = ior_.profiles.end()
            ;first != last; ++first)
      {
        if(first->tag == 0 /*TAG_INTERNET_IOP*/)
          if(boost::spirit::qi::parse(first->profile_data.begin()
                                      , first->profile_data.end()
                                      , profile_body_grammar(val(first->profile_data.begin())
                                                             , binary[0])
                                      , profile_body))
          {
            std::cout << "Parsed IIOP profile body information" << std::endl;
            found = true;
            break;
          }
          else
          {
            std::cout << "Couldn't parse IIOP tagged profile" << std::endl;
          }
      }
      if(found)
      {
        structured_ior ior = {ior_.type_id};
        ior.structured_profiles.push_back(profile_body);
        return Object_ptr(new ::morbid::remote_stub::Object(ior));
      }
      else
        throw morbid::INVALID_PARAM();
    }
    else
    {
      std::cout << "Failed ior_grammar" << std::endl;
    }
  }
  else
  {
    std::cout << "Not a IOR" << std::endl;
  }
  std::cout << "Couldn't handle corbaloc (should throw INVALID_PARAM)" << std::endl;
  throw morbid::INVALID_PARAM();
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

