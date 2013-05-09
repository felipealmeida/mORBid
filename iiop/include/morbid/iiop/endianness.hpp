/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_ENDIANNESS_HPP
#define MORBID_IIOP_ENDIANNESS_HPP

#include <morbid/giop/rule.hpp>
#include <morbid/iiop/meta_compiler.hpp>
#include <morbid/iiop/octet.hpp>

#include <morbid/giop/common_terminals.hpp>

namespace morbid { namespace iiop {

namespace spirit = boost::spirit;
namespace karma = spirit::karma;
namespace fusion = boost::fusion;

BOOST_SPIRIT_TERMINAL_EX(save_endian);

struct endianness_attribute
{
  endianness_attribute() : endianness(0) {}
  endianness_attribute(giop::little_endian_type)
    : endianness(true) {}
  endianness_attribute(giop::big_endian_type)
    : endianness(false) {}
  endianness_attribute(giop::endian e)
    : endianness(e.b) {}

  bool endianness;
};

} }

namespace boost {

template <>
struct is_scalar< ::morbid::iiop::endianness_attribute> : mpl::true_
{};

template <>
struct is_scalar< ::morbid::iiop::endianness_attribute const> : mpl::true_
{};

}

namespace morbid { namespace iiop {

namespace mpl = boost::mpl;

template <typename Attributes>
struct generator_endianness
{
  typedef Attributes attributes_type;
  typedef typename fusion::result_of::find<attributes_type, iiop::endianness_attribute>::type index_iterator_type;
  typedef typename fusion::result_of::distance
    <index_iterator_type, typename fusion::result_of::end<attributes_type>::type>::type distance_to_end;
  BOOST_MPL_ASSERT_RELATION(distance_to_end::value, !=, 0);
  typedef typename fusion::result_of::distance
    <typename fusion::result_of::begin<attributes_type>::type, index_iterator_type>::type index_type;


  static bool& call(Attributes& attributes)
  {
    std::cout << "reading endianness " << fusion::at_c<index_type::value>(attributes).endianness << std::endl;
    return fusion::at_c<index_type::value>(attributes).endianness;
  }
};

namespace parser {

template <typename Subject>
struct endianness_parser : qi::unary_parser<endianness_parser<Subject> >
{
  endianness_parser(Subject const& subject) : subject(subject) {}

  template <typename Context, typename Iterator>
  struct attribute : spirit::traits::attribute_of<Subject, Context, Iterator>
  {
  };

  template <typename Iterator, typename Context, typename Skipper, typename Attribute>
  bool parse(Iterator& first, Iterator const& last
             , Context& ctx, Skipper const& skipper
             , Attribute& attr, boost::mpl::true_) const
  {
    qi::char_class<spirit::tag::char_code
                   <spirit::tag::char_, octet_encoding> > octet_parser;

    typedef typename Context::attributes_type attributes_type;
    typedef typename fusion::result_of::as_list
      <typename fusion::result_of::push_back
       <attributes_type, endianness_attribute>::type
       >::type
      endianness_attributes_type;
    typedef spirit::context
      <endianness_attributes_type, typename Context::locals_type> context_type;

    unsigned char endianness;
    if(!octet_parser.parse(first, last, ctx, skipper, endianness))
      return false;

    endianness_attribute e;
    e.endianness = endianness;
    endianness_attributes_type attributes
      = fusion::as_list(fusion::push_back(ctx.attributes, e));
    context_type context(attributes);
    context.locals = ctx.locals;
    bool r = subject.parse(first, last, context, skipper, attr);
    ctx.locals = context.locals;
    return r;
  }  

  template <typename Iterator, typename Context, typename Skipper, typename Attribute>
  bool parse(Iterator& first, Iterator const& last
             , Context& ctx, Skipper const& skipper
             , Attribute& attr, boost::mpl::false_) const
  {
    std::cout << "endianness_parser::parse already has this attribute" << std::endl;
    qi::char_class<spirit::tag::char_code
                   <spirit::tag::char_, octet_encoding> > octet_parser;
    unsigned char endianness;
    if(!octet_parser.parse(first, last, ctx, skipper, endianness))
      return false;
    std::cout << "endianness read from stream " << (unsigned int)endianness << std::endl;
    typedef generator_endianness<typename Context::attributes_type> getter_endianness;
    bool old_endianness = getter_endianness::call(ctx.attributes);
    getter_endianness::call(ctx.attributes) = endianness;
    bool r = subject.parse(first, last, ctx, skipper, attr);
    getter_endianness::call(ctx.attributes) = old_endianness;
    return r;
  }

  template <typename Iterator, typename Context, typename Skipper, typename Attribute>
  bool parse(Iterator& first, Iterator const& last
             , Context& ctx, Skipper const& skipper
             , Attribute& attr) const
  {
    typedef typename Context::attributes_type attributes_type;
    typedef typename fusion::result_of::find<attributes_type, iiop::endianness_attribute>::type index_iterator_type;
    typedef typename fusion::result_of::distance
      <index_iterator_type, typename fusion::result_of::end<attributes_type>::type>::type distance_to_end;
    return parse(first, last, ctx, skipper, attr
                 , typename mpl::equal_to<distance_to_end, mpl::int_<0> >::type());
  }

  typedef Subject subject_type;
  Subject subject;
};

struct save_endian_parser : qi::unary_parser<save_endian_parser>
{
  template <typename Context, typename Unused>
  struct attribute
  {
    typedef bool type;
  };

  template <typename Iterator, typename Context, typename Skipper, typename Attribute>
  bool parse(Iterator& first, Iterator const& last
             , Context& ctx, Skipper const& skipper
             , Attribute& attr) const
  {
    attr = generator_endianness<typename Context::attributes_type>
      ::call(ctx.attributes);
    return true;
  }
};

template <typename Modifiers, typename Enable>
struct make_primitive<tag::save_endian, Modifiers, Enable>
{
  typedef save_endian_parser result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

template <typename Subject>
struct specific_endianness_parser : endianness_parser<Subject>
{
  
};

template <typename Subject, typename Modifiers>
struct make_directive<giop::tag::endianness, Subject, Modifiers>
{
  typedef endianness_parser<Subject> result_type;

  result_type operator()(spirit::unused_type, Subject const& subject, boost::spirit::unused_type) const
  {
    return result_type(subject);
  }
};

template <typename Subject, typename Modifiers>
struct make_directive<spirit::terminal_ex<giop::tag::endianness
                                          , boost::fusion::vector1<bool> >
                      , Subject, Modifiers>
{
  typedef specific_endianness_parser<Subject> result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, Subject const& subject, spirit::unused_type) const;
  // {
  //   return result_type(subject, fusion::at_c<0>(term.args));
  // }
};

}

namespace generator {

template <typename Subject>
struct endianness_generator : karma::unary_generator<endianness_generator<Subject> >
{
  template <typename Context, typename Iterator>
  struct attribute : spirit::traits::attribute_of<Subject, Context, Iterator>
  {
  };

  typedef typename Subject::properties properties;

  template <typename OutputIterator, typename Context, typename Delimiter, typename C>
  bool generate(OutputIterator& sink, Context& ctx, Delimiter const& d, C& attr) const
  {
    std::cout << "endianness_generator::generate" << std::endl;
    bool endianness = generator_endianness<typename Context::attributes_type>
      ::call(ctx.attributes);
    karma::any_char<octet_encoding, spirit::unused_type> octet_generator;
    return octet_generator.generate(sink, ctx, d, endianness)
      && subject.generate(sink, ctx, d, attr);
  }

  endianness_generator(Subject const& subject)
    : subject(subject) 
  {
  }

  Subject subject;
};

template <typename Subject>
struct specific_endianness_generator : endianness_generator<Subject>
{
  typedef endianness_generator<Subject> endianness_type;

  template <typename OutputIterator, typename Context, typename Delimiter, typename C>
  bool generate(OutputIterator& sink, Context& ctx, Delimiter const& d, C& attr) const
  {
    std::cout << "specific_endianness_generator::generate" << std::endl;
    typedef typename Context::attributes_type attributes_type;
    typedef typename fusion::result_of::as_list
      <typename fusion::result_of::push_back
       <attributes_type, endianness_attribute>::type
       >::type
      endianness_attributes_type;
    typedef spirit::context
      <endianness_attributes_type, typename Context::locals_type> context_type;
    endianness_attribute e(endianness);
    endianness_attributes_type attributes
      = fusion::as_list(fusion::push_back(ctx.attributes, e));
    context_type context(attributes);
    context.locals = ctx.locals;
    bool r = endianness_type::generate(sink, context, d, attr);
    ctx.locals = context.locals;
    return r;
  }

  specific_endianness_generator(Subject const& subject, giop::endian endianness)
    : endianness_type(subject), endianness(endianness)
  {
  }

  giop::endian endianness;
};

template <typename Subject, typename Modifiers>
struct make_directive<giop::tag::endianness, Subject, Modifiers>
{
  typedef endianness_generator<Subject> result_type;

  result_type operator()(spirit::unused_type, Subject const& subject, boost::spirit::unused_type) const
  {
    return result_type(subject);
  }
};

template <typename Subject, typename Modifiers>
struct make_directive<spirit::terminal_ex<giop::tag::endianness, boost::fusion::vector1<giop::endian> >
                      , Subject, Modifiers>
{
  typedef specific_endianness_generator<Subject> result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, Subject const& subject, spirit::unused_type) const
  {
    return result_type(subject, fusion::at_c<0>(term.args));
  }
};

template <typename Subject, typename Modifiers>
struct make_directive<spirit::terminal_ex<giop::tag::endianness, boost::fusion::vector1<giop::little_endian_type> >
                      , Subject, Modifiers>
  : make_directive<spirit::terminal_ex<giop::tag::endianness, boost::fusion::vector1<giop::endian> >
                   , Subject, Modifiers>
{};

template <typename Subject, typename Modifiers>
struct make_directive<spirit::terminal_ex<giop::tag::endianness, boost::fusion::vector1<giop::big_endian_type> >
                      , Subject, Modifiers>
  : make_directive<spirit::terminal_ex<giop::tag::endianness, boost::fusion::vector1<giop::endian> >
                   , Subject, Modifiers>
{};

template <typename Subject, typename Modifiers>
struct make_directive<spirit::terminal_ex<giop::tag::endianness, boost::fusion::vector1<giop::native_endian_type> >
                      , Subject, Modifiers>
  : make_directive<spirit::terminal_ex<giop::tag::endianness, boost::fusion::vector1<giop::endian> >
                   , Subject, Modifiers>
{};

}

} }

namespace boost { namespace spirit {

namespace traits {

template <typename Subject>
struct has_semantic_action< ::morbid::iiop::generator::endianness_generator<Subject> >
  : unary_has_semantic_action<Subject> {};

template <typename Subject, typename Attribute, typename Context
          , typename Iterator>
struct handles_container< ::morbid::iiop::generator::endianness_generator<Subject>, Attribute
                          , Context, Iterator>
: unary_handles_container<Subject, Attribute, Context, Iterator> {};

}

template <typename Enable>
struct use_directive< ::morbid::iiop::generator_domain, morbid::giop::tag::endianness, Enable> : mpl::true_ {};

template <typename Enable>
struct use_directive< ::morbid::iiop::generator_domain
                      , terminal_ex<morbid::giop::tag::endianness, boost::fusion::vector1< ::morbid::giop::little_endian_type> >
                      , Enable> : mpl::true_ {};

template <typename Enable>
struct use_directive< ::morbid::iiop::generator_domain
                      , terminal_ex<morbid::giop::tag::endianness, boost::fusion::vector1< ::morbid::giop::endian> >
                      , Enable> : mpl::true_ {};

template <typename Enable>
struct use_directive< ::morbid::iiop::generator_domain
                      , terminal_ex<morbid::giop::tag::endianness, boost::fusion::vector1< ::morbid::giop::big_endian_type> >
                      , Enable> : mpl::true_ {};

template <typename Enable>
struct use_directive< ::morbid::iiop::generator_domain
                      , terminal_ex<morbid::giop::tag::endianness, boost::fusion::vector1< ::morbid::giop::native_endian_type> >
                      , Enable> : mpl::true_ {};

template <>
struct use_lazy_directive< ::morbid::iiop::generator_domain, morbid::giop::tag::endianness, 1>
  : mpl::true_ {};

// Parser
template <typename Enable>
struct use_directive< ::morbid::iiop::parser_domain, morbid::giop::tag::endianness, Enable> : mpl::true_ {};

// Parser
template <typename Enable>
struct use_terminal< ::morbid::iiop::parser_domain, ::morbid::iiop::tag::save_endian, Enable> : mpl::true_ {};

} }

#endif
