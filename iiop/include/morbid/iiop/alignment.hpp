/* (c) Copyright 2012,2013 Felipe Magno de Almeida
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

#include <boost/mpl/bool.hpp>

namespace morbid { namespace iiop {

namespace spirit = boost::spirit;
namespace karma = spirit::karma;
namespace qi = spirit::qi;
namespace fusion = boost::fusion;
namespace mpl = boost::mpl;

BOOST_SPIRIT_TERMINAL_EX(aligned);

template <typename OutputIterator>
struct alignment_attribute
{
  OutputIterator first;
  std::size_t offset;
};

template <typename OutputIterator>
std::ostream& operator<<(std::ostream& os, alignment_attribute<OutputIterator> align)
{
  return os << "[alignment_attribute offset: " << align.offset << "]";
}

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

namespace proto = boost::proto;

struct has_align_grammar
  : proto::or_
  <
    proto::subscript<proto::terminal<spirit::terminal_ex<tag::aligned, proto::_> >, proto::_>
  , proto::subscript<proto::terminal<tag::aligned>, proto::_>
  >
{
};

template <typename T>
struct is_aligned
  : proto::matches<T, has_align_grammar>
{
};

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
  std::cout << "alignment_padding for " << N/CHAR_BIT << " bytes" << std::endl;
  alignment_attribute<Iterator> align_from
    = get_alignment_attribute<Attributes, Iterator>::call(attributes);

  std::size_t distance = std::distance(align_from.first, first) + align_from.offset;
  std::cout << "position distance " << distance << std::endl;

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
struct specific_alignment_enabler : qi::unary_parser<specific_alignment_enabler<Subject> >
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
    std::cout << "specific_alignment_enabler::parse " << offset << std::endl;
    typedef typename Context::attributes_type attributes_type;
    typedef typename fusion::result_of::find<attributes_type, alignment_attribute<Iterator>
                                             >::type index_iterator_type;
    typedef typename fusion::result_of::distance
      <index_iterator_type, typename fusion::result_of::end<attributes_type>::type>::type distance_to_end;
    BOOST_MPL_ASSERT_RELATION(distance_to_end::value, ==, 0);

    typedef typename fusion::result_of::as_list
      <typename fusion::result_of::push_back
       <attributes_type, alignment_attribute<Iterator> >::type
       >::type
      alignment_attributes_type;
    typedef spirit::context
      <alignment_attributes_type, typename Context::locals_type> context_type;
    alignment_attribute<Iterator> e = { first, offset };
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

  specific_alignment_enabler(Subject const& subject, std::size_t offset)
    : subject(subject), offset(offset)
  {
  }

  Subject subject;
  std::size_t offset;
};

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
             , Attribute& attr, mpl::true_) const
  {
    std::cout << "alignment_enabler::parse" << std::endl;
    typedef typename Context::attributes_type attributes_type;
    typedef typename fusion::result_of::find<attributes_type, alignment_attribute<Iterator>
                                             >::type index_iterator_type;
    typedef typename fusion::result_of::distance
      <index_iterator_type, typename fusion::result_of::end<attributes_type>::type>::type distance_to_end;
    BOOST_MPL_ASSERT_RELATION(distance_to_end::value, ==, 0);

    typedef typename fusion::result_of::as_list
      <typename fusion::result_of::push_back
       <attributes_type, alignment_attribute<Iterator> >::type
       >::type
      alignment_attributes_type;
    typedef spirit::context
      <alignment_attributes_type, typename Context::locals_type> context_type;
    alignment_attribute<Iterator> e = { first, 0u };
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

  template <typename Iterator, typename Context, typename Skipper, typename Attribute>
  bool parse(Iterator& first, Iterator const& last
             , Context& ctx, Skipper const& skipper
             , Attribute& attr, mpl::false_) const
  {
    return subject.parse(first, last, ctx, skipper, attr);
  }

  template <typename Iterator, typename Context, typename Skipper, typename Attribute>
  bool parse(Iterator& first, Iterator const& last
             , Context& ctx, Skipper const& skipper
             , Attribute& attr) const
  {
    std::cout << "alignment_parser::parse ctx.attributes " << typeid(ctx.attributes).name()
              << std::endl;
    typedef typename fusion::result_of::find<typename Context::attributes_type
                                             , alignment_attribute<Iterator>
                                             >::type index_iterator_type;
    typedef typename fusion::result_of::end<typename Context::attributes_type>::type
      end_type;
    return parse(first, last, ctx, skipper, attr
                 , typename boost::is_same<index_iterator_type, end_type>::type());
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
    return result_type(subject);
  }
};

template <typename U, typename Subject, typename Modifiers>
struct make_directive<spirit::terminal_ex<tag::aligned, fusion::vector1<U> >, Subject, Modifiers>
{
  typedef specific_alignment_enabler<Subject> result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, Subject const& subject, boost::spirit::unused_type) const
  {
    return result_type(subject, fusion::at_c<0>(term.args));
  }
};

}

namespace generator {

template <std::size_t N, typename OutputIterator, typename Attributes>
void alignment_padding(OutputIterator& sink, Attributes const& attributes)
{
  std::cout << "alignment_padding for " << N/CHAR_BIT << " bytes of alignment" << std::endl;
  typedef typename output_iterator<OutputIterator>::type output_iterator;
  alignment_attribute<output_iterator> align_from
    = get_alignment_attribute<Attributes
                                    , output_iterator>::call(attributes);

  std::size_t distance = std::distance(align_from.first, sink.base()) + align_from.offset;
  std::cout << "alignment_padding distance from start " << distance << std::endl;
  const std::size_t alignment = N/CHAR_BIT;

  int remainder = distance % alignment;
  int padding = remainder? alignment - remainder : 0;
  std::cout << "padding bytes " << padding << std::endl;
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
    alignment_attribute<output_iterator> e = { sink.base(), offset };
    alignment_attributes_type attributes
      = fusion::as_list(fusion::push_back(ctx.attributes, e));
    context_type context(attributes);
    context.locals = ctx.locals;
    bool r = subject.generate(sink, context, d, attr);
    ctx.locals = context.locals;
    return r;
  }

  alignment_enabler(Subject const& subject, std::size_t offset = 0)
    : subject(subject), offset(offset)
  {
  }

  Subject subject;
  std::size_t offset;
};

template <typename Subject, typename Modifiers>
struct make_directive<tag::aligned, Subject, Modifiers>
{
  typedef alignment_enabler<Subject> result_type;

  result_type operator()(spirit::unused_type, Subject const& subject, boost::spirit::unused_type) const
  {
    return result_type(subject);
  }
};

template <typename U, typename Subject, typename Modifiers>
struct make_directive<spirit::terminal_ex<tag::aligned, fusion::vector1<U> >, Subject, Modifiers>
{
  typedef alignment_enabler<Subject> result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, Subject const& subject, boost::spirit::unused_type) const
  {
    return result_type(subject, fusion::at_c<0>(term.args));
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

template <typename U, typename Enable>
struct use_directive< ::morbid::iiop::generator_domain
                      , terminal_ex<morbid::iiop::tag::aligned, boost::fusion::vector1<U> >
                      , Enable> : mpl::true_ {};

template <typename U, typename Enable>
struct use_directive< ::morbid::iiop::parser_domain
                      , terminal_ex<morbid::iiop::tag::aligned, boost::fusion::vector1<U> >
                      , Enable> : mpl::true_ {};

} }

#endif
