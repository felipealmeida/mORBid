/* Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_STL_WORKAROUNDS_IPP
#define TECORB_IDL_COMPILER_STL_WORKAROUNDS_IPP

#include <ostream>
#include <utility>
#include <vector>

namespace std {

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, std::pair<T, U> p)
{
  return os << "[pair first: " << p.first << " second: " << p.second << ']';
}

template <typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> p)
{
  return os << "[vector size: " << p.size() << ']';
}

}

#endif
