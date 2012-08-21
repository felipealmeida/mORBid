/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IIOP_SERIALIZE_OBJECT_HPP
#define TECORB_IIOP_SERIALIZE_OBJECT_HPP

#include <tecorb/iiop/generator/string.hpp>

namespace tecorb { namespace iiop {

template <typename OutputIterator>
void serialize_object(OutputIterator iterator, bool little_endian
                      , const char* string)
{
  namespace karma = boost::spirit::karma;
  generator::string<OutputIterator> string_grammar;
  std::string attr(string);
  if(!karma::generate(iterator, string_grammar(little_endian), attr))
    throw std::runtime_error("Failed serializing a string");
}

} }

#endif
