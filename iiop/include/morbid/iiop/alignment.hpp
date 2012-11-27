/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_ALIGNMENT_HPP
#define MORBID_IIOP_ALIGNMENT_HPP

#include <morbid/iiop/meta_compiler.hpp>
#include <morbid/iiop/output_iterator.hpp>

#include <morbid/giop/common_terminals.hpp>

namespace morbid { namespace iiop {

namespace spirit = boost::spirit;
namespace karma = spirit::karma;
namespace qi = spirit::qi;
namespace fusion = boost::fusion;

BOOST_SPIRIT_TERMINAL_EX(aligned);

template <typename OutputIterator>
struct alignment_attribute
{
  OutputIterator first;
};

} }

namespace boost {

template <typename I>
struct is_scalar< ::morbid::iiop::alignment_attribute<I> > : mpl::true_
{};

template <typename I>
struct is_scalar< ::morbid::iiop::alignment_attribute<I> const> : mpl::true_
{};

}

namespace morbid { namespace iiop {

template <typename Attributes, typename OutputIterator>
struct get_alignment_attribute
{
  typedef Attributes attributes_type;
  typedef typename fusion::result_of::find<attributes_type, alignment_attribute<OutputIterator>
                                           >::type index_iterator_type;
  typedef typename fusion::result_of::distance
    <index_iterator_type, typename fusion::result_of::end<attributes_type>::type>::type distance_to_end;
  BOOST_MPL_ASSERT_RELATION(distance_to_end::value, !=, 0);
  typedef typename fusion::result_of::distance
    <typename fusion::result_of::begin<attributes_type>::type, index_iterator_type>::type index_type;

  static alignment_attribute<OutputIterator> call(Attributes const& attributes)
  {
    return fusion::at_c<index_type::value>(attributes);
  }
};

namespace parser {

template <std::size_t N, typename Iterator, typename Attributes>
bool alignment_padding(Iterator& first, Iterator last, Attributes const& attributes)
{
  alignment_attribute<Iterator> align_from
    = get_alignment_attribute<Attributes, Iterator>::call(attributes);

  std::size_t distance = std::distance(align_from.first, first);

  const std::size_t alignment = N/CHAR_BIT;

  int remainder = distance % alignment;
  int padding = remainder? alignment - remainder : 0;
  for(;padding && first != last; --padding)
  {
    std::cout << "padding byte" << std::endl;
    ++first;
  }
  return first != last;
}

template <typename Subject>
struct alignment_enabler : qi::unary_parser<alignment_enabler<Subject> >
{
  template <typename Context, typename Iterator>
  struct attribute : spirit::traits::attribute_of<Subject, Context, Iterator>
  {
  };

  template <typename Iterator, typename Context, typename Skipper, typename Attribute>
  bool parse(Iterator& first, Iterator const& last
             , Context& ctx, Skipper const& skipper
             , Attribute& attr) const
  {
    typedef typename Context::attributes_type attributes_type;
    typedef typename fusion::result_of::as_list
      <typename fusion::result_of::push_back
       <attributes_type, alignment_attribute<Iterator> >::type
       >::type
      alignment_attributes_type;
    typedef spirit::context
      <alignment_attributes_type, typename Context::locals_type> context_type;
    alignment_attribute<Iterator> e = { first };
    alignment_attributes_type attributes
      = fusion::as_list(fusion::push_back(ctx.attributes, e));
    context_type context(attributes);
    context.locals = ctx.locals;

    if(subject.parse(first, last, context, skipper, attr))
    {
      ctx.locals = context.locals;
      return true;
    }
    return false;
  }

  alignment_enabler(Subject const& subject)
    : subject(subject) 
  {
  }

  Subject subject;
};

template <typename Subject, typename Modifiers>
struct make_directive<tag::aligned, Subject, Modifiers>
{
  typedef alignment_enabler<Subject> result_type;

  result_type operator()(spirit::unused_type, Subject const& subject, boost::spirit::unused_type) const
  {
    std::cout << "Modifiers " << typeid(Modifiers).name() << std::endl;
    return result_type(subject);
  }
};

}

namespace generator {

template <std::size_t N, typename OutputIterator, typename Attributes>
void alignment_padding(OutputIterator& sink, Attributes const& attributes)
{
  typedef typename output_iterator<OutputIterator>::type output_iterator;
  alignment_attribute<output_iterator> align_from
    = get_alignment_attribute<Attributes
                                    , output_iterator>::call(attributes);

  std::size_t distance = std::distance(align_from.first, sink.base());

  const std::size_t alignment = N/CHAR_BIT;

  int remainder = distance % alignment;
  int padding = remainder? alignment - remainder : 0;
  for(;padding != 0; --padding)
  {
    std::cout << "padding byte" << std::endl;
    *sink = '\0';
    ++sink;
  }
}

template <typename Subject>
struct alignment_enabler : karma::unary_generator<alignment_enabler<Subject> >
{
  template <typename Context, typename Iterator>
  struct attribute : spirit::traits::attribute_of<Subject, Context, Iterator>
  {
  };

  typedef typename Subject::properties properties;

  template <typename OutputIterator, typename Context, typename Delimiter, typename C>
  bool generate(OutputIterator& sink, Context& ctx, Delimiter const& d, C& attr) const
  {
    typedef typename output_iterator<OutputIterator>::type output_iterator;
    typedef typename Context::attributes_type attributes_type;
    typedef typename fusion::result_of::as_list
      <typename fusion::result_of::push_back
       <attributes_type, alignment_attribute<output_iterator> >::type
       >::type
      alignment_attributes_type;
    typedef spirit::context
      <alignment_attributes_type, typename Context::locals_type> context_type;
    alignment_attribute<output_iterator> e = { sink.base() };
    alignment_attributes_type attributes
      = fusion::as_list(fusion::push_back(ctx.attributes, e));
    context_type context(attributes);
    context.locals = ctx.locals;
    bool r = subject.generate(sink, context, d, attr);
    ctx.locals = context.locals;
    return r;
  }

  alignment_enabler(Subject const& subject)
    : subject(subject) 
  {
  }

  Subject subject;
};

template <typename Subject, typename Modifiers>
struct make_directive<tag::aligned, Subject, Modifiers>
{
  typedef alignment_enabler<Subject> result_type;

  result_type operator()(spirit::unused_type, Subject const& subject, boost::spirit::unused_type) const
  {
    std::cout << "Modifiers " << typeid(Modifiers).name() << std::endl;

    return result_type(subject);
  }
};

} } }

namespace boost { namespace spirit {

namespace traits {

template <typename Subject>
struct has_semantic_action< ::morbid::iiop::generator::alignment_enabler<Subject> >
  : unary_has_semantic_action<Subject> {};

template <typename Subject, typename Attribute, typename Context
          , typename Iterator>
struct handles_container< ::morbid::iiop::generator::alignment_enabler<Subject>, Attribute
                          , Context, Iterator>
: unary_handles_container<Subject, Attribute, Context, Iterator> {};

}

template <typename Enable>
struct use_directive< ::morbid::iiop::generator_domain, morbid::iiop::tag::aligned, Enable> : mpl::true_ {};

template <typename Enable>
struct use_directive< ::morbid::iiop::parser_domain, morbid::iiop::tag::aligned, Enable> : mpl::true_ {};

} }

#endif
