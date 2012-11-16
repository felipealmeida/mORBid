/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IOR_GRAMMAR_IOR_HPP
#define MORBID_IOR_GRAMMAR_IOR_HPP

#include <morbid/ior/grammar/tagged_profile.hpp>
#include <morbid/ior/ior.hpp>
#include <morbid/iiop/grammar/string.hpp>
#include <morbid/iiop/grammar/align.hpp>
#include <morbid/iiop/profile_body.hpp>

#include <iostream>

namespace std {

template <typename Iterator>
std::ostream& operator<<(std::ostream& os, std::vector<char>const& v)
{
  os << "[vector ";
  std::copy(v.begin(), v.end()
            , std::ostream_iterator<char>(os));
  return os << "]";
}

}

namespace morbid { namespace ior { namespace grammar {

namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;

template <typename Iterator>
struct ior : qi::grammar<Iterator, morbid::ior::ior(Iterator)
           , qi::locals<bool> >
{
  ior() : ior::base_type(start)
  {
    using qi::_1;
    using qi::_a; using qi::_val; using qi::_r1;
    namespace phoenix = boost::phoenix;

    start %= 
      qi::omit[qi::char_[_a = _1]]
      >> iiop::grammar::align(_r1, 4u)
      >> string(_a)
      >> iiop::grammar::align(_r1, 4u)
      >> tagged_profiles(_r1, _a)
      ;
    start.name("ior");
    qi::debug(start);
    // octet_sequence.name("octet_sequence");
    // qi::debug(octet_sequence);
  }

  grammar::tagged_profiles<Iterator> tagged_profiles;
  iiop::grammar::string<Iterator, char> string;
  qi::rule<Iterator, morbid::ior::ior(Iterator), qi::locals<bool> > start;
};

} } }

#endif
