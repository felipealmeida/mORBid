/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IOR_TAGGED_PROFILE_HPP
#define MORBID_IOR_TAGGED_PROFILE_HPP

#include <boost/integer.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <vector>
#include <ostream>

namespace morbid { namespace ior {

struct tagged_profile
{
  boost::uint_t<32u>::least tag;
  std::vector<char> profile_data;
};

inline std::ostream& operator<<(std::ostream& os, tagged_profile const& p)
{
  return os << "[tagged_profile tag: " << p.tag << " profile_data: " << p.profile_data.size() << ']';
}

} }

BOOST_FUSION_ADAPT_STRUCT( ::morbid::ior::tagged_profile
                           , (boost::uint_t<32u>::least, tag)
                           (std::vector<char>, profile_data));

#endif
