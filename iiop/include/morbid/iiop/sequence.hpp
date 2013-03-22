/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_SEQUENCE_HPP
#define MORBID_IIOP_SEQUENCE_HPP

#include <morbid/iiop/meta_compiler.hpp>
#include <morbid/giop/common_terminals.hpp>

#include <boost/spirit/home/karma.hpp>

namespace morbid { namespace iiop {

namespace spirit = boost::spirit;
namespace karma = spirit::karma;
namespace qi = spirit::qi;

namespace parser {

template <typename Subject>
struct sequence_parser : qi::unary_parser<sequence_parser<Subject> >
{
  template <typename Context, typename Iterator>
  struct attribute
  {
    typedef typename
    spirit::traits::build_std_vector<
      typename spirit::traits::attribute_of<
        Subject, Context, Iterator>::type
      >::type
    type;
  };

  template <typename Iterator, typename Context, typename Skipper, typename Attribute>
  bool parse(Iterator& first, Iterator const& last
             , Context& ctx, Skipper const& skipper
             , Attribute& attr) const
  {
    std::cout << "sequence_parser::parse" << std::endl;

    unsigned_parser<32u> unsigned_;
    boost::uint_t<32u>::least size;
    bool r = unsigned_.parse(first, last, ctx, skipper, size);
    std::cout << "sequence size " << size << std::endl;

    if(!r)
      return false;

    for(;size && r;--size)
    {
      typename spirit::traits::container_value<Attribute>::type value;
      r = subject.parse(first, last, ctx, skipper, value);
      spirit::traits::push_back(attr, value);
    }
    return r;
  }

  sequence_parser(Subject const& subject)
    : subject(subject) 
  {
  }

  Subject subject;
};

template <typename Subject, typename Modifiers>
struct make_directive<giop::tag::sequence, Subject, Modifiers>
{
  typedef sequence_parser<Subject> result_type;

  result_type operator()(spirit::unused_type, Subject const& subject, boost::spirit::unused_type) const
  {
    return result_type(subject);
  }
};

}

namespace generator {

template <typename Subject>
struct sequence_generator : karma::unary_generator<sequence_generator<Subject> >
{
  template <typename Context, typename Iterator>
  struct attribute
  {
    typedef typename
    spirit::traits::build_std_vector<
      typename spirit::traits::attribute_of<
        Subject, Context, Iterator>::type
      >::type
    type;
  };

  template <typename OutputIterator, typename Context, typename Delimiter, typename C>
  bool generate(OutputIterator& sink, Context& ctx, Delimiter const& d, C& attr) const
  {
    unsigned_generator<32u> unsigned_;
    boost::uint_t<32>::least size = attr.size();
    bool r = unsigned_.generate(sink, ctx, d, size);
    for(typename spirit::traits::container_iterator<C>::type 
          first = spirit::traits::begin(attr)
          , last = spirit::traits::end(attr)
          ;first != last; ++first)
    {
      typename spirit::traits::container_value<C>::type value = *first;
      r = subject.generate(sink, ctx, d, value);
    }
    return r;
  }

  sequence_generator(Subject const& subject)
    : subject(subject) 
  {
  }

  Subject subject;
};

template <typename Subject, typename Modifiers>
struct make_directive<giop::tag::sequence, Subject, Modifiers>
{
  typedef sequence_generator<Subject> result_type;

  result_type operator()(spirit::unused_type, Subject const& subject, boost::spirit::unused_type) const
  {
    return result_type(subject);
  }
};

}

} }

namespace boost { namespace spirit {

// template <typename Enable>
// struct use_terminal< ::morbid::iiop::generator_domain, morbid::giop::tag::sequence, Enable> : mpl::true_ {};

template <typename Enable>
struct use_directive< ::morbid::iiop::generator_domain, morbid::giop::tag::sequence, Enable> : mpl::true_ {};

template <typename Enable>
struct use_directive< ::morbid::iiop::parser_domain, morbid::giop::tag::sequence, Enable> : mpl::true_ {};

namespace traits {

template <typename Subject, typename Attribute, typename Context
          , typename Iterator>
struct handles_container< ::morbid::iiop::parser::sequence_parser<Subject>, Attribute
                          , Context, Iterator>
: mpl::true_ {};

}

} }

#endif
