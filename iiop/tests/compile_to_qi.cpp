/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/iiop/all.hpp>
#include <morbid/giop/forward_back_insert_iterator.hpp>

#include <boost/algorithm/hex.hpp>

int main()
{
  namespace giop = morbid::giop;
  namespace qi = boost::spirit::qi;
  namespace fusion = boost::fusion;
  namespace spirit = boost::spirit;
  namespace iiop = morbid::iiop;
  namespace phoenix = boost::phoenix;
  using phoenix::at_c;

  typedef fusion::vector<unsigned char, unsigned char, std::string
                         , unsigned short, std::vector<unsigned char> > attribute_type;

  const char object_key_literal[] = "RootPOA/OBJECTKEY";
  std::vector<unsigned char> object_key;
  std::copy(&object_key_literal[0], &object_key_literal[sizeof(object_key_literal)-1]
            , std::back_inserter(object_key));
  attribute_type attribute('\1', '\0', "localhost", 8080, object_key);

  attribute_type attribute_read;
  const char output[] = 
    "\x01\x01\x00\x00\x0A\x00\x00\x00\x6C\x6F\x63\x61\x6C"
    "\x68\x6F\x73\x74\x00\x90\x1F\x11\x00\x00\x00\x52\x6F"
    "\x6F\x74\x50\x4F\x41\x2F\x4F\x42\x4A\x45\x43\x54\x4B\x45\x59";
  typedef const char* iterator_type;
  iterator_type first = &output[0]
    , last = &output[sizeof(output)-1];
  if(qi::parse(first, last
               , giop::compile<iiop::parser_domain>
               (
                giop::endianness
                [
                 giop::octet                      // IIOP major version
                 & giop::octet                    // IIOP minor version
                 & giop::string                   // host
                 & giop::ushort_                  // port
                 & giop::sequence[giop::octet]    // object key
                ]
               )
               , attribute_read))
  {
    std::cout << "Success" << std::endl;
    assert(attribute == attribute_read);
  }
  else
  {
    std::cout << "Failed" << std::endl;
    std::abort();
  }
}

