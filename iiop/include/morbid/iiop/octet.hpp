/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_OCTET_HPP
#define MORBID_IIOP_OCTET_HPP

#include <morbid/iiop/meta_compiler.hpp>
#include <morbid/giop/common_terminals.hpp>

#include <boost/spirit/home/karma.hpp>

namespace morbid { namespace iiop {

struct octet_encoding
{
  typedef unsigned char char_type;

  static bool isascii_(int ch) { return false; }
  static bool ischar(int ch) { return true; }
  static bool isalnum(int ch) { return false; }
  static bool isalpha(int ch) { return false; }
  static bool isdigit(int ch) { return false; }
  static bool isxdigit(int ch) { return false; }
  static bool iscntrl(int ch) { return false; }
  static bool isgraph(int ch) { return false; }
  static bool islower(int ch) { return true; }
  static bool isprint(int ch) { return false; }
  static bool ispunct(int ch) { return false; }
  static bool isspace(int ch) { return false; }
  static bool isblank(int ch) { return false; }
  static bool isupper(int ch) { return false; }
  static int tolower(int ch) { return ch; }
  static int toupper(int ch) { return ch; }
  static boost::uint32_t toucs4(int ch) { return ch; }
};

namespace spirit = boost::spirit;
namespace karma = spirit::karma;

namespace generator {

template <typename Modifiers, typename Enable>
struct make_primitive<giop::tag::octet, Modifiers, Enable>
{
  typedef karma::any_char<octet_encoding, spirit::unused_type> result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

}

} }

namespace boost { namespace spirit {

template <typename Enable>
struct use_terminal< ::morbid::iiop::generator_domain, morbid::giop::tag::octet, Enable> : mpl::true_ {};

} }

#endif
