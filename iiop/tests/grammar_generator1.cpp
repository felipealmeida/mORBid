/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/giop/grammars/message_1_0.hpp>

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
  namespace phoenix = boost::phoenix;

  typedef fusion::vector2<unsigned char, unsigned int> something;
  typedef fusion::vector3<unsigned char, unsigned char, fusion::vector2<unsigned char, unsigned int> > attribute_type;

  attribute_type attribute('\1', '\0', something('\0', 32u));

  typedef giop::forward_back_insert_iterator<std::vector<char> > iterator_type;
  typedef morbid::giop::grammars::message_1_0<iiop::generator_domain
                                              , iterator_type
                                              , attribute_type, 0u> message_header_grammar;
  message_header_grammar message_header;
  std::vector<char> output;
  iterator_type iterator(output);
  if(karma::generate(iterator
                     , giop::compile<iiop::generator_domain>
                     (message_header(phoenix::val(giop::little_endian)))
                     , attribute))
  {
    std::cout << "Success" << std::endl;
    boost::algorithm::hex(output.begin(), output.end(), std::ostream_iterator<char>(std::cout));
    std::endl(std::cout);
  }
  else
  {
    std::cout << "Failed" << std::endl;
    std::abort();
  }
}


