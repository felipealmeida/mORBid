/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_RAW_SIZE_HPP
#define MORBID_IIOP_RAW_SIZE_HPP

#include <boost/spirit/home/support.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/karma.hpp>

#include <boost/utility.hpp>

namespace morbid { namespace iiop {

namespace spirit = boost::spirit;
namespace karma = spirit::karma;
namespace qi = spirit::qi;

namespace generator {

template <typename Size, typename Subject>
struct raw_size_generator : karma::unary_generator<raw_size_generator<Size, Subject> >
{
  template <typename Context, typename Iterator>
  struct attribute : spirit::traits::attribute_of<Subject, Context, Iterator>
  {};

  raw_size_generator(Size const& size, Subject const& subject)
    : size(size), subject(subject) {}

  typedef typename Subject::properties properties;

  template <typename OutputIterator, typename Context, typename Delimiter, typename C>
  bool generate(OutputIterator& sink, Context& ctx, Delimiter const& d, C& attr) const
  {
    std::cout << "raw_size_generator::generate" << std::endl;
    std::vector<char> value;
    typedef giop::forward_back_insert_iterator<std::vector<char> > forward_iterator;
    BOOST_MPL_ASSERT((boost::is_same<typename iiop::output_iterator<OutputIterator>::type, forward_iterator>));
    typedef karma::detail::output_iterator
      <forward_iterator, properties> output_iterator;
    forward_iterator first(value);
    output_iterator buffered_sink (first);
    if(subject.generate(buffered_sink, ctx, d, attr))
    {
      typedef typename spirit::result_of::compile<generator_domain, Size>::type
        size_generator_type;
      size_generator_type size_generator = spirit::compile<generator_domain>(this->size);
      typedef typename spirit::traits::attribute_of<size_generator_type, Context, OutputIterator>::type size_attribute;
      size_attribute size = value.size();
      if(!size_generator.generate(sink, ctx, d, size))
        return false;
      
      sink = std::copy(value.begin(), value.end(), sink);
      return true;
    }
    return false;
  }

  Size size;
  Subject subject;
};

template <typename Size, typename Subject, typename Modifiers>
struct make_directive<spirit::terminal_ex<giop::tag::raw_size, boost::fusion::vector1<Size> >
                      , Subject, Modifiers>
{
  typedef raw_size_generator<Size, Subject> result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, Subject const& subject, spirit::unused_type) const
  {
    return result_type(fusion::at_c<0>(term.args), subject);
  }
};

}

namespace parser {

template <typename Size, typename Subject>
struct raw_size_parser : qi::unary_parser<raw_size_parser<Size, Subject> >
{
  template <typename Context, typename Iterator>
  struct attribute : spirit::traits::attribute_of<Subject, Context, Iterator>
  {};

  raw_size_parser(Size const& size, Subject const& subject)
    : size(size), subject(subject) {}

  template <typename Iterator, typename Context, typename Skipper, typename Attribute>
  bool parse(Iterator& first, Iterator last
             , Context& ctx, Skipper const& skipper
             , Attribute& attr) const
  {
    std::cout << "raw size parser" << std::endl;
    typedef typename spirit::result_of::compile<parser_domain, Size>::type
      size_parser_type;
    size_parser_type size_parser = spirit::compile<parser_domain>(this->size);
    typedef typename spirit::traits::attribute_of<size_parser_type, Context, Iterator>::type size_attribute;
    size_attribute size;
    if(!size_parser.parse(first, last, ctx, skipper, size))
      return false;

    std::cout << "Size of the input for subject " << size << std::endl;

    if(std::distance(first, last) < size)
      return false;

    last = boost::next(first, size);
    bool r = subject.parse(first, last, ctx, skipper, attr);
    if(r) first = last;
    return r;
  }  
  
  Size size;
  Subject subject;
};

template <typename Size, typename Subject, typename Modifiers>
struct make_directive<spirit::terminal_ex<giop::tag::raw_size, boost::fusion::vector1<Size> >
                      , Subject, Modifiers>
{
  typedef raw_size_parser<Size, Subject> result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, Subject const& subject, spirit::unused_type) const
  {
    return result_type(fusion::at_c<0>(term.args), subject);
  }
};

}

} }

namespace boost { namespace spirit {

namespace traits {

template <typename Size, typename Subject>
struct has_semantic_action< ::morbid::iiop::generator::raw_size_generator<Size, Subject> >
  : unary_has_semantic_action<Subject> {};

template <typename Size, typename Subject, typename Attribute, typename Context
          , typename Iterator>
struct handles_container< ::morbid::iiop::generator::raw_size_generator<Size, Subject>, Attribute
                          , Context, Iterator>
  : unary_handles_container<Subject, Attribute, Context, Iterator> {};

}

// Parser
template <>
struct use_lazy_directive< ::morbid::iiop::parser_domain, morbid::giop::tag::raw_size, 1>
  : mpl::true_ {};
template <typename Size, typename Enable>
struct use_directive< ::morbid::iiop::parser_domain
                      , terminal_ex<morbid::giop::tag::raw_size, boost::fusion::vector1<Size> >
                      , Enable> : mpl::true_ {};

// Generator
template <>
struct use_lazy_directive< ::morbid::iiop::generator_domain, morbid::giop::tag::raw_size, 1>
  : mpl::true_ {};
template <typename Size, typename Enable>
struct use_directive< ::morbid::iiop::generator_domain
                      , terminal_ex<morbid::giop::tag::raw_size, boost::fusion::vector1<Size> >
                      , Enable> : mpl::true_ {};

} }

#endif
