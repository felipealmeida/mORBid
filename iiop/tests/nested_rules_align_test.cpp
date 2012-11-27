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
  if(karma::generate(iterator
                     , giop::compile<iiop::generator_domain>
                     (giop::endianness(giop::little_endian)
                      [
                       generator_reply_header
                      ])
                     , attribute))
  {
    std::cout << "Success" << std::endl;

    const char correct_output[] = "\x01\x00\x00\x00\x01\x00\x00\x00\xE4\x07\x00\x00\x0F\x00\x00\x00\x73\x65\x72\x76\x69\x63\x65\x5F\x63\x6F\x6E\x74\x65\x78\x74\x00\x0A\x00\x00\x00\x14\x00\x00\x00";
    std::size_t const correct_output_size = sizeof(correct_output)-1;
    assert(correct_output_size == output.size()
           && std::equal(output.begin(), output.end(), &correct_output[0]));
    
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
                 (giop::endianness
                  [
                   parser_reply_header
                  ])
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


