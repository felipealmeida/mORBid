/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_COMMON_TYPES
#define MORBID_IDL_COMPILER_COMMON_TYPES

// #include <morbid/idl_parser/type_spec.hpp>
// #include <morbid/idl_parser/interface_def.hpp>
// #include <morbid/idl_parser/typedef_def.hpp>
// #include <morbid/idl_parser/exception_def.hpp>
// #include <morbid/idl_parser/struct_def.hpp>

// #include <boost/spirit/home/lex.hpp>
// #include <boost/spirit/home/lex/lexer_lexertl.hpp>
#include <boost/graph/adjacency_list.hpp>
// #include <boost/shared_ptr.hpp>
// #include <boost/wave.hpp>
// #include <boost/wave/cpplexer/cpp_lex_iterator.hpp>

// #include <vector>
// #include <iterator>
// #include <ostream>

namespace boost { namespace spirit { namespace traits {

// template <>
// struct char_type_of< ::morbid::idl_parser::wave_string>
// {
//   typedef char type;
// };

// template <>
// struct is_string< ::morbid::idl_parser::wave_string> : mpl::true_ {};

// template <typename Domain>
// struct transform_attribute<std::string, ::morbid::idl_parser::wave_string, Domain>
// {
//    typedef ::morbid::idl_parser::wave_string type;

//     static type pre(std::string const& val)
//     {
//       type r;
//       r.insert(r.end(), val.begin(), val.end());
//       return r;
//     }
//     static void post(std::string const& val, type attr) {}    // Qi only
//     static void fail(std::string const&) {}                   // Qi only
// };

// template <typename Domain>
// struct transform_attribute< ::morbid::idl_parser::wave_string, std::string, Domain>
// {
//     typedef ::morbid::idl_parser::wave_string exposed_type;
//     typedef std::string type;

//     static type pre(exposed_type const& val)
//     {
//       type r;
//       r.insert(r.end(), val.begin(), val.end());
//       return r;
//     }
//     static void post(exposed_type const& val, type attr) {}    // Qi only
//     static void fail(exposed_type const&) {}                   // Qi only
// };

// template <>
// struct extract_c_string< ::morbid::idl_parser::wave_string>
// {
//     typedef char char_type;

//     static char_type const* call (::morbid::idl_parser::wave_string const& s) { return s.c_str(); }
// };


} } }

namespace morbid { namespace idlc {

// typedef boost::wave::cpplexer::lex_token<> token_type;
// typedef boost::wave::cpplexer::lex_iterator
//   <token_type> lex_iterator_type;
// typedef boost::wave::context
//   <std::vector<char>::const_iterator, lex_iterator_type> context_type;

// typedef context_type::iterator_type parser_iterator_type;

// typedef std::ostream_iterator<char> output_iterator_type;

struct module_property_t
{
  typedef boost::vertex_property_tag kind;
};

struct module;

typedef boost::property<module_property_t, boost::shared_ptr<module>
                        /*, boost::property<boost::vertex_color_t, boost::default_color_type>*/ >
  module_property_type;
typedef boost::adjacency_list<boost::setS, boost::listS, boost::bidirectionalS
                              , module_property_type>
  modules_tree_type;
typedef modules_tree_type::vertex_descriptor vertex_descriptor;
typedef modules_tree_type::edge_descriptor edge_descriptor;

} }


#endif
