/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_PARSE_ARGUMENT_HPP
#define TECORB_PARSE_ARGUMENT_HPP

#include <morbid/iiop/grammar/sequence.hpp>
#include <morbid/iiop/grammar/align.hpp>
#include <morbid/any.hpp>
#include <morbid/primitive_types.hpp>

#include <boost/mpl/assert.hpp>

#include <limits>
#include <iomanip>

namespace morbid {

template <typename T>
struct argument_tag
{
};

inline bool parse_argument(const char* first, const char*& rq_current
                           , const char* rq_last, bool little_endian
                           , argument_tag<bool>)
{
  if(rq_current != rq_last)
  {
    return *rq_current++;
  }
  else
    throw std::runtime_error("Format error");
}

inline char parse_argument(const char* first, const char*& rq_current
                           , const char* rq_last, bool little_endian
                           , argument_tag<char>)
{
  if(rq_current != rq_last)
  {
    return *rq_current++;
  }
  else
    throw std::runtime_error("Format error");
}

inline Double parse_argument(const char* first, const char*& rq_current
                             , const char* rq_last, bool little_endian
                             , argument_tag< ::morbid::Double>)
{
  BOOST_MPL_ASSERT_RELATION( (std::numeric_limits< ::morbid::Double>::is_iec559), ==, true);
  BOOST_MPL_ASSERT_RELATION( sizeof(::morbid::Double), ==, 8u);
  int const size = sizeof(::morbid::Double);
  if(std::distance(rq_current, rq_last) >= size)
  {
    ::morbid::Double r;
    if(little_endian && true /* am I little endian? */)
    {
      std::memcpy(&r, rq_current, sizeof(::morbid::Double));
    }
    else // endianness doesnt match
    {
      std::reverse_copy(rq_current, rq_current + sizeof(::morbid::Double), &r);
    }
    rq_current += sizeof(::morbid::Double);
    return r;
  }
  else
    throw std::runtime_error("Error parsing");
}

inline Float parse_argument(const char* first, const char*& rq_current
                            , const char* rq_last, bool little_endian
                            , argument_tag< ::morbid::Float>)
{
  BOOST_MPL_ASSERT_RELATION( (std::numeric_limits< ::morbid::Float>::is_iec559), ==, true);
  BOOST_MPL_ASSERT_RELATION( sizeof(::morbid::Float), ==, 4u);
  int const size = sizeof(::morbid::Float);
  if(std::distance(rq_current, rq_last) >= size)
  {
    ::morbid::Float r;
    if(little_endian && true /* am I little endian? */)
    {
      std::memcpy(&r, rq_current, sizeof(::morbid::Float));
    }
    else // endianness doesnt match
    {
      std::reverse_copy(rq_current, rq_current + sizeof(::morbid::Float), &r);
    }
    rq_current += sizeof(::morbid::Float);
    return r;
  }
  else
    throw std::runtime_error("Error parsing");
}

inline ::morbid::Long parse_argument(const char* first, const char*& rq_current
                                     , const char* rq_last, bool little_endian
                                     , argument_tag< ::morbid::Long>)
{
  int const size = sizeof(::morbid::Long);
  if(std::distance(rq_current, rq_last) >= size)
  {
    ::morbid::Long r;
    if(little_endian && true /* am I little endian? */)
    {
      std::memcpy(&r, rq_current, sizeof(::morbid::Long));
    }
    else // endianness doesnt match
    {
      std::reverse_copy(rq_current, rq_current + sizeof(::morbid::Long), &r);
    }
    rq_current += sizeof(::morbid::Long);
    return r;
  }
  else
    throw std::runtime_error("Error parsing");
}

inline ::morbid::Short parse_argument(const char* first, const char*& rq_current
                                      , const char* rq_last, bool little_endian
                                      , argument_tag< ::morbid::Short>)
{
  throw std::runtime_error("NOT_IMPLEMENTED");
}

inline wchar_t parse_argument(const char* first, const char*& rq_current
                           , const char* rq_last, bool little_endian
                           , argument_tag<wchar_t>)
{
  throw std::runtime_error("NOT_IMPLEMENTED");
}

inline std::vector<wchar_t> parse_argument(const char* first, const char*& rq_current
                           , const char* rq_last, bool little_endian
                           , argument_tag<const wchar_t*>)
{
  throw std::runtime_error("NOT_IMPLEMENTED");
}

inline morbid::Any_ptr parse_argument(const char* first, const char*& rq_current
                                      , const char* rq_last, bool little_endian
                                      , argument_tag<morbid::Any_ptr>)
{
  throw std::runtime_error("NOT_IMPLEMENTED");
}

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

inline std::vector<unsigned char>
parse_argument(const char* first, const char*& rq_current
               , const char* rq_last, bool little_endian
               , argument_tag<const unsigned char*>)
{
  std::vector<char> v = parse_argument(first, rq_current
                                       , rq_last, little_endian
                                       , argument_tag<const char*>());
  std::vector<unsigned char> r(v.begin(), v.end());
  return r;
}

}

#endif
