/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_REPLY_HPP
#define TECORB_REPLY_HPP

namespace tecorb {

struct reply
{
  unsigned int request_id;
  std::vector<char> reply_body;
};

}

#endif

