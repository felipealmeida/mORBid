/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IIOP_SERIALIZE_OBJECT_HPP
#define TECORB_IIOP_SERIALIZE_OBJECT_HPP

#include <morbid/iiop/generator/string.hpp>

#include <boost/integer.hpp>

namespace morbid { namespace iiop {

template <typename OutputIterator>
void serialize_object(OutputIterator& iterator, bool little_endian
                      , const char* string)
{
  namespace karma = boost::spirit::karma;
  generator::string<OutputIterator> string_grammar;
  std::string attr(string);
  if(!karma::generate(iterator, string_grammar(little_endian), attr))
    throw std::runtime_error("Failed serializing a string");
}

template <typename OutputIterator>
void serialize_object(OutputIterator& iterator, bool little_endian
                      , const unsigned char* string)
{
  return serialize_object(iterator, little_endian
                          , static_cast<const char*>(static_cast<const void*>(string)));
}

template <typename OutputIterator>
void serialize_object(OutputIterator& iterator, bool little_endian
                      , bool b)
{
  *iterator++ = b ? '\1' : '\0' ;
}

template <typename OutputIterator>
void serialize_object(OutputIterator& iterator, bool little_endian, char c)
{
  *iterator++ = c;
}

template <typename OutputIterator>
void serialize_object(OutputIterator& iterator, bool little_endian, double d)
{
  const char* i = static_cast<const char*>(static_cast<const void*>(&d));
  if(little_endian && true /* am I little endian? */)
  {
    *iterator++ = i[0];
    *iterator++ = i[1];
    *iterator++ = i[2];
    *iterator++ = i[3];
    *iterator++ = i[4];
    *iterator++ = i[5];
    *iterator++ = i[6];
    *iterator++ = i[7];
  }
  else
  {
    *iterator++ = i[7];
    *iterator++ = i[6];
    *iterator++ = i[5];
    *iterator++ = i[4];
    *iterator++ = i[3];
    *iterator++ = i[2];
    *iterator++ = i[1];
    *iterator++ = i[0];
  }
}

template <typename OutputIterator>
void serialize_object(OutputIterator& iterator, bool little_endian, float f)
{
  const char* i = static_cast<const char*>(static_cast<const void*>(&f));
  if(little_endian && true /* am I little endian? */)
  {
    *iterator++ = i[0];
    *iterator++ = i[1];
    *iterator++ = i[2];
    *iterator++ = i[3];
  }
  else
  {
    *iterator++ = i[3];
    *iterator++ = i[2];
    *iterator++ = i[1];
    *iterator++ = i[0];
  }
}

template <typename OutputIterator>
void serialize_object(OutputIterator& iterator, bool little_endian, boost::int_t<32u>::least l)
{
  const char* first = static_cast<const char*>(static_cast<const void*>(&l));
  const char* last = static_cast<const char*>(static_cast<const void*>(&l)) + sizeof(l);
  if(little_endian && true)
    iterator = std::copy(first, last, iterator);
  else
    iterator = std::reverse_copy(first, last, iterator);
}

template <typename OutputIterator>
void serialize_object(OutputIterator& iterator, bool little_endian, boost::int_t<16u>::least l)
{
  const char* first = static_cast<const char*>(static_cast<const void*>(&l));
  const char* last = static_cast<const char*>(static_cast<const void*>(&l)) + sizeof(l);
  if(little_endian && true)
    iterator = std::copy(first, last, iterator);
  else
    iterator = std::reverse_copy(first, last, iterator);
}

} }

#endif
