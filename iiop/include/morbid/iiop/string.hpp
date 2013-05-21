/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_STRING_HPP
#define MORBID_IIOP_STRING_HPP

#include <morbid/iiop/meta_compiler.hpp>
#include <morbid/iiop/numeric.hpp>
#include <morbid/iiop/octet.hpp>

#include <morbid/giop/common_terminals.hpp>

#include <boost/spirit/home/karma.hpp>

namespace morbid { namespace iiop {

namespace spirit = boost::spirit;
namespace karma = spirit::karma;

namespace generator {

struct string_generator : karma::primitive_generator<string_generator>
{
  template <typename Context, typename Unused>
  struct attribute
  {
    typedef std::string type;
  };

  template <typename OutputIterator, typename Context, typename Delimiter, typename C>
  bool generate(OutputIterator& sink, Context& ctx, Delimiter const& d, C& attr) const
  {
    unsigned_generator<32u> unsigned_;
    boost::uint_t<32>::least size = attr.size()+1;
    bool r = unsigned_.generate(sink, ctx, d, size);
    karma::any_char<octet_encoding, spirit::unused_type> octet_generator;
    for(typename spirit::traits::container_iterator<C>::type 
          first = spirit::traits::begin(attr)
          , last = spirit::traits::end(attr)
          ;first != last; ++first)
    {
      octet_encoding::char_type c = *first;
      octet_generator.generate(sink, ctx, d, c);
    }
    octet_encoding::char_type zero = '\0';
    octet_generator.generate(sink, ctx, d, zero);
    return r;
  }
};

template <typename Modifiers, typename Enable>
struct make_primitive<giop::tag::string, Modifiers, Enable>
{
  typedef string_generator result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

}

namespace parser {

struct string_parser : qi::primitive_parser<string_parser>
{
  template <typename Context, typename Unused>
  struct attribute
  {
    typedef std::string type;
  };

  template <typename Iterator, typename Context, typename Skipper, typename Attribute>
  bool parse(Iterator& first, Iterator const& last
             , Context& ctx, Skipper const& skipper
             , Attribute& attr) const
  {
    // std::cout << "string_parser::parse" << std::endl;

    unsigned_parser<32u> unsigned_;
    boost::uint_t<32u>::least size;
    bool r = unsigned_.parse(first, last, ctx, skipper, size);
    // std::cout << "string size " << size << std::endl;

    if(!r)
      return false;

    qi::char_class<spirit::tag::char_code
                   <spirit::tag::char_, octet_encoding> > octet_parser;
    octet_encoding::char_type c = 0;
    for(;size > 1 && r;--size)
    {
      r = octet_parser.parse(first, last, ctx, skipper, c);
      spirit::traits::push_back(attr, c);
    }
    if(size != 1 || !r)
      return false;
    r = octet_parser.parse(first, last, ctx, skipper, c);
    return r && c == 0;
  }
};

template <typename Modifiers, typename Enable>
struct make_primitive<giop::tag::string, Modifiers, Enable>
{
  typedef string_parser result_type;

  result_type operator()(spirit::unused_type val, spirit::unused_type) const
  {
    return result_type();
  }
};

}

} }

namespace boost { namespace spirit {

template <typename Enable>
struct use_terminal< ::morbid::iiop::generator_domain, morbid::giop::tag::string, Enable>
  : mpl::true_ {};

template <typename Enable>
struct use_terminal< ::morbid::iiop::parser_domain, morbid::giop::tag::string, Enable>
  : mpl::true_ {};

namespace traits {

template <typename Attribute, typename Context, typename Iterator>
struct handles_container< ::morbid::iiop::parser::string_parser, Attribute
                          , Context, Iterator>
: mpl::true_ {};

template <typename Attribute, typename Context, typename Iterator>
struct handles_container< ::morbid::iiop::generator::string_generator, Attribute
                          , Context, Iterator>
: mpl::true_ {};

}

} }

#endif
