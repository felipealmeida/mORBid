/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_GRAMMAR_REFERENCE_FROM_IOR_HPP
#define MORBID_GRAMMAR_REFERENCE_FROM_IOR_HPP

#include <morbid/in_out_traits.hpp>

#include <morbid/ior/grammar/ior.hpp>
#include <morbid/iiop/grammar/profile_body_1_1.hpp>

namespace morbid { namespace grammar {

namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;

template <typename Iterator>
struct iiop_from_ior : qi::grammar<Iterator, iiop::profile_body(Iterator)>
{
  iiop_from_ior() : iiop_from_ior::base_type(start)
  {
  }

  qi::rule<Iterator, iiop::profile_body(Iterator)> start;
};

template <typename Iterator, typename T>
struct reference_from_ior : qi::grammar<Iterator, typename T::_ptr_type(Iterator)>
{
  reference_from_ior() : reference_from_ior::base_type(start)
  {
  }

  grammar::iiop_from_ior<Iterator> iiop_from_ior;
  qi::rule<Iterator, typename T::_ptr_type(Iterator)> start;
};

} }

#endif
