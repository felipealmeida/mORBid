/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_SERIALIZE_REFERENCE_HPP
#define MORBID_IIOP_SERIALIZE_REFERENCE_HPP

#include <morbid/ior/generator/structured_ior.hpp>

namespace morbid { namespace iiop {

template <typename OutputIterator, typename StructuredIOR>
void serialize_reference(OutputIterator& iterator, StructuredIOR sior)
{
  std::cout << "serializing reference" << std::endl;
  ior::generator::structured_ior_generator<OutputIterator, structured_ior>
    structured_ior_generator;
  namespace karma = boost::spirit::karma;
  if(karma::generate(iterator, structured_ior_generator(true /*little endian*/), sior))
  {
    std::cout << "Success generating IOR" << std::endl;
  }
  else
  {
    std::cout << "Failed generating interface" << std::endl;
    throw MARSHALL();
  }
}

} }

#endif

