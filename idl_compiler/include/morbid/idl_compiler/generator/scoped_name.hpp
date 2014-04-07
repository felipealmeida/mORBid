/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_SCOPED_NAME_HPP
#define MORBID_IDL_COMPILER_SCOPED_NAME_HPP

#include <morbid/idl_compiler/common_types.hpp>
#include <morbid/idl_compiler/lookuped_type.hpp>
#include <morbid/idl_parser/type_spec.hpp>

#include <boost/spirit/home/karma/nonterminal.hpp>
#include <boost/fusion/include/adapt_adt.hpp>

namespace morbid { namespace idlc { namespace generator {

namespace karma = boost::spirit::karma;

struct lookup_inside_type {};

struct lookuped_type_wrapper
{
  lookuped_type_wrapper() : inside(false) {}
  lookuped_type_wrapper(lookuped_type l)
    : l(l), inside(false) {}
  lookuped_type_wrapper(lookuped_type l, lookup_inside_type)
    : l(l), inside(true) {}

  std::vector<idl_parser::wave_string> get_type() const;
  kind_variant kind() const
  {
    return l.kind;
  }

  lookuped_type l;
  bool inside;
};

} } }

BOOST_FUSION_ADAPT_ADT( ::morbid::idlc::generator::lookuped_type_wrapper
                        , (std::vector< ::morbid::idl_parser::wave_string>
                           , std::vector< ::morbid::idl_parser::wave_string>
                           , obj.get_type(), ::std::abort())
                        (::morbid::idlc::kind_variant
                         , ::morbid::idlc::kind_variant
                         , obj.kind(), ::std::abort())
                        );

namespace morbid { namespace idlc { namespace generator {

template <typename OutputIterator>
struct scoped_name : karma::grammar<OutputIterator, idl_parser::types::scoped_name
                                    (lookuped_type_wrapper)>
{
  scoped_name();

  karma::rule<OutputIterator, idl_parser::types::scoped_name
              (lookuped_type_wrapper)> start;
  karma::rule<OutputIterator, idlc::is_interface_kind()>
    is_interface_kind;
  karma::rule<OutputIterator, idlc::is_struct_kind()>
    is_struct_kind;
  karma::rule<OutputIterator, idlc::is_primitive_kind()>
    is_primitive_kind;
  karma::rule<OutputIterator, idlc::is_typedef_kind()>
    is_typedef_kind;
};

} } }

#endif

