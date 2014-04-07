/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

//#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS

#include <morbid/idl_compiler/generator/concept_generator.ipp>
// #include <morbid/idl_compiler/generator/reference_model_generator.hpp>
// #include <morbid/idl_compiler/generator/concept_interface.hpp>
// #include <morbid/idl_compiler/common_types.hpp>
// #include <morbid/idl_compiler/generate_header_modules_visitor.hpp>
// #include <morbid/idl_compiler/module.hpp>
#include <morbid/idl_compiler/iterator.hpp>

template struct
morbid::idlc::generator::header_concept_generator
 < ::morbid::idlc::output_iterator>;

namespace morbid { namespace idlc {

// void generate_header_modules_visitor::discover_vertex(vertex_descriptor v, modules_tree_type const& g)
// {
//   namespace spirit = boost::spirit;
//   namespace karma = spirit::karma;
//   namespace phoenix = boost::phoenix;
//   namespace fusion = boost::fusion;

//   typedef boost::property_map<modules_tree_type, module_property_t>
//     ::const_type module_map;
//   module_map map = boost::get(module_property_t(), g);
//   idlc::module const& module = *boost::get(map, v);
//   opened_modules.push_back(v);
//   if(opened_modules.size() != 1)
//     modules_names.push_back(module.name);

//   generator::concept_interface<output_iterator> concept_interface;
//   using karma::_1; using karma::_r1; using karma::attr_cast; using idl_parser::wave_string;
//   using karma::eol; using spirit::unused_type;

//   for(std::vector<idlc::interface_>::const_iterator
//         Iter = module.interfaces.begin()
//         , EndIter = module.interfaces.end()
//         ; Iter != EndIter; ++Iter)
//   {
//     if(Iter->definition.fully_defined)
//     {
//       bool r = karma::generate(iterator
//                                , "namespace boost { namespace type_erasure { // specialization of concept_interface"
//                                << eol
//                                << concept_interface(phoenix::ref(*Iter), phoenix::ref(modules_names))
//                                << "}} // specialization of concept_interface" << eol
//                                , Iter->definition);
//       if(!r) throw std::runtime_error("Failed generating header_concept_generator");

//       r = karma::generate(iterator
//                           , "namespace morbid {" << eol
//                           << "template <>" << eol
//                           << "struct is_remote_reference< "
//                           << (*(
//                                 "::"
//                                 << attr_cast<wave_string>(karma::string)
//                              ))
//                           << "::"
//                           << attr_cast<wave_string>(karma::string)
//                           << "_ref > : ::boost::mpl::true_ {};" << eol
//                           << "}" << eol
//                           , fusion::make_vector(modules_names, Iter->definition.name));
//       if(!r) throw std::runtime_error("Failed generating header_concept_generator");

//       r = karma::generate(iterator
//                           , "namespace boost { namespace type_erasure { // specialization of is_placeholder to avoid "
//                           "instantiating morbid::remote< concept > too eagerly" << eol
//                           << "template <>" << eol
//                           << "struct is_placeholder< ::morbid::reference< "
//                           << (*(
//                                 "::"
//                                 << attr_cast<wave_string>(karma::string)
//                              ))
//                           << "::"
//                           << attr_cast<wave_string>(karma::string)
//                           << "_concept> > : ::boost::mpl::false_ {};" << eol
//                           << "}} // specialization of is_placeholder" << eol
//                           , fusion::make_vector(modules_names, Iter->definition.name));
//       if(!r) throw std::runtime_error("Failed generating header_concept_generator");
//     }
//   }

//   for(std::vector<idlc::struct_>::const_iterator
//         Iter = module.structs.begin()
//         , EndIter = module.structs.end()
//         ; Iter != EndIter; ++Iter)
//   {
//     bool r = karma::generate(iterator
//                              , "namespace boost { namespace type_erasure { // specialization of is_placeholder to avoid "
//                              "instantiating morbid::remote< concept > too eagerly" << eol
//                              << "template <>" << eol
//                              << "struct is_placeholder< "
//                              << (*(
//                                    "::"
//                                    << attr_cast<wave_string>(karma::string)
//                                 ))
//                              << "::"
//                              << attr_cast<wave_string>(karma::string)
//                              << "_struct<> > : ::boost::mpl::false_ {};" << eol
//                              << "}} // specialization of is_placeholder" << eol
//                              , fusion::make_vector(modules_names, Iter->definition.name));
//     if(!r) throw std::runtime_error("Failed generating header_concept_generator");
//   }
// }

// void generate_header_modules_visitor::finish_vertex(vertex_descriptor v, modules_tree_type const& g)
// {
//   namespace spirit = boost::spirit;
//   namespace karma = spirit::karma;

//   opened_modules.pop_back();
//   if(!opened_modules.empty())
//     modules_names.pop_back();
// }

// void generate_reference_model_visitor::discover_vertex(vertex_descriptor v, modules_tree_type const& g)
// {
//   namespace spirit = boost::spirit;
//   namespace karma = spirit::karma;
//   namespace phoenix = boost::phoenix;

//   typedef boost::property_map<modules_tree_type, module_property_t>
//     ::const_type module_map;
//   module_map map = boost::get(module_property_t(), g);
//   idlc::module const& module = *boost::get(map, v);
//   opened_modules.push_back(v);
//   if(opened_modules.size() != 1)
//   {
//     modules_names.push_back(module.name);

//     bool r = karma::generate(iterator
//                              , "namespace " << karma::string << " {" << karma::eol
//                              , module.name);
//     if(!r) throw std::runtime_error("Failed generating header_concept_generator");
//   }

//   generator::header_reference_model_generator<output_iterator> reference_model_generator;
//   generator::proxy_reference_generator<output_iterator> proxy_reference_generator;
//   for(std::vector<idlc::interface_>::const_iterator
//         Iter = module.interfaces.begin()
//         , EndIter = module.interfaces.end()
//         ; Iter != EndIter; ++Iter)
//   {
//     if(Iter->definition.fully_defined)
//     {
//       bool r = karma::generate(iterator, reference_model_generator
//                                (phoenix::val(*Iter), phoenix::val(modules_names))
//                                , Iter->definition);
//       if(!r) throw std::runtime_error("Failed generating header_concept_generator");

//       r = karma::generate(iterator, proxy_reference_generator(phoenix::val(*Iter)), Iter->definition);
//       if(!r) throw std::runtime_error("Failed generating header_concept_generator");
//     }
//   }
// }

// void generate_reference_model_visitor::finish_vertex(vertex_descriptor v, modules_tree_type const& g)
// {
//   namespace spirit = boost::spirit;
//   namespace karma = spirit::karma;

//   opened_modules.pop_back();
//   if(!opened_modules.empty())
//   {
//     modules_names.pop_back();

//     typedef boost::property_map<modules_tree_type, module_property_t>
//       ::const_type module_map;
//     module_map map = boost::get(module_property_t(), g);
//     idlc::module const& module = *boost::get(map, v);

//     bool r = karma::generate(iterator
//                              , "} // namespace " << karma::string << " {" << karma::eol
//                              , module.name);
//     if(!r) throw std::runtime_error("Failed generating header_concept_generator");
//   }
// }

} }

