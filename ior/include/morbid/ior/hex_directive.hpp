/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IOR_HEX_DIRECTIVE_HPP
#define MORBID_IOR_HEX_DIRECTIVE_HPP

#include <morbid/ior/hex_directive.hpp>
#include <morbid/iiop/output_iterator.hpp>

#include <boost/spirit/home/support.hpp>
#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/qi.hpp>

#include <boost/algorithm/hex.hpp>

namespace morbid { namespace ior {

BOOST_SPIRIT_TERMINAL_EX(hex);

namespace spirit = boost::spirit;
namespace karma = spirit::karma;
namespace qi = spirit::qi;

template <typename Subject>
struct hex_generator : karma::unary_generator<hex_generator<Subject> >
{
  template <typename Context, typename Iterator>
  struct attribute : spirit::traits::attribute_of<Subject, Context, Iterator> {};
  
  typedef typename Subject::properties properties;

  hex_generator(Subject const& subject)
    : subject(subject) {}

  template <typename OutputIterator, typename Context, typename Delimiter, typename C>
  bool generate(OutputIterator& sink, Context& ctx, Delimiter const& d, C& attr) const
  {
    std::vector<char> value;
    typedef giop::forward_back_insert_iterator<std::vector<char> > forward_iterator;
    BOOST_MPL_ASSERT((boost::is_same<typename iiop::output_iterator<OutputIterator>::type, forward_iterator>));
    typedef karma::detail::output_iterator
      <forward_iterator, properties> output_iterator;
    forward_iterator first(value);
    output_iterator buffered_sink (first);
    if(subject.generate(buffered_sink, ctx, d, attr))
    {
      boost::algorithm::hex(value.begin(), value.end(), sink);
      return true;
    }
    return false;
  }

  Subject subject;
};

template <typename Subject, typename C = std::vector<char> >
struct hex_parser : qi::unary_parser<hex_parser<Subject> >
{
  template <typename Context, typename Iterator>
  struct attribute : spirit::traits::attribute_of<Subject, Context, Iterator> {};
  
  hex_parser(Subject const& subject)
    : subject(subject) {}

  template <typename Iterator, typename Context, typename Skipper, typename Attribute>
  bool parse(Iterator& first, Iterator const& last
             , Context& context, Skipper const& skipper
             , Attribute& attr) const
  {
    // std::cout << "hex_parser::parse" << std::endl;
    BOOST_MPL_ASSERT((boost::is_convertible<typename std::iterator_traits<Iterator>::iterator_category
                      , std::forward_iterator_tag>));
    C data;
    Iterator first_tmp = first;
    std::back_insert_iterator<C> sink(data);
    boost::algorithm::unhex(first_tmp, last, sink);
    typename C::const_iterator first_ = data.begin()
      , first_tmp_ = first_
      , last_ = data.end();
    bool r = subject.parse(first_tmp_, last_, context, skipper, attr);
    if(r)
    {
      std::advance(first, std::distance(first_, first_tmp_)*2);
    }
    return r;
  }

  Subject subject;
};

} }

namespace boost { namespace spirit {

namespace karma {

template <typename Subject, typename Modifiers>
struct make_directive< ::morbid::ior::tag::hex, Subject, Modifiers>
{
  typedef ::morbid::ior::hex_generator<Subject> result_type;

  result_type operator()(spirit::unused_type, Subject const& subject, boost::spirit::unused_type) const
  {
    return result_type(subject);
  }
};

}

namespace qi {

template <typename Subject, typename Modifiers>
struct make_directive< ::morbid::ior::tag::hex, Subject, Modifiers>
{
  typedef ::morbid::ior::hex_parser<Subject> result_type;

  result_type operator()(spirit::unused_type, Subject const& subject, boost::spirit::unused_type) const
  {
    return result_type(subject);
  }
};

}

namespace traits {

template <typename Subject>
struct has_semantic_action< ::morbid::ior::hex_generator<Subject> >
  : unary_has_semantic_action<Subject> {};

template <typename Subject, typename Attribute, typename Context
          , typename Iterator>
struct handles_container< ::morbid::ior::hex_generator<Subject>, Attribute
                          , Context, Iterator>
: unary_handles_container<Subject, Attribute, Context, Iterator> {};

}

template <typename Enable>
struct use_directive< karma::domain, morbid::ior::tag::hex, Enable> : mpl::true_ {};

template <typename Enable>
struct use_directive< qi::domain, morbid::ior::tag::hex, Enable> : mpl::true_ {};

} }

#endif
