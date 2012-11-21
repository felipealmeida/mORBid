/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_GENERATOR_ALIGN_HPP
#define MORBID_IIOP_GENERATOR_ALIGN_HPP

#include <boost/spirit/home/karma.hpp>

#include <vector>

namespace morbid { namespace iiop { namespace generator {

namespace karma = boost::spirit::karma;

BOOST_SPIRIT_TERMINAL_EX(align)
BOOST_SPIRIT_TERMINAL_EX(alignable)

struct align_generator : karma::primitive_generator<align_generator>
{
  template <typename Context, typename Unused>
  struct attribute
  {
    typedef karma::unused_type type;
  };

  align_generator(unsigned int align)
    : align(align)
  {}

  template <typename OutputIterator, typename Context, typename Delimiter>
  bool generate(OutputIterator& sink, Context&
                , Delimiter const&, karma::unused_type) const
  {
    karma::detail::counting_sink<OutputIterator>* count
      = sink.chain_counting(0);
    sink.chain_counting(count);

    if(!count)
      throw std::runtime_error("Using align without alignable");

    std::size_t dist = count->get_count();
    int div_dif = dist % align;
    int align_dif = div_dif? align - div_dif : 0;
    for(;align_dif != 0; --align_dif)
      *sink++ = '\0';
    return true;
  }

  unsigned int align;
};

template <typename Subject>
struct alignable_generator : karma::unary_generator<alignable_generator<Subject> >
{
  typedef Subject subject_type;
  enum
  {
    properties = karma::generator_properties::countingbuffer
    | subject_type::properties::value
  };

  template <typename Context, typename Iterator>
  struct attribute : boost::spirit::traits
    ::attribute_of<subject_type, Context, Iterator>
  {};

  alignable_generator(Subject const& subject)
    : subject(subject) {}

  template <typename OutputIterator, typename Context
            , typename Delimiter, typename Attribute>
  bool generate(OutputIterator& sink, Context& ctx
                , Delimiter const& delimiter, Attribute const& attr) const
  {
    std::cout << "alignable_generator" << std::endl;
    boost::spirit::karma::detail
      ::enable_counting<OutputIterator> counting(sink);
    bool b = subject.generate(sink, ctx, delimiter, attr);
    std::cout << "Finished: " << counting.count() << std::endl;
    return b;
  }

  template <typename Context>
  karma::info what(Context& context) const
  {
    return karma::info("alignable", subject.what(context));
  }

  Subject subject;
};

} } }

namespace boost { namespace spirit {

namespace traits
{
    ///////////////////////////////////////////////////////////////////////////
    template <typename Subject>
    struct has_semantic_action< ::morbid::iiop::generator::alignable_generator<Subject> >
      : unary_has_semantic_action<Subject> {};

    ///////////////////////////////////////////////////////////////////////////
    template <typename Subject, typename Attribute, typename Context
        , typename Iterator>
    struct handles_container< ::morbid::iiop::generator::alignable_generator<Subject>, Attribute
        , Context, Iterator>
      : unary_handles_container<Subject, Attribute, Context, Iterator> {};
}

  ///////////////////////////////////////////////////////////////////////////
  // Enablers
  ///////////////////////////////////////////////////////////////////////////

template <typename Integer>
struct use_terminal
  <karma::domain
   , terminal_ex<morbid::iiop::generator::tag::align, fusion::vector1<Integer> > >
  : mpl::true_ {};

template <typename T>
struct use_terminal
  <karma::domain
   , terminal_ex<morbid::iiop::generator::tag::alignable, fusion::vector1<T> > >
  : mpl::true_ {};

template <>
struct use_directive<karma::domain, morbid::iiop::generator::tag::alignable>
  : mpl::true_ {};

template <>
struct use_lazy_terminal<karma::domain, morbid::iiop::generator::tag::align, 1>
  : mpl::true_ {};

} } // boost::spirit

namespace boost { namespace spirit { namespace karma {

template <typename Modifiers, typename Integer>
struct make_primitive
<terminal_ex<morbid::iiop::generator::tag::align, fusion::vector1<Integer> >
   , Modifiers>
{
  typedef morbid::iiop::generator::align_generator result_type;
  template <typename Terminal>
  result_type operator()(const Terminal& term, unused_type) const
  {
    return result_type(fusion::at_c<0>(term.args));
  }
};

template <typename Subject, typename Modifiers>
struct make_directive<morbid::iiop::generator::tag::alignable, Subject, Modifiers>
{
  typedef morbid::iiop::generator::alignable_generator<Subject> result_type;

  result_type operator()(karma::unused_type, Subject const& subject
                         , karma::unused_type) const
  {
    return result_type (subject);
  }
};

} } } // boost::spirit::karma

#endif
