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
  namespace karma = boost::spirit::karma;
  namespace fusion = boost::fusion;
  namespace spirit = boost::spirit;
  namespace iiop = morbid::iiop;
  namespace phoenix = boost::phoenix;
  using phoenix::at_c;

  typedef fusion::vector<unsigned char, unsigned char, std::string
                         , unsigned short, std::vector<unsigned char> > attribute_type;
  typedef giop::forward_back_insert_iterator<std::vector<char> > iterator_type;

  const char object_key_literal[] = "RootPOA/OBJECTKEY";
  std::vector<unsigned char> object_key;
  std::copy(&object_key_literal[0], &object_key_literal[sizeof(object_key_literal)-1]
            , std::back_inserter(object_key));
  attribute_type attribute('\1', '\0', "localhost", 8080, object_key);

  std::vector<char> output;
  iterator_type iterator(output);
  if(karma::generate(iterator
                     , giop::compile<iiop::generator_domain>
                     (
                      giop::endianness(true)
                      [
                       giop::octet                      // IIOP major version
                       & giop::octet                    // IIOP minor version
                       & giop::string                   // host
                       & giop::ushort_                  // port
                       & giop::sequence[giop::octet]    // object key
                      ]
                     )
                     , attribute))
  {
    std::cout << "Success" << std::endl;
    boost::algorithm::hex(output.begin(), output.end(), std::ostream_iterator<char>(std::cout));
  }
  else
  {
    std::cout << "Failed" << std::endl;
    std::abort();
  }
}

