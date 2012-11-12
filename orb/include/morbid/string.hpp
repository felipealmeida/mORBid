/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_STRING_HPP
#define MORBID_STRING_HPP

#include <morbid/var.hpp>
#include <morbid/primitive_types.hpp>

#include <cstring>

namespace morbid {

typedef char* String;
typedef char* string;
typedef char* String_ptr;
typedef char* string_ptr;

struct String_var;

void swap(String_var& lhs, String_var& rhs);

struct String_var
{
  String_var() : string(0) {}
  String_var(char *p) : string(p) {}
  String_var(const char *p)
  {
    string = new char[std::strlen(p)+1];
    std::strcpy(string, p);
  }
  String_var(const String_var &s)
  {
    if(s.string)
    {
      string = new char[std::strlen(s.string)+1];
      std::strcpy(string, s.string);
    }
    else
      string = 0;
  }
  ~String_var()
  {
    delete[] string;
  }
  String_var &operator=(char *p)
  {
    String_var tmp(p);
    swap(tmp);
    return *this;
  }
  String_var &operator=(const char *p)
  {
    String_var tmp(p);
    swap(tmp);
    return *this;
  }
  String_var &operator=(const String_var &p)
  {
    String_var tmp(p);
    swap(tmp);
    return *this;
  }
  operator char*&()
  {
    return string;
  }
  operator const char*() const
  {
    return string;
  }
  const char* in() const
  {
    return string;
  }
  char*& inout()
  {
    return string;
  }
  char*& out()
  {
    return string;
  }
  char* _retn()
  {
    return string;
  }
  char &operator[](ULong index)
  {
    return string[index];
  }
  char operator[](ULong index) const
  {
    return string[index];
  }
  void swap(String_var& other)
  {
    morbid::swap(*this, other);
  }
  friend void swap(String_var& lhs, String_var& rhs)
  {
    std::swap(lhs.string, rhs.string);
  }
  char* release()
  {
    char* p = string;
    string = 0;
    return p;
  }
private:
  char* string;
};

struct String_out;
void swap(String_out& lhs, String_out& rhs);

struct String_out
{
  String_out() : string(0)
  {}
  String_out(char*& p)
  {
    string = p;
    p = 0;
  }
  String_out(const char* p)
  {
    string = new char[std::strlen(p)+1];
    std::strcpy(string, p);
  }
  String_out(String_var& p)
  {
    string = p.release();
  }
  String_out(const String_out& s)
  {
    if(s.string)
    {
      string = new char[std::strlen(s.string)+1];
      std::strcpy(string, s.string);
    }
    else
      string = 0;
  }
  String_out& operator=(const String_out& s)
  {
    String_out tmp(s);
    swap(tmp);
    return *this;
  }
  String_out& operator=(char* p)
  {
    String_out tmp(p);
    swap(tmp);
    return *this;
  }
  String_out& operator=(const char* p)
  {
    String_out tmp(p);
    swap(tmp);
    return *this;
  }
  void swap(String_out& other)
  {
    morbid::swap(*this, other);
  }
  friend void swap(String_out& lhs, String_out& rhs)
  {
    std::swap(lhs.string, rhs.string);
  }
  operator char*&();
  char*& ptr();
private:
  // assignment from String_var disallowed
  void operator=(const String_var&);
  char* string;
};

typedef String_var string_var;

}

#endif
