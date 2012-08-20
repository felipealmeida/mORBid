/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_PARSE_ARGUMENT_HPP
#define TECORB_PARSE_ARGUMENT_HPP

#include <tecorb/iiop/grammar/sequence.hpp>
#include <tecorb/iiop/grammar/align.hpp>

#include <iomanip>

namespace tecorb {

template <typename T>
struct argument_tag
{
};

inline
std::vector<char> parse_argument(const char* first, const char*& rq_current
                                 , const char* rq_last, bool little_endian
                                 , argument_tag<const char*>)
{
  {
    std::cout << "start of argument reading" << std::endl;
  const char *iterator = rq_current
    , *last = rq_last;
  while(iterator != last)
  {
    std::cout.fill('0');
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::endl(std::cout);
  }
  std::endl(std::cout);
  std::dec(std::cout);
  std::cout << "end of argument reading" << std::endl;
  }

  namespace qi = boost::spirit::qi;
  iiop::grammar::sequence<const char*, char> sequence_grammar;
  std::vector<char> sequence;
  if(qi::parse(rq_current, rq_last
               , iiop::grammar::align(first, 4u) >> sequence_grammar(little_endian)
               , sequence))
  {
    std::cout << "Parsed sequence" << std::endl;
    if(!sequence.empty() && !*boost::prior(sequence.end()))
      return sequence;
  }
  throw std::runtime_error("Error parsing argument");
}

}

#endif
