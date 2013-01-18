/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/orb.hpp>
#include <morbid/giop/forward_back_insert_iterator.hpp>
#include <morbid/ior/grammar/corbaloc.hpp>
#include <morbid/ior/grammar/ior.hpp>
#include <morbid/ior/grammar/tagged_profile.hpp>
#include <morbid/ior/grammar/generic_tagged_profile.hpp>
#include <morbid/ior/hex_directive.hpp>
#include <morbid/iiop/all.hpp>
#include <morbid/iiop/grammars/profile_body_1_1.hpp>

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

  typedef giop::forward_back_insert_iterator<std::vector<char> > output_iterator_type;
  typedef structured_ior attribute_type;
  typedef ior::grammar::ior<iiop::generator_domain, output_iterator_type
                            , attribute_type>
    ior_grammar_type;

  ior::grammar::tagged_profile<iiop::generator_domain, output_iterator_type
                               , ior::tagged_profile> tagged_profile;
  iiop::grammar::profile_body_1_0<iiop::generator_domain, output_iterator_type
                                  , iiop::profile_body> profile_body;
  ior::grammar::generic_tagged_profile<iiop::generator_domain, output_iterator_type
                                       , iiop::profile_body, 0u
                                       > tagged_profile_body
    (giop::endianness[profile_body]);

  ior_grammar_type ior_grammar(tagged_profile | tagged_profile_body);

  std::vector<char> ior;
  namespace karma = boost::spirit::karma;
  output_iterator_type iterator(ior);
  if(karma::generate(iterator
                     , karma::lit("IOR:")
                     << ior::hex
                     [
                      giop::compile<iiop::generator_domain>(ior_grammar(giop::native_endian))
                     ]
                     , sior))
  {
    std::cout << "Success" << std::endl;
    std::copy(ior.begin(), ior.end(), std::ostream_iterator<char>(std::cout));
    std::endl(std::cout);

    char* string = new char[ior.size()+1];
    string[ior.size()] = 0;
    std::copy(ior.begin(), ior.end(), string);
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
  std::cout << "string_to_object" << std::endl;
  typedef const char* iterator_type;
  typedef std::vector<char> hex_parser_container;
  typedef hex_parser_container::const_iterator hex_iterator;
  iterator_type first = ref, last = ref + std::strlen(ref);

  typedef structured_ior attribute_type;
  namespace fusion = boost::fusion;
  typedef typename fusion::result_of::as_vector
    <fusion::joint_view<fusion::single_view<char> // minor version
                        , iiop::profile_body> >::type profile_body_1_1_attr;

  ior::grammar::tagged_profile<iiop::parser_domain, hex_iterator
                               , ior::tagged_profile> tagged_profile;
  iiop::grammar::profile_body_1_0<iiop::parser_domain, hex_iterator
                                  , iiop::profile_body> profile_body_1_0;
  iiop::grammar::profile_body_1_1<iiop::parser_domain, hex_iterator
                                  , profile_body_1_1_attr> profile_body_1_1;
  ior::grammar::generic_tagged_profile<iiop::parser_domain, hex_iterator
                                       , boost::variant<iiop::profile_body, profile_body_1_1_attr>, 0u
                                       > tagged_profile_body
    (giop::endianness[profile_body_1_0 | profile_body_1_1]);

  typedef fusion::vector2<std::string
                  , std::vector
                   <boost::variant<iiop::profile_body, profile_body_1_1_attr, ior::tagged_profile> >
                  > result_type;
  result_type r;

  typedef ior::grammar::ior<iiop::parser_domain, hex_iterator, result_type>
    ior_grammar_type;


  ior_grammar_type ior_grammar(tagged_profile_body | tagged_profile);
  namespace qi = boost::spirit::qi;
  iterator_type first_tmp = first;
  if(qi::parse(first_tmp, last
               ,  qi::lit("IOR:")
               >> ior::hex
               [
                   giop::compile<iiop::parser_domain>(ior_grammar)
               ]
               , r))
  {
    std::cout << "Success" << std::endl;
    structured_ior sior = {fusion::at_c<0u>(r)};
    
    for(std::vector<boost::variant<iiop::profile_body, profile_body_1_1_attr, ior::tagged_profile> >::const_iterator
          first = fusion::at_c<1u>(r).begin()
          , last = fusion::at_c<1u>(r).end(); first != last; ++first)
    {
      if(iiop::profile_body const* p = boost::get<iiop::profile_body>(&*first))
        sior.structured_profiles.push_back(*p);
      else if(ior::tagged_profile const* p = boost::get<ior::tagged_profile>(&*first))
        sior.structured_profiles.push_back(*p);
      else if(profile_body_1_1_attr const* p = boost::get<profile_body_1_1_attr>(&*first))
      {
        iiop::profile_body b;
        fusion::copy(fusion::pop_front(*p), b);
        sior.structured_profiles.push_back(b);
      }
    }

    return Object_ptr(new ::morbid::remote_stub::Object(sior));
  }
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

