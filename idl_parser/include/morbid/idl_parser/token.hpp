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

namespace morbid { namespace idl_parser {

namespace spirit = boost::spirit;
namespace qi = spirit::qi;

BOOST_SPIRIT_TERMINAL_EX(token_id);
BOOST_SPIRIT_TERMINAL_EX(token_category);
BOOST_SPIRIT_TERMINAL_EX(token_value);

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

    std::cout << "Trying to match " << id << " with " << first->get_id() << std::endl;
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
    return c & boost::wave::ExtTokenTypeMask;
  }

  template <typename Iterator, typename Context, typename Skipper, typename Attribute>
  bool parse(Iterator& first, Iterator const& last
             , Context& caller_context, Skipper const& skipper
             , Attribute& attr_param) const
  {
    typedef typename boost::remove_cv<Attribute>::type attribute_type;
    BOOST_MPL_ASSERT((boost::is_same<spirit::unused_type, attribute_type>));
    qi::skip_over(first, last, skipper);

    std::cout << "Trying to match (category) " << category << " with " << get_category(first->get_id()) << std::endl;
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

    std::cout << "Trying to match (value) " << value << " with " << first->get_value() << std::endl;
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

}

namespace traits {

template <typename Attribute, typename Context, typename Iterator>
struct handles_container< ::morbid::idl_parser::token_value_parser, Attribute
                          , Context, Iterator>
 : mpl::true_ {};

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

} }

#endif
