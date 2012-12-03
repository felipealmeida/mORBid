/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_BUFFER_HPP
#define MORBID_IIOP_BUFFER_HPP

#include <morbid/iiop/meta_compiler.hpp>


#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/support.hpp>


namespace morbid { namespace iiop {

namespace spirit = boost::spirit;
namespace karma = spirit::karma;

BOOST_SPIRIT_TERMINAL_EX(buffer);

namespace parser {

struct buffer_parser : qi::primitive_parser<buffer_parser>
{
  template <typename Context, typename Unused>
  struct attribute
  {
    typedef std::vector<char> type;
  };
  
  template <typename Iterator, typename Context, typename Skipper, typename Attribute>
  bool parse(Iterator& first, Iterator const& last
             , Context& ctx, Skipper const& skipper
             , Attribute& attr) const
  {
    for(;first != last;++first)
    {
      spirit::traits::push_back(attr, *first);
    }
    return true;
  }
};

template <typename Modifiers, typename Enable>
struct make_primitive<tag::buffer, Modifiers, Enable>
{
  typedef buffer_parser result_type;

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
struct use_terminal< ::morbid::iiop::generator_domain, ::morbid::iiop::tag::buffer, Enable> : mpl::true_ {};

// Parser
template <typename Enable>
struct use_terminal< ::morbid::iiop::parser_domain, ::morbid::iiop::tag::buffer, Enable> : mpl::true_ {};

} }

#endif
