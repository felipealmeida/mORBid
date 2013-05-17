/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_STRUCT_GENERATOR_HPP
#define MORBID_IDL_COMPILER_STRUCT_GENERATOR_HPP

#include <morbid/idl_compiler/module.hpp>
#include <morbid/idl_compiler/generator/type_spec.hpp>
#include <morbid/idl_compiler/generator/scoped_name.hpp>
#include <morbid/idl_compiler/generator/struct_generator_generator.hpp>

#include <boost/spirit/home/karma.hpp>

#include <string>
#include <ostream>
#include <vector>

namespace morbid { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator, typename Iterator>
struct struct_generator : karma::grammar
  <OutputIterator, idl_parser::struct_def(struct_)>
{
  struct_generator()
    : struct_generator::base_type(start)
  {
    using phoenix::at_c;
    using karma::eol;
    using karma::_1; using karma::_r1; using karma::_r2;
    using karma::_val; using karma::_a;

    wave_string %= karma::string;
    mpl_member_type =
      type_spec
      (
       at_c<1>(_r1)[at_c<0>(_val)]
      )[_1 = at_c<0>(_val)]
      ;    
    mpl_sequence_type =
      karma::string[_1 = "::boost::mpl::vector< "]
      << (mpl_member_type(_r1) % ',')[_1 = at_c<1>(_val)]
      << " >"
      ;
    fusion_at =
      indent << mpl_member_type(_r1)[_1 = _val] << "& _morbid_fusion_at( ::boost::mpl::size_t<"
      << karma::lit(_r2) << "u>) { return " << wave_string[_1 = at_c<1>(_val)] << "; }" << eol
      << indent << mpl_member_type(_r1)[_1 = _val] << " const& _morbid_fusion_at( ::boost::mpl::size_t<"
      << karma::lit(_r2) << "u>) const { return " << wave_string[_1 = at_c<1>(_val)] << "; }" << eol
      ;
    morbid_fusion_model = // karma::eps
         indent << "typedef boost::mpl::size_t<" << karma::lit(phoenix::size(at_c<1>(_val))) << "> _morbid_fusion_size;" << eol
      << indent << "typedef " << mpl_sequence_type(_r1)[_1 = _val] << " _morbid_fusion_types;" << eol
      //      << indent << "typedef ::boost::fusion::fusion_sequence_tag tag;" << eol
      << (*fusion_at(_r1, _a++))[_1 = at_c<1>(_val)]
      ;
    start =
      eol
      << "template <typename = void> struct "
      << wave_string[_1 = at_c<0>(_val)]
      << "_struct"
      << '{' << eol
      << indent << "// members" << eol
      << (*(indent << member(_r1) << ';' << eol))[_1 = at_c<1>(_val)]
      << indent << "// members end" << eol << eol
      << indent << "typedef ::morbid::struct_tag _morbid_type_kind;" << eol
      << morbid_fusion_model(_r1)[_1 = _val]
      << struct_generator_generator(_r1)[_1 = _val]
      << "};" << eol << eol
      << "typedef " << wave_string[_1 = at_c<0>(_val)] << "_struct<> "
      << wave_string[_1 = at_c<0>(_val)] << ";" << eol
      << "inline ::std::ostream& operator<<( ::std::ostream& os, " << wave_string[_1 = at_c<0>(_val)] << " const& obj)" << eol
      << '{' << eol
      << indent << "return os;" << eol
      << '}' << eol
      ;
    indent = karma::space << karma::space;
    member =
      type_spec
      (
       at_c<1>(_r1)[at_c<0>(_val)]
       )[_1 = at_c<0>(_val)]
      << karma::space << wave_string[_1 = at_c<1>(_val)]
      ;
  }

  generator::type_spec<OutputIterator, Iterator> type_spec;
  generator::struct_generator_generator<OutputIterator, Iterator>
    struct_generator_generator;
  karma::rule<OutputIterator, idl_parser::member(struct_)> mpl_member_type;
  karma::rule<OutputIterator, idl_parser::member(struct_, unsigned int)> fusion_at;
  karma::rule<OutputIterator, idl_parser::struct_def(struct_)> mpl_sequence_type;
  karma::rule<OutputIterator> indent;
  karma::rule<OutputIterator, idl_parser::struct_def(struct_), karma::locals<unsigned int> > morbid_fusion_model;
  karma::rule<OutputIterator, idl_parser::member(struct_)> member;
  karma::rule<OutputIterator, idl_parser::struct_def(struct_)> start;
  karma::rule<OutputIterator, idl_parser::wave_string()> wave_string;
};

} } }

#endif
