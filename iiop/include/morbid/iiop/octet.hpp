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

namespace fusion = boost::fusion;

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

template <typename T, typename Modifiers>
struct make_primitive<T, Modifiers, typename boost::enable_if
                      <spirit::traits::is_string<T>, void>::type>
{
  typedef typename boost::add_const<T>::type const_string;
  typedef karma::literal_string<const_string, octet_encoding, spirit::unused_type, true> result_type;

  result_type operator()(typename boost::add_reference<const_string>::type val, boost::spirit::unused_type) const
  {
    return result_type(val);
  }
};

}

} }

namespace boost { namespace spirit {

template <typename Enable>
struct use_terminal< ::morbid::iiop::generator_domain, morbid::giop::tag::octet, Enable> : mpl::true_ {};
template <typename T>
struct use_terminal< ::morbid::iiop::generator_domain
                     , T, typename enable_if<traits::is_string<T>, void>::type> : mpl::true_ {};
// template <std::size_t N, typename Enable>
// struct use_terminal< ::morbid::iiop::generator_domain
//                      , terminal_ex<tag::lit, fusion::vector1<const char (&)[N]> >, Enable> : mpl::true_ {};

} }

#endif
