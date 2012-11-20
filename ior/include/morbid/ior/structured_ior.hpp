/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IOR_STRUCTURED_IOR_HPP
#define MORBID_IOR_STRUCTURED_IOR_HPP

#include <morbid/ior/tagged_profile.hpp>

#include <boost/integer.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/variant.hpp>

#include <vector>

namespace morbid { namespace ior {

template <typename Sequence, typename Variant>
struct structured_ior
{
  std::string type_id;

  typedef Sequence sequence_type;
  typedef Variant variant_type;
  std::vector<variant_type> structured_profiles;
};

} }

BOOST_FUSION_ADAPT_TPL_STRUCT((Sequence)(Variant)
                              , (::morbid::ior::structured_ior)(Sequence)(Variant)
                              , (std::string, type_id)
                              (std::vector<Variant>, structured_profiles));

#endif
