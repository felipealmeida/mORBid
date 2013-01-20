/* (c) Copyright 2012 Felipe Magno de Almeida
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
  namespace qi = spirit::qi;
  namespace phoenix = boost::phoenix;

  typedef fusion::vector0<> attribute_type;

  attribute_type attribute;

  typedef giop::forward_back_insert_iterator<std::vector<char> > output_iterator_type;
  typedef morbid::giop::grammars::message_1_0<iiop::generator_domain
                                              , output_iterator_type
                                              , attribute_type, 0u>
    generator_message_header_grammar;
  generator_message_header_grammar generator_message_header;
  std::vector<char> output;
  output_iterator_type iterator(output);
  if(karma::generate(iterator
                     , giop::compile<iiop::generator_domain>
                     (generator_message_header(phoenix::val(giop::little_endian)))
                     , attribute))
  {
    std::cout << "Success" << std::endl;
    typedef std::vector<char>::const_iterator iterator_type;
    typedef morbid::giop::grammars::message_1_0<iiop::parser_domain
                                                , iterator_type
                                                , attribute_type, 0u>
      parser_message_header_grammar;
    parser_message_header_grammar parser_message_header;
    attribute_type attribute_read;
    iterator_type first = output.begin(), last = output.end();
    if(qi::parse(first, last, giop::compile<iiop::parser_domain>
                 (parser_message_header)
                 , attribute_read))
    {
      std::cout << "attribute " << attribute
                << std::endl
                << "attribute read " << attribute_read << std::endl;

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


