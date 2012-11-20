/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IOR_IOR_HPP
#define MORBID_IOR_IOR_HPP

#include <morbid/ior/tagged_profile.hpp>

#include <boost/integer.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/range/iterator_range.hpp>

#include <vector>

namespace morbid { namespace ior {

struct ior
{
  std::string type_id;
  std::vector<tagged_profile> profiles;
};

inline std::ostream& operator<<(std::ostream& os, ior const& i)
{
  return os << "[ior type_id: " << boost::make_iterator_range(i.type_id.begin(), i.type_id.end())
            << " profiles: " << boost::make_iterator_range(i.profiles.begin(), i.profiles.end()) << ']';
}

} }

BOOST_FUSION_ADAPT_STRUCT( ::morbid::ior::ior
                           , (std::string, type_id)
                           (std::vector< ::morbid::ior::tagged_profile>, profiles));

#endif
