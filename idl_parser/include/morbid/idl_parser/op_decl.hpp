/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_PARSER_OP_DECL_HPP
#define TECORB_IDL_PARSER_OP_DECL_HPP

#include <morbid/idl_parser/type_spec.hpp>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/variant.hpp>

#include <string>
#include <vector>
#include <ostream>

namespace morbid { namespace idl_parser {

namespace direction {

struct in {};
inline std::ostream& operator<<(std::ostream& os, in) { return os << "in"; }
struct out {};
inline std::ostream& operator<<(std::ostream& os, out) { return os << "out"; }
struct inout {};
inline std::ostream& operator<<(std::ostream& os, inout) { return os << "inout"; }

}

typedef boost::variant<morbid::idl_parser::direction::in
                       , morbid::idl_parser::direction::out
                       , morbid::idl_parser::direction::inout> direction_variant;

template <typename Iterator>
struct param_decl
{
  boost::variant<direction::in, direction::out, direction::inout> direction;
  type_spec<Iterator> type;
};

template <typename Iterator>
bool operator<(param_decl<Iterator> const& lhs, param_decl<Iterator> const& rhs)
{
  return lhs.direction < rhs.direction 
         || (lhs.direction == rhs.direction
             && lhs.type < rhs.type);
}
template <typename Iterator>
bool operator==(param_decl<Iterator> const& lhs, param_decl<Iterator> const& rhs)
{
  return lhs.direction == rhs.direction && lhs.type == rhs.type;
}
template <typename Iterator>
bool operator!=(param_decl<Iterator> const& lhs, param_decl<Iterator> const& rhs)
{
  return !(lhs == rhs);
}

template <typename Iterator>
struct op_decl
{
  typedef typename Iterator::base_iterator_type base_iterator;

  type_spec<Iterator> type;
  std::string name;
  std::vector<param_decl<Iterator> > params;
  bool user_defined;
};

template <typename Iterator>
bool operator<(op_decl<Iterator> const& lhs, op_decl<Iterator> const& rhs)
{
  return lhs.type < rhs.type
      || (lhs.type == rhs.type
          &&
          (lhs.name < rhs.name
           ||
           (lhs.name == rhs.name
            &&
            (lhs.params < rhs.name
             ||
             (lhs.params == rhs.name
              &&
              (lhs.user_defined < rhs.user_defined))))));
}
template <typename Iterator>
bool operator==(op_decl<Iterator> const& lhs, op_decl<Iterator> const& rhs)
{
  return lhs.type == rhs.type && lhs.name == rhs.name
    && lhs.params == rhs.params && lhs.user_defined == rhs.user_defined;
}
template <typename Iterator>
bool operator!=(op_decl<Iterator> const& lhs, op_decl<Iterator> const& rhs)
{
  return !(lhs == rhs);
}

template <typename Iterator>
std::ostream& operator<<(std::ostream& os, param_decl<Iterator> op)
{
  return os << "[param_decl direction: " << op.direction << " type: " << op.type << "]";
}

template <typename Iterator>
std::ostream& operator<<(std::ostream& os, op_decl<Iterator> op)
{
  return os << "[op_decl type: " << op.type << " name: " << op.name << " user_defined: " << op.user_defined
            << " params: " << boost::make_iterator_range(op.params.begin(), op.params.end()) << "]";
}

} }


BOOST_FUSION_ADAPT_TPL_STRUCT((Iterator)
                              , (::morbid::idl_parser::param_decl)(Iterator)
                              , (::morbid::idl_parser::direction_variant, direction)
                              (::morbid::idl_parser::type_spec<Iterator>, type)
                              );

BOOST_FUSION_ADAPT_TPL_STRUCT((Iterator)
                              , (::morbid::idl_parser::op_decl) (Iterator)
                              , (::morbid::idl_parser::type_spec<Iterator>, type)(std::string, name)
                              (std::vector< ::morbid::idl_parser::param_decl<Iterator> >, params)
                              (bool, user_defined)
                              );

BOOST_FUSION_ADAPT_STRUCT(::morbid::idl_parser::direction::in, BOOST_PP_EMPTY());
BOOST_FUSION_ADAPT_STRUCT(::morbid::idl_parser::direction::out, BOOST_PP_EMPTY());
BOOST_FUSION_ADAPT_STRUCT(::morbid::idl_parser::direction::inout, BOOST_PP_EMPTY());

#endif
