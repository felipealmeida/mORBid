/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_PARSER_TOKEN_HPP
#define MORBID_IDL_PARSER_TOKEN_HPP

#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/support.hpp>
#include <boost/wave/token_ids.hpp>
#include <boost/wave/util/file_position.hpp>

namespace morbid { namespace idl_parser {

namespace spirit = boost::spirit;
namespace qi = spirit::qi;

BOOST_SPIRIT_TERMINAL_EX(token_id);
BOOST_SPIRIT_TERMINAL_EX(token_category);
BOOST_SPIRIT_TERMINAL_EX(token_value);
BOOST_SPIRIT_TERMINAL_EX(parse_token_value);
BOOST_SPIRIT_TERMINAL_EX(token_position);

template <typename Id>
struct token_id_parser : qi::primitive_parser<token_id_parser<Id> >
{
  token_id_parser(Id id)
    : id(id) {}

  template <typename Context, typename Iterator>
  struct attribute
  {
    typedef spirit::unused_type type;
  };

  template <typename Iterator, typename Context, typename Skipper, typename Attribute>
  bool parse(Iterator& first, Iterator const& last
             , Context& caller_context, Skipper const& skipper
             , Attribute& attr_param) const
  {
    typedef typename boost::remove_cv<Attribute>::type attribute_type;
    BOOST_MPL_ASSERT((boost::is_same<spirit::unused_type, attribute_type>));
    qi::skip_over(first, last, skipper);

    if(first != last && first->get_id() == id)
      return (++first, true);
    else
      return false;
  }

  Id id;
};

template <typename Category>
struct token_category_parser : qi::primitive_parser<token_category_parser<Category> >
{
  token_category_parser(Category category)
    : category(category) {}

  template <typename Context, typename Iterator>
  struct attribute
  {
    typedef spirit::unused_type type;
  };

  template <typename U>
  static std::size_t get_category(U c)
  {
    return c & (boost::wave::ExtTokenTypeMask + boost::wave::PPTokenFlag);
  }

  template <typename Iterator, typename Context, typename Skipper, typename Attribute>
  bool parse(Iterator& first, Iterator const& last
             , Context& caller_context, Skipper const& skipper
             , Attribute& attr_param) const
  {
    typedef typename boost::remove_cv<Attribute>::type attribute_type;
    BOOST_MPL_ASSERT((boost::is_same<spirit::unused_type, attribute_type>));
    qi::skip_over(first, last, skipper);

    assert(category == get_category(category));
    if(first != last && get_category(first->get_id()) == category)
      return (++first, true);
    else
      return false;
  }

  Category category;
};

template <typename V>
struct specific_token_value_parser : qi::primitive_parser<specific_token_value_parser<V> >
{
  specific_token_value_parser(V value)
    : value(value) {}

  template <typename Context, typename Iterator>
  struct attribute
  {
    typedef spirit::unused_type type;
  };

  template <typename Iterator, typename Context, typename Skipper, typename Attribute>
  bool parse(Iterator& first, Iterator const& last
             , Context& caller_context, Skipper const& skipper
             , Attribute& attr_param) const
  {
    typedef typename boost::remove_cv<Attribute>::type attribute_type;
    BOOST_MPL_ASSERT((boost::is_same<spirit::unused_type, attribute_type>));
    qi::skip_over(first, last, skipper);

    if(first != last && first->get_value() == value)
      return (++first, true);
    else
      return false;
  }

  V value;
};

struct token_value_parser : qi::primitive_parser<token_value_parser>
{
  template <typename Context, typename Iterator>
  struct attribute
  {
    typedef typename Iterator::value_type::string_type type;
  };

  template <typename Iterator, typename Context, typename Skipper, typename Attribute>
  bool parse(Iterator& first, Iterator const& last
             , Context& caller_context, Skipper const& skipper
             , Attribute& attr_param) const
  {
    typedef typename boost::remove_cv<Attribute>::type attribute_type;
    BOOST_MPL_ASSERT_NOT((boost::is_same<spirit::unused_type, attribute_type>));
    qi::skip_over(first, last, skipper);

    if(first != last)
    {
      attr_param = first->get_value();
      ++first;
      return true;
    }
    else
      return false;
  }
};

template <typename Subject>
struct token_value_directive : qi::unary_parser<token_value_directive<Subject> >
{
  template <typename Context, typename Iterator>
  struct attribute
  {
    typedef typename std::iterator_traits<Iterator>::value_type::string_type string_type;
    typedef typename spirit::traits::attribute_of<Subject, Context, typename string_type::iterator>::type type;
  };

  token_value_directive(Subject const& subject)
    : subject(subject) {}

  template <typename Iterator, typename Context, typename Skipper, typename Attribute>
  bool parse(Iterator& first, Iterator last
             , Context& ctx, Skipper const& skipper
             , Attribute& attr) const
  {
    typedef typename boost::remove_cv<Attribute>::type attribute_type;
    BOOST_MPL_ASSERT_NOT((boost::is_same<spirit::unused_type, attribute_type>));
    qi::skip_over(first, last, skipper);

    if(first != last)
    {
      typedef typename std::iterator_traits<Iterator>::value_type::string_type string_type;
      string_type string = first->get_value();
      typename string_type::iterator first_ = string.begin();
      bool r = boost::spirit::qi::parse(first_, string.end(), subject, attr);
      if (r)
        ++first;
      return r;
    }
    else
      return false;
  }  
  
  Subject subject;
};

struct token_position_parser : qi::unary_parser<token_position_parser>
{
  template <typename Context, typename Iterator>
  struct attribute
  {
    typedef boost::wave::util::file_position_type type;
  };

  template <typename Iterator, typename Context, typename Skipper, typename Attribute>
  bool parse(Iterator& first, Iterator last
             , Context& ctx, Skipper const& skipper
             , Attribute& attr) const
  {
    typedef typename boost::remove_cv<Attribute>::type attribute_type;
    BOOST_MPL_ASSERT_NOT((boost::is_same<spirit::unused_type, attribute_type>));
    qi::skip_over(first, last, skipper);

    if(first != last)
    {
      attr = first->get_position();
      ++first;
      return true;
    }
    else
      return false;
  }  
};

} }

namespace boost { namespace spirit {

namespace qi {

template <typename U, typename Modifiers>
struct make_primitive<spirit::terminal_ex< ::morbid::idl_parser::tag::token_id, fusion::vector1<U> >
                      , Modifiers>
{
  typedef ::morbid::idl_parser::token_id_parser<U> result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, boost::spirit::unused_type) const
  {
    return result_type(fusion::at_c<0>(term.args));
  }
};

template <typename U, typename Modifiers>
struct make_primitive<spirit::terminal_ex< ::morbid::idl_parser::tag::token_category, fusion::vector1<U> >
                      , Modifiers>
{
  typedef ::morbid::idl_parser::token_category_parser<U> result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, boost::spirit::unused_type) const
  {
    return result_type(fusion::at_c<0>(term.args));
  }
};

template <typename U, typename Modifiers>
struct make_primitive<spirit::terminal_ex< ::morbid::idl_parser::tag::token_value, fusion::vector1<U> >
                      , Modifiers>
{
  typedef ::morbid::idl_parser::specific_token_value_parser<U> result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, boost::spirit::unused_type) const
  {
    return result_type(fusion::at_c<0>(term.args));
  }
};

template <typename Modifiers>
struct make_primitive< ::morbid::idl_parser::tag::token_value, Modifiers>
{
  typedef ::morbid::idl_parser::token_value_parser result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

template <typename Modifiers>
struct make_primitive< ::morbid::idl_parser::tag::token_position, Modifiers>
{
  typedef ::morbid::idl_parser::token_position_parser result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

template <typename Subject, typename Modifiers>
struct make_directive< ::morbid::idl_parser::tag::parse_token_value, Subject, Modifiers>
{
  typedef ::morbid::idl_parser::token_value_directive<Subject> result_type;

  result_type operator()(unused_type, Subject const& subject, unused_type) const
  {
    return result_type(subject);
  }
};

}

namespace traits {

template <typename Attribute, typename Context, typename Iterator>
struct handles_container< ::morbid::idl_parser::token_value_parser, Attribute
                          , Context, Iterator>
 : mpl::true_ {};

template <typename Subject>
struct has_semantic_action< ::morbid::idl_parser::token_value_directive<Subject> >
  : unary_has_semantic_action<Subject> {};

template <typename Subject, typename Attribute, typename Context, typename Iterator>
struct handles_container< ::morbid::idl_parser::token_value_directive<Subject>, Attribute
                          , Context, Iterator>
  : unary_handles_container< Subject, Attribute, Context, Iterator> {};

}

template <typename U>
struct use_terminal
 < qi::domain
   , terminal_ex<morbid::idl_parser::tag::token_id, fusion::vector1<U> >
 > : mpl::true_ {};

template <typename U>
struct use_terminal
 < qi::domain
   , terminal_ex<morbid::idl_parser::tag::token_category, fusion::vector1<U> >
 > : mpl::true_ {};

template <typename U>
struct use_terminal
 < qi::domain
   , terminal_ex<morbid::idl_parser::tag::token_value, fusion::vector1<U> >
 > : mpl::true_ {};

template <>
struct use_terminal
 < qi::domain, morbid::idl_parser::tag::token_value > : mpl::true_ {};

template <>
struct use_terminal
 < qi::domain, morbid::idl_parser::tag::token_position > : mpl::true_ {};

template <>
struct use_directive< qi::domain, ::morbid::idl_parser::tag::parse_token_value>
 : mpl::true_ {};

} }

#endif
