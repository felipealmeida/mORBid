/* Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/idl_compiler/generate_functions.hpp>

#include <boost/spirit/home/karma.hpp>

namespace morbid { namespace idlc {

void generate_module_close(
  morbid::idlc::output_iterator& output_iterator,
  std::vector<morbid::idlc::vertex_descriptor>& current_module
) {
  namespace karma = boost::spirit::karma;
  if(current_module.size() > 1)
  {
    karma::generate(output_iterator, karma::eol << karma::lit("}") << karma::eol);
    current_module.pop_back();
  }
  else
    throw std::runtime_error("Error, closing non existant module");
}

} }
