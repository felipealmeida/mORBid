/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_PARSER_INTERFACE_DEF_HPP
#define MORBID_IDL_PARSER_INTERFACE_DEF_HPP

#include <morbid/idl_parser/op_decl.hpp>
#include <morbid/idl_parser/attribute.hpp>

#include <boost/range/iterator_range.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/wave/util/file_position.hpp>

#include <ostream>

namespace morbid { namespace idl_parser {

struct interface_def
{
  wave_string name;
  bool fully_defined;
  std::vector<op_decl> op_decls;
  std::vector<attribute> attributes;
  std::vector<wave_string> repoids;
  boost::wave::util::file_position_type file_position;
};

inline std::ostream& operator<<(std::ostream& os, interface_def d)
{
  return os << "[interface_def name: " << d.name << "\n"
            << " defined at " << d.file_position
            << "ops: " << boost::make_iterator_range(d.op_decls.begin()
                                                     , d.op_decls.end())
            << "\nrepoids: " << boost::make_iterator_range(d.repoids.begin(), d.repoids.end())
            << "]";
}

} }

BOOST_FUSION_ADAPT_STRUCT(::morbid::idl_parser::interface_def
                          , 
                          ( ::morbid::idl_parser::wave_string, name)
                          (std::vector< ::morbid::idl_parser::op_decl>, op_decls)
                          (std::vector< ::morbid::idl_parser::attribute>, attributes)
                          (std::vector< ::morbid::idl_parser::wave_string>, repoids)
                          ( ::boost::wave::util::file_position_type, file_position)
                          (bool, fully_defined)
                          );

#endif
