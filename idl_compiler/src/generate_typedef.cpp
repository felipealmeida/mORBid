/* Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/idl_compiler/generate_functions.hpp>

#include <morbid/idl_compiler/errors.hpp>
#include <morbid/idl_compiler/lookup.hpp>
#include <morbid/idl_compiler/module_map.hpp>
#include <morbid/idl_compiler/generator/typedef_generator.hpp>
#include <morbid/idl_compiler/parse_types.hpp>
#include <boost/spirit/home/karma.hpp>

namespace morbid { namespace idlc {

void generate_typedef(
  morbid::idlc::output_iterator& output_iterator,
  morbid::idl_parser::typedef_def const& typedef_,
  std::vector<morbid::idlc::vertex_descriptor>& current_module, // const ??
  morbid::idlc::modules_tree_type& modules_tree // const ??
){
  morbid::idlc::typedef_ t(typedef_);
  try
  {
    t.lookup = morbid::idlc::lookup_type_spec(typedef_.alias, current_module, modules_tree);
  }
  catch(morbid::idlc::lookup_error const& e)
  {
    throw morbid::idlc::compilation_error("Not found type of typedef", typedef_.file_position);
  }

  morbid::idlc::module_map map = get(module_property_t(), modules_tree);
  morbid::idlc::module& module = *boost::get(map, current_module.back());
  std::cout << "Adding typedef " << typedef_.name << " at module " << module.name << std::endl;
  module.typedefs.push_back(t);

  morbid::idlc::generator::typedef_generator
    <morbid::idlc::output_iterator, morbid::idlc::parser_iterator_type>
    typedef_generator;

  bool r = boost::spirit::karma::generate(output_iterator, typedef_generator
                            (boost::phoenix::val(module.typedefs.back().lookup))
                            , module.typedefs.back().definition);
  assert(r);
}

} }
