/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/giop/grammars/reply_header_1_0.hpp>

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

  typedef fusion::vector2<unsigned int, std::vector<unsigned char> > service_context_f;
  typedef fusion::vector3
    <std::vector<service_context_f>, unsigned int, unsigned int> attribute_type;

  std::vector<unsigned char> service_context_seq;
  const char service_context_array[] = "service_context"; // 15
  service_context_seq.insert(service_context_seq.end()
                             , &service_context_array[0]
                             , &service_context_array[sizeof(service_context_array)-1]);

  std::vector<service_context_f> service_contexts;
  service_contexts.push_back(service_context_f(2020, service_context_seq));
  attribute_type attribute(service_contexts, 10, 20);

  typedef giop::forward_back_insert_iterator<std::vector<char> > output_iterator_type;
  typedef morbid::giop::grammars::reply_header_1_0<iiop::generator_domain
                                                   , output_iterator_type
                                                   , attribute_type>
    generator_reply_header_grammar;
  generator_reply_header_grammar generator_reply_header;
  std::vector<char> output;
  output_iterator_type iterator(output);
  iiop::endianness_attribute e(giop::endian(giop::little_endian));
  if(karma::generate(iterator
                     , giop::compile<iiop::generator_domain>
                     (generator_reply_header(phoenix::val(e)))
                     , attribute))
  {
    std::cout << "Success" << std::endl;
    
    boost::algorithm::hex(output.begin(), output.end(), std::ostream_iterator<char>(std::cout));
    std::endl(std::cout);

    typedef std::vector<char>::const_iterator iterator_type;
    typedef morbid::giop::grammars::reply_header_1_0<iiop::parser_domain
                                                       , iterator_type
                                                       , attribute_type>
      parser_reply_header_grammar;
    parser_reply_header_grammar parser_reply_header;
    attribute_type attribute_read;
    iterator_type first = output.begin(), last = output.end();
    if(qi::parse(first, last, giop::compile<iiop::parser_domain>
                 (parser_reply_header(phoenix::val(giop::little_endian)))
                 , attribute_read))
    {
      assert(attribute == attribute_read);
    }
    else
    {
      std::cout << "Failed" << std::endl;
      std::abort();
    }
  }
  else
  {
    std::cout << "Failed" << std::endl;
    std::abort();
  }
}


