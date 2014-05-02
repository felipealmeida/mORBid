/* Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/idl_compiler/generate_functions.hpp>
#include <morbid/idl_compiler/lookup.hpp>
#include <morbid/idl_compiler/module_map.hpp>
#include <morbid/idl_compiler/module.hpp>
#include <morbid/idl_compiler/generator/struct_generator.hpp>
#include <morbid/idl_compiler/parse_types.hpp>

namespace morbid { namespace idlc {

void generate_struct(
  morbid::idlc::output_iterator& output_iterator,
  morbid::idl_parser::struct_def const& struct_,
  std::vector<morbid::idlc::vertex_descriptor>& current_module,
  morbid::idlc::modules_tree_type& modules_tree)
{
  morbid::idlc::struct_ s(struct_);
  
  for(std::vector<morbid::idl_parser::member>::const_iterator
      first = s.definition.members.begin()
      , last = s.definition.members.end()
      ;first != last; ++first)
  {
    if(s.lookups.find(first->type) == s.lookups.end())
    {
      try
      {
        s.lookups.insert(std::make_pair
                          (first->type
                          , morbid::idlc::lookup_type_spec
                          (first->type, current_module, modules_tree)));
      }
      catch(morbid::idlc::lookup_error const& e)
      {
        throw morbid::idlc::compilation_error("Not found type of struct member"
                                                , s.definition.file_position);
      }
    }
  }
  
  morbid::idlc::module_map map = get(module_property_t(), modules_tree);
  morbid::idlc::module& module = *boost::get(map, current_module.back());
  module.structs.push_back(s);
  
  morbid::idlc::generator::struct_generator
  <idlc::output_iterator, morbid::idlc::parser_iterator_type>
    struct_generator;
  
  namespace karma = boost::spirit::karma;
  bool r = karma::generate(output_iterator, struct_generator
                            (boost::phoenix::val(module.structs.back()))
                            , (module.structs.back().definition));
  if(!r) throw std::runtime_error("Failed generating header_concept_generator");
}

} }

