/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/giop/grammars/message_header_1_0.hpp>

#include <morbid/iiop/all.hpp>
#include <morbid/giop/forward_back_insert_iterator.hpp>

#include <boost/algorithm/hex.hpp>

int main()
{
  namespace giop = morbid::giop;
  namespace iiop = morbid::iiop;
  namespace fusion = boost::fusion;
  namespace spirit = boost::spirit;
  namespace karma = spirit::karma;
  namespace qi = spirit::qi;
  namespace phoenix = boost::phoenix;

  typedef fusion::vector2<unsigned char, unsigned int> something;
  typedef fusion::vector3<unsigned char, unsigned char, fusion::vector2<unsigned char, unsigned int> > attribute_type;

  attribute_type attribute('\1', '\0', something('\0', 32u));

  const char output[] = "GIOP\x01\x00\x01\x00\x20\x00\x00\x00";
  typedef const char* iterator_type;
  typedef morbid::giop::grammars::message_header_1_0<iiop::parser_domain
                                                     , iterator_type
                                                     , attribute_type>
    parser_message_header_grammar;
  parser_message_header_grammar parser_message_header;
  attribute_type attribute_read;
  iterator_type first = &output[0], last = &output[sizeof(output)-1];
  if(qi::parse(first, last, giop::compile<iiop::parser_domain>
               (parser_message_header), attribute_read))
  {
    std::cout << "Major " << (unsigned int)fusion::at_c<0>(attribute_read) << std::endl;
    std::cout << "Minor " << (unsigned int)fusion::at_c<1>(attribute_read) << std::endl;
    std::cout << "Flags " << (unsigned int)fusion::at_c<0>(fusion::at_c<2>(attribute_read)) << std::endl;
    std::cout << "Size " << fusion::at_c<1>(fusion::at_c<2>(attribute_read)) << std::endl;
    assert(attribute == attribute_read);
  }
  else
  {
    std::cout << "Failed" << std::endl;
    std::abort();
  }
}


