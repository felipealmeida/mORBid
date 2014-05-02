/* Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/idl_compiler/generate_functions.hpp>

#include <morbid/idl_compiler/interface.hpp>
#include <morbid/idl_compiler/module_map.hpp>
#include <morbid/idl_compiler/generator/wave_string.hpp>
#include <morbid/idl_compiler/module.hpp>
#include <morbid/idl_compiler/lookup.hpp>
#include <morbid/idl_compiler/errors.hpp>
#include <morbid/idl_compiler/generator/concept_generator.hpp>
#include <boost/spirit/home/karma.hpp>
#include <boost/phoenix/fusion/at.hpp>
#include <boost/phoenix.hpp>

namespace morbid { namespace idlc {

void generate_interface(
  morbid::idlc::output_iterator& output_iterator,
  morbid::idl_parser::interface_def const& interface,
  std::vector<morbid::idlc::vertex_descriptor>& current_module,
  morbid::idlc::modules_tree_type& modules_tree)
{
  using morbid::idl_parser::wave_string;
  
  morbid::idlc::interface_ i(interface);

  morbid::idlc::module_map map = get(module_property_t(), modules_tree);
  std::vector<wave_string> modules_names;
  for(std::vector<vertex_descriptor>::const_iterator
        first = boost::next(current_module.begin(), 2)
        , last = current_module.end()
        ;first != last; ++first)
  {
    morbid::idlc::module const& mx = *boost::get(map, *first);
    modules_names.push_back(mx.name);
  }

  modules_names.push_back(interface.name);
  i.definition.repoids.push_back("IDL:omg.org/CORBA/Object:1.0");
  assert(!modules_names.empty());
  namespace karma = boost::spirit::karma;
  namespace idlc = morbid::idlc;
  wave_string repoid;
  if(!karma::generate(std::back_insert_iterator<wave_string>(repoid)
                      , "IDL:"
                      << (idlc::wave_string % '/')
                      << ":1.0"
                      , modules_names))
  {
    throw std::runtime_error("Failed constructing RepoID");
  }
  
  i.definition.repoids.push_back(repoid);
  assert(i.definition.repoids.size() == 2);

  for(std::vector<morbid::idl_parser::types::scoped_name>::const_iterator
        first = i.definition.bases.begin()
        , last = i.definition.bases.end()
        ;first != last; ++first)
  {
    if(i.lookups.find(*first) == i.lookups.end())
    {
      try
      {
        i.lookups.insert(std::make_pair
                          (*first
                          , morbid::idlc::lookup_type_spec
                          (*first, current_module, modules_tree)));
      }
      catch(morbid::idlc::lookup_error const& e)
      {
        throw morbid::idlc::compilation_error("Not found type for base of interface"
                                                      , i.definition.file_position);
      }
    }
  }

  for(std::vector<morbid::idl_parser::attribute>::const_iterator
        first = i.definition.attributes.begin()
        , last = i.definition.attributes.end()
        ;first != last; ++first)
  {
    if(i.lookups.find(first->type) == i.lookups.end())
    {
      try
      {
        i.lookups.insert(std::make_pair
                          (first->type
                          , morbid::idlc::lookup_type_spec
                          (first->type, current_module, modules_tree)));
      }
      catch(morbid::idlc::lookup_error const& e)
      {
        throw morbid::idlc::compilation_error("Not found type for attribute of interface"
                                                      , first->file_position);
      }
    }
  }

  for(std::vector<morbid::idl_parser::op_decl>::const_iterator
        first = i.definition.op_decls.begin()
        , last = i.definition.op_decls.end()
        ;first != last; ++first)
  {
    std::cout << "method" << std::endl;
    std::cout << "return type " << first->type << std::endl;
    if(i.lookups.find(first->type) == i.lookups.end())
    {
      try
      {
        i.lookups.insert(std::make_pair
                          (first->type
                          , morbid::idlc::lookup_type_spec
                          (first->type, current_module, modules_tree)));
      }
      catch(morbid::idlc::lookup_error const& e)
      {
        throw morbid::idlc::compilation_error("Not found type for return of the operation", first->file_position);
      }
    }
    
    for(std::vector<morbid::idl_parser::param_decl>::const_iterator
          pfirst = first->params.begin()
          , plast = first->params.end()
          ;pfirst != plast; ++pfirst)
    {
      if(i.lookups.find(pfirst->type) == i.lookups.end())
      {
        try
        {
        i.lookups.insert(std::make_pair
                          (pfirst->type
                          , morbid::idlc::lookup_type_spec
                          (pfirst->type, current_module, modules_tree)));
        }
        catch(morbid::idlc::lookup_error const& e)
        {
          throw morbid::idlc::compilation_error("Not found type of operation parameter", pfirst->file_position);
        }
      }
    }
  }

  morbid::idlc::module& module = *boost::get(map, current_module.back());
  module.interfaces.push_back(i);

  namespace phoenix = boost::phoenix;
  if(interface.fully_defined)
  {
    morbid::idlc::generator::header_concept_generator<idlc::output_iterator>
      header_concept_generator;

    modules_names.pop_back();
    bool r = karma::generate(output_iterator, header_concept_generator
                              (phoenix::val(module.interfaces.back()), phoenix::val(modules_names))
                              , (module.interfaces.back().definition));
    if(!r) throw std::runtime_error("Failed generating header_concept_generator");
  }
  else
  {
    using morbid::idl_parser::wave_string;
    using karma::_1; using karma::_val;
    using karma::eol;
    namespace fusion = boost::fusion;
    karma::generate(output_iterator
                    , "struct " << idlc::wave_string[_1 = phoenix::at_c<0>(_val)] << "_concept;" << eol
                    << "template <typename T> struct " << idlc::wave_string[_1 = phoenix::at_c<0>(_val)] << "_ref_impl;" << eol
                    << "typedef " << idlc::wave_string[_1 = phoenix::at_c<0>(_val)] << "_ref_impl<void> "
                    << idlc::wave_string[_1 = phoenix::at_c<0>(_val)] << "_ref;" << eol
                    << "typedef ::morbid::reference< " << idlc::wave_string[_1 = phoenix::at_c<0>(_val)]
                    << "_concept> " << idlc::wave_string[_1 = phoenix::at_c<0>(_val)] << ";" << eol
                    , fusion::make_vector(module.interfaces.back().definition.name));
  }
}

} }

