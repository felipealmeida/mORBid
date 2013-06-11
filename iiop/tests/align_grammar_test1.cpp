/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/giop/grammars/reply_1_0.hpp>

#include <morbid/iiop/all.hpp>
#include <morbid/giop/forward_back_insert_iterator.hpp>

#include <boost/algorithm/hex.hpp>

namespace std {

template <typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> const& v)
{
  return os << "[Service contexts size: " << v.size() << "]";
}

}

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
  typedef fusion::vector4
    <std::vector<service_context_f>, unsigned int, unsigned int
     , unsigned short> attribute_type;

  std::vector<unsigned char> service_context_seq;
  const char service_context_array[] = "service_context"; // 15
  service_context_seq.insert(service_context_seq.end()
                             , &service_context_array[0]
                             , &service_context_array[sizeof(service_context_array)-1]);

  std::vector<service_context_f> service_contexts;
  service_contexts.push_back(service_context_f(2020, service_context_seq));
  typedef boost::fusion::vector3<unsigned char
                                 , attribute_type
                                 , unsigned int>
    attribute_test_type;
  attribute_test_type attribute(15, attribute_type(service_contexts, 10, 20, 16u)
                                , 17u);

  typedef giop::forward_back_insert_iterator<std::vector<char> > output_iterator_type;
  typedef morbid::giop::grammars::reply_1_0<iiop::generator_domain
                                            , output_iterator_type
                                            , attribute_type>
    generator_reply_header_grammar;
  generator_reply_header_grammar generator_reply_header
    (giop::ushort_);
  std::vector<char> output;
  output_iterator_type iterator(output);
  if(karma::generate(iterator
                     , giop::compile<iiop::generator_domain>
                     (giop::octet                             // 1 byte
                                                              // 3 bytes of padding
                      & generator_reply_header(giop::native_endian)
                      & giop::endianness(giop::native_endian) // 1 byte
                      [
                                                              // 1 bytes of padding
                        giop::ulong_                          // 4 bytes
                      ]
                     )
                     , attribute))
  {
    std::cout << "Success" << std::endl;
    
    boost::algorithm::hex(output.begin(), output.end(), std::ostream_iterator<char>(std::cout));
    std::endl(std::cout);

    {
      std::vector<char> output1;
      output_iterator_type iterator(output1);
      attribute_type attribute(service_contexts, 10, 20, 16u);
      bool r = karma::generate(iterator
                               , giop::compile<iiop::generator_domain>
                               (generator_reply_header(giop::little_endian))
                               , attribute);
      assert(r);
      assert(output1.size() + 10 == output.size());
    }

    typedef std::vector<char>::const_iterator iterator_type;
    typedef morbid::giop::grammars::reply_1_0<iiop::parser_domain
                                              , iterator_type
                                              , attribute_type>
      parser_reply_header_grammar;
    parser_reply_header_grammar parser_reply_header(giop::ushort_);
    attribute_test_type attribute_read;
    iterator_type first = output.begin(), last = output.end();
    std::cout << "Read test" << std::endl;
    if(qi::parse(first, last, giop::compile<iiop::parser_domain>
                 (giop::octet
                  & parser_reply_header(giop::native_endian)
                  & giop::endianness
                  [
                   giop::ulong_
                  ]
                 )
                 , attribute_read))
    {
      std::cout << "Value written to " << attribute << std::endl;
      std::cout << "Value read " << attribute_read << std::endl;
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


