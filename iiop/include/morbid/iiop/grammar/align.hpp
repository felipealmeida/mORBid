/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IIOP_GRAMMAR_ALIGN_HPP
#define TECORB_IIOP_GRAMMAR_ALIGN_HPP

#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>
#include <boost/spirit/home/support.hpp>

namespace morbid { namespace iiop { namespace grammar {

namespace qi = boost::spirit::qi;

BOOST_SPIRIT_TERMINAL_EX(align)

template <typename Iterator>
struct align_parser : qi::primitive_parser<align_parser<Iterator> >
{
  template <typename Context, typename Iterator1>
  struct attribute
  {
    typedef qi::unused_type type;
  };

  align_parser(Iterator first_iterator, unsigned int align)
    : first_iterator(first_iterator), align(align)
  {}

  template <typename Context, typename Skipper, typename Attribute>
  bool parse(Iterator& first, Iterator const& last
             , Context& context, Skipper const& skipper
             , Attribute& attr) const
  {
    std::size_t dist = std::distance(first_iterator, first);
    int div_dif = dist % align;
    int align_dif = div_dif? 4 - div_dif : 0;
    if(align_dif <= std::distance(first, last))
    {
      std::advance(first, align_dif);
      return true;
    }
    else
      return false;
  }

  Iterator first_iterator;
  unsigned int align;
};

} } }

namespace boost { namespace spirit {

  ///////////////////////////////////////////////////////////////////////////
  // Enablers
  ///////////////////////////////////////////////////////////////////////////

template <typename Iterator, typename T>
struct use_terminal
  <qi::domain
   , terminal_ex<morbid::iiop::grammar::tag::align, fusion::vector2<Iterator, T> > >
  : mpl::true_ {};

template <>
struct use_lazy_terminal<qi::domain, morbid::iiop::grammar::tag::align, 2>
  : mpl::true_ {};

} } // boost::spirit

namespace boost { namespace spirit { namespace qi {

template <typename Modifiers, typename Iterator, typename T>
struct make_primitive
  <terminal_ex<morbid::iiop::grammar::tag::align, fusion::vector2<Iterator, T> >
   , Modifiers>
{
  typedef morbid::iiop::grammar::align_parser<Iterator> result_type;
  template <typename Terminal>
  result_type operator()(const Terminal& term, unused_type) const
  {
    return result_type(fusion::at_c<0>(term.args), fusion::at_c<1>(term.args));
  }
};

} } } // boost::spirit::qi

#endif
