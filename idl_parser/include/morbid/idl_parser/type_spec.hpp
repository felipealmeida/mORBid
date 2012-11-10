/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_PARSER_TYPE_SPEC_HPP
#define TECORB_IDL_PARSER_TYPE_SPEC_HPP

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant.hpp>
#include <boost/optional.hpp>
#include <boost/utility.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/distance.hpp>

#include <string>
#include <vector>

namespace morbid { namespace idl_parser {

template <typename Iterator>
struct type_spec;

namespace types {
  
  struct floating_point
  {
    enum floating_enum
    {
      float_, double_, long_double_
    };
    floating_enum type;

    floating_point() : type(float_) {}
    floating_point(floating_enum e)
      : type(e) {}
  };
  inline bool operator<(floating_point lhs, floating_point rhs)
  {
    return lhs.type < rhs.type;
  }
  inline std::ostream& operator<<(std::ostream& os, floating_point o)
  {
    switch(o.type)
    {
    case floating_point::float_:
      return os << "float";
    case floating_point::double_:
      return os << "double";
    case floating_point::long_double_:
      return os << "long double";
    };
    std::abort();
  }
  struct integer
  {
    enum integer_enum
    {
      signed_short_int /* "short" */
      , signed_long_int /* "long" */
      , signed_longlong_int /* "long" "long" */
      , unsigned_short_int /* "unsigned" "short" */
      , unsigned_long_int /* "unsigned" "long" */
      , unsigned_longlong_int /* "unsigned" "long" "long" */
    };
    integer_enum type;

    integer() : type(signed_short_int) {}
    integer(integer_enum e)
      : type(e) {}
  };
  inline bool operator<(integer lhs, integer rhs)
  {
    return lhs.type < rhs.type;
  }
  inline std::ostream& operator<<(std::ostream& os, integer o)
  {
    switch(o.type)
    {
    case integer::signed_short_int:
      return os << "short";
    case integer::signed_long_int:
      return os << "long";
    case integer::signed_longlong_int:
      return os << "long long";
    case integer::unsigned_short_int:
      return os << "unsigned short";
    case integer::unsigned_long_int:
      return os << "unsigned long";
    case integer::unsigned_longlong_int:
      return os << "unsigned long long";
    };
    std::abort();
  }
  struct char_ {};
  inline bool operator<(char_ lhs, char_ rhs) { return false; }
  inline std::ostream& operator<<(std::ostream& os, char_ o) { return os << "char"; }
  struct wchar_ {};
  inline bool operator<(wchar_ lhs, wchar_ rhs) { return false; }
  inline std::ostream& operator<<(std::ostream& os, wchar_ o) { return os << "wchar"; }
  struct boolean {};
  inline bool operator<(boolean lhs, boolean rhs) { return false; }
  inline std::ostream& operator<<(std::ostream& os, boolean o) { return os << "boolean"; }
  struct octet {};
  inline bool operator<(octet lhs, octet rhs) { return false; }
  inline std::ostream& operator<<(std::ostream& os, octet o) { return os << "octet"; }
  struct any {};
  inline bool operator<(any lhs, any rhs) { return false; }
  inline std::ostream& operator<<(std::ostream& os, any o) { return os << "any"; }
  struct object {};
  inline bool operator<(object lhs, object rhs) { return false; }
  inline std::ostream& operator<<(std::ostream& os, object o) { return os << "Object"; }
  struct value_base {};
  inline bool operator<(value_base lhs, value_base rhs) { return false; }
  inline std::ostream& operator<<(std::ostream& os, value_base o) { return os << "ValueBase"; }
  struct void_ {}; // for op_param_decl
  inline bool operator<(void_ lhs, void_ rhs) { return false; }
  inline std::ostream& operator<<(std::ostream& os, void_ o) { return os << "void"; }
  struct scoped_name
  {
    bool globally_qualified;
    std::vector<std::string> identifiers; // identifier | "::" identifier | scoped_name "::" identifier
  };
  inline bool operator<(scoped_name const& lhs, scoped_name const& rhs)
  {
    return lhs.globally_qualified < rhs.globally_qualified
        || (lhs.globally_qualified == rhs.globally_qualified
            && lhs.identifiers < rhs.identifiers);
  }
  inline std::ostream& operator<<(std::ostream& os, scoped_name o)
  {
    os << "[scoped_name ";
    if(o.globally_qualified)
      os << "::";
    for(std::vector<std::string>::const_iterator
          first = o.identifiers.begin()
          , last = o.identifiers.end()
          ;first != last; ++first)
    {
      os << *first;
      if(boost::next(first) != last)
        os << "::";
    }
    return os << ']';
  }
  template <typename Iterator>
  struct sequence
  {
    type_spec<Iterator> type;

    sequence() {}
    sequence(type_spec<Iterator> o)
      : type(o) {}
  };
  template <typename Iterator>
  bool operator<(sequence<Iterator> const& lhs, sequence<Iterator> const& rhs)
  {
    return lhs.type < rhs.type;
  }
  template <typename Iterator>
  std::ostream& operator<<(std::ostream& os, sequence<Iterator> o)
  {
    return os << "[sequence of " << o.type << "]";
  }

}

template <typename Iterator>
struct type_spec
{
  typedef boost::variant<types::floating_point, types::integer, types::char_
                         , types::wchar_, types::boolean, types::octet
                         , types::any, types::object, types::value_base
                         , types::void_
                         , types::scoped_name
                         , boost::recursive_wrapper<types::sequence<Iterator> > >
    variant_type;
  variant_type type;
  type_spec(variant_type v)
    : type(v) {}
  type_spec() {}
  type_spec(types::floating_point::floating_enum e)
    : type(types::floating_point(e)) {}
  type_spec(types::integer::integer_enum e)
    : type(types::integer(e)) {}
  type_spec(types::char_ o)
    : type(o) {}
  type_spec(types::wchar_ o)
    : type(o) {}
  type_spec(types::boolean o)
    : type(o) {}
  type_spec(types::octet o)
    : type(o) {}
  type_spec(types::any o)
    : type(o) {}
  type_spec(types::object o)
    : type(o) {}
  type_spec(types::value_base o)
    : type(o) {}
  type_spec(types::void_ o)
    : type(o) {}
  type_spec(types::scoped_name o)
    : type(o)
  {}
  type_spec(types::sequence<Iterator> o)
    : type(o)
  {}
};

template <typename Iterator>
bool operator<(type_spec<Iterator> const& lhs, type_spec<Iterator> const& rhs)
{
  return lhs.type < rhs.type;
}

template <typename Iterator>
std::ostream& operator<<(std::ostream& os, type_spec<Iterator> d)
{
  return os << "[type_spec type: " << d.type << "]";
}

} }

BOOST_FUSION_ADAPT_TPL_STRUCT((Iterator)
                              , (::morbid::idl_parser::type_spec)(Iterator)
                              , (typename ::morbid::idl_parser::type_spec<Iterator>::variant_type, type)
                              );

BOOST_FUSION_ADAPT_STRUCT(::morbid::idl_parser::types::scoped_name
                          , (bool, globally_qualified)
                          (std::vector<std::string>, identifiers));

BOOST_FUSION_ADAPT_TPL_STRUCT((Iterator)
                              , (::morbid::idl_parser::types::sequence)(Iterator)
                              , (::morbid::idl_parser::type_spec<Iterator>, type));

BOOST_FUSION_ADAPT_STRUCT(::morbid::idl_parser::types::floating_point
                          , (::morbid::idl_parser::types::floating_point::floating_enum, type));

BOOST_FUSION_ADAPT_STRUCT(::morbid::idl_parser::types::integer
                          , (::morbid::idl_parser::types::integer::integer_enum, type));

BOOST_FUSION_ADAPT_STRUCT(::morbid::idl_parser::types::char_
                          , BOOST_PP_EMPTY());

BOOST_FUSION_ADAPT_STRUCT(::morbid::idl_parser::types::wchar_
                          , BOOST_PP_EMPTY());

BOOST_FUSION_ADAPT_STRUCT(::morbid::idl_parser::types::boolean
                          , BOOST_PP_EMPTY());

BOOST_FUSION_ADAPT_STRUCT(::morbid::idl_parser::types::octet
                          , BOOST_PP_EMPTY());

BOOST_FUSION_ADAPT_STRUCT(::morbid::idl_parser::types::any
                          , BOOST_PP_EMPTY());

BOOST_FUSION_ADAPT_STRUCT(::morbid::idl_parser::types::object
                          , BOOST_PP_EMPTY());

BOOST_FUSION_ADAPT_STRUCT(::morbid::idl_parser::types::value_base
                          , BOOST_PP_EMPTY());

BOOST_FUSION_ADAPT_STRUCT(::morbid::idl_parser::types::void_
                          , BOOST_PP_EMPTY());

#endif
