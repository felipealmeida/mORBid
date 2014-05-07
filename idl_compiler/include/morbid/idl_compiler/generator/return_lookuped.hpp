/* Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_RETURN_LOOKUPED_HPP
#define MORBID_IDL_COMPILER_RETURN_LOOKUPED_HPP

#include <morbid/idl_compiler/generator/return.hpp>
#include <morbid/idl_parser/type_spec.hpp>
#include <boost/spirit/home/karma.hpp>

namespace morbid { namespace idlc { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
struct return_lookuped : karma::grammar<OutputIterator, idl_parser::type_spec(lookuped_type, std::string)>
{
  return_lookuped();

  idlc::generator::return_<OutputIterator> return_traits;
  karma::rule<OutputIterator, idl_parser::type_spec(lookuped_type, std::string)> start;
};

} } }

#endif
