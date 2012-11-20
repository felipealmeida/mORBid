/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_STRUCTURED_IOR_HPP
#define MORBID_STRUCTURED_IOR_HPP

#include <morbid/ior/structured_ior.hpp>
#include <morbid/ior/tagged_profile.hpp>
#include <morbid/iiop/profile_body.hpp>

#include <boost/mpl/vector.hpp>

namespace morbid {

namespace mpl = boost::mpl;

typedef ior::structured_ior<mpl::vector2<iiop::profile_body, ior::tagged_profile>
                            , boost::variant<iiop::profile_body, ior::tagged_profile> > structured_ior;

}

#endif

