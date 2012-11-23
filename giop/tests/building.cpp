/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/giop/common_terminals.hpp>

template <typename Expr>
void test(Expr const& expr)
{
  
}

int main()
{
  namespace giop = morbid::giop;
  test(giop::float_ & giop::double_ & giop::long_double);
}

