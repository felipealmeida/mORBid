/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_PARSER_OP_DECL_HPP
#define MORBID_IDL_PARSER_OP_DECL_HPP

#include <morbid/idl_parser/type_spec.hpp>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/variant.hpp>
#include <boost/wave/util/file_position.hpp>

#include <string>
#include <vector>
#include <ostream>

namespace morbid { namespace idl_parser {

namespace direction {

struct in {};
inline bool operator<(in const&, in const&) { return false; }
inline bool operator==(in const&, in const&) { return true; }
inline std::ostream& operator<<(std::ostream& os, in) { return os << "in"; }
struct out {};
inline bool operator<(out const&, out const&) { return false; }
inline bool operator==(out const&, out const&) { return true; }
inline std::ostream& operator<<(std::ostream& os, out) { return os << "out"; }
struct inout {};
inline bool operator<(inout const&, inout const&) { return false; }
inline bool operator==(inout const&, inout const&) { return true; }
inline std::ostream& operator<<(std::ostream& os, inout) { return os << "inout"; }

}

typedef boost::variant<morbid::idl_parser::direction::in
                       , morbid::idl_parser::direction::out
                       , morbid::idl_parser::direction::inout> direction_variant;

struct param_decl
{
  boost::variant<direction::in, direction::out, direction::inout> direction;
  type_spec type;
  boost::wave::util::file_position_type file_position;
};

inline bool operator<(param_decl const& lhs, param_decl const& rhs)
{
  return lhs.direction < rhs.direction 
         || (lhs.direction == rhs.direction
             && lhs.type < rhs.type);
}

inline bool operator==(param_decl const& lhs, param_decl const& rhs)
{
  return lhs.direction == rhs.direction && lhs.type == rhs.type;
}

inline bool operator!=(param_decl const& lhs, param_decl const& rhs)
{
  return !(lhs == rhs);
}

struct op_decl
{
  type_spec type;
  wave_string name;
  std::vector<param_decl> params;
  boost::wave::util::file_position_type file_position;
  bool user_defined;
};

inline bool operator<(op_decl const& lhs, op_decl const& rhs)
{
  return lhs.type < rhs.type
      || (lhs.type == rhs.type
          &&
          (lhs.name < rhs.name
           ||
           (lhs.name == rhs.name
            &&
            (lhs.params < rhs.params
             ||
             (lhs.params == rhs.params
              &&
              (lhs.user_defined < rhs.user_defined))))));
}

inline bool operator==(op_decl const& lhs, op_decl const& rhs)
{
  return lhs.type == rhs.type && lhs.name == rhs.name
    && lhs.params == rhs.params && lhs.user_defined == rhs.user_defined;
}

inline bool operator!=(op_decl const& lhs, op_decl const& rhs)
{
  return !(lhs == rhs);
}

inline std::ostream& operator<<(std::ostream& os, param_decl op)
{
  return os << "[param_decl direction: " << op.direction << " type: " << op.type << "]";
}

inline std::ostream& operator<<(std::ostream& os, op_decl op)
{
  return os << "[op_decl type: " << op.type << " name: " << op.name << " user_defined: " << op.user_defined
            << " params: " << boost::make_iterator_range(op.params.begin(), op.params.end()) << "]";
}

} }


BOOST_FUSION_ADAPT_STRUCT(::morbid::idl_parser::param_decl
                          , (::morbid::idl_parser::direction_variant, direction)
                          (::morbid::idl_parser::type_spec, type)
                          (boost::wave::util::file_position_type, file_position));

BOOST_FUSION_ADAPT_STRUCT(::morbid::idl_parser::op_decl
                          , (::morbid::idl_parser::type_spec, type)
                          (::morbid::idl_parser::wave_string, name)
                          (std::vector< ::morbid::idl_parser::param_decl>, params)
                          (boost::wave::util::file_position_type, file_position)
                          (bool, user_defined));

BOOST_FUSION_ADAPT_STRUCT(::morbid::idl_parser::direction::in, BOOST_PP_EMPTY());
BOOST_FUSION_ADAPT_STRUCT(::morbid::idl_parser::direction::out, BOOST_PP_EMPTY());
BOOST_FUSION_ADAPT_STRUCT(::morbid::idl_parser::direction::inout, BOOST_PP_EMPTY());

#endif
