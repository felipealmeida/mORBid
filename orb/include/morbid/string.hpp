/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_STRING_HPP
#define MORBID_STRING_HPP

#include <morbid/var.hpp>

namespace morbid {

typedef char* String;
typedef char* string;
typedef char* String_ptr;
typedef char* string_ptr;

struct String_var
{
  String_var(const char* s)
    : str(s) {}
  String_var(char* s)
    : str(s)
  {
    delete[] s;
  }

  const char* in() const { return str.c_str(); }

private:
  std::string str;
};

typedef String_var string_var;
}

#endif
