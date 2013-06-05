/* (c) Copyright 2012,2013 Felipe Magno de Almeida
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
namespace spirit = boost::spirit;
namespace qi = spirit::qi;
namespace karma = spirit::karma;

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

struct bool_encoding
{
  typedef bool char_type;

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

namespace parser {



}

namespace parser {

template <typename Modifiers, typename Enable>
struct make_primitive<giop::tag::octet, Modifiers, Enable>
{
  typedef qi::char_class<spirit::tag::char_code
                         <spirit::tag::char_, octet_encoding> > result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

template <typename Modifiers, typename Enable>
struct make_primitive<spirit::tag::bool_, Modifiers, Enable>
{
  typedef qi::char_class<spirit::tag::char_code
                         <spirit::tag::char_, bool_encoding> > result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

template <typename U, typename Modifiers, typename Enable>
struct make_primitive<spirit::terminal_ex<giop::tag::octet, fusion::vector1<U> >
                      , Modifiers, Enable>
{
  typedef qi::literal_char<octet_encoding, true> result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, boost::spirit::unused_type) const
  {
    return result_type(fusion::at_c<0>(term.args));
  }
};


template <typename T, typename Modifiers>
struct make_primitive<T, Modifiers, typename boost::enable_if
                      <spirit::traits::is_string<T>, void>::type>
{
  typedef typename boost::add_const<T>::type const_string;
  typedef qi::literal_string<const_string, true>
    result_type;

  result_type operator()(typename boost::add_reference<const_string>::type val
                         , boost::spirit::unused_type) const
  {
    return result_type(val);
  }
};

}

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

template <typename U, typename Modifiers, typename Enable>
struct make_primitive<spirit::terminal_ex<giop::tag::octet, fusion::vector1<U> >
                      , Modifiers, Enable>
{
  typedef karma::literal_char<octet_encoding, spirit::unused_type, true> result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, boost::spirit::unused_type) const
  {
    return result_type(fusion::at_c<0>(term.args));
  }
};

template <typename Modifiers, typename Enable>
struct make_primitive<spirit::tag::bool_, Modifiers, Enable>
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

// Generator
template <typename Enable>
struct use_terminal< ::morbid::iiop::generator_domain, tag::bool_, Enable> : mpl::true_ {};
template <typename Enable>
struct use_terminal< ::morbid::iiop::generator_domain, morbid::giop::tag::octet, Enable> : mpl::true_ {};
template <typename U, typename Enable>
struct use_terminal< ::morbid::iiop::generator_domain
                     , terminal_ex< ::morbid::giop::tag::octet, fusion::vector1<U> >, Enable> : mpl::true_ {};
template <typename T>
struct use_terminal< ::morbid::iiop::generator_domain
                     , T, typename enable_if<traits::is_string<T>, void>::type> : mpl::true_ {};

// Parser
template <typename Enable>
struct use_terminal< ::morbid::iiop::parser_domain, tag::bool_, Enable> : mpl::true_ {};
template <typename Enable>
struct use_terminal< ::morbid::iiop::parser_domain, morbid::giop::tag::octet, Enable> : mpl::true_ {};
template <typename U, typename Enable>
struct use_terminal< ::morbid::iiop::parser_domain
                     , terminal_ex< ::morbid::giop::tag::octet, fusion::vector1<U> >, Enable> : mpl::true_ {};
template <typename T>
struct use_terminal< ::morbid::iiop::parser_domain
                     , T, typename enable_if<traits::is_string<T>, void>::type> : mpl::true_ {};

} }

#endif
