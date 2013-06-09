/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_CONCEPT_GENERATOR_IPP
#define TECORB_IDL_COMPILER_CONCEPT_GENERATOR_IPP

#include <morbid/idl_compiler/generator/proxy_reference_generator.ipp>
#include <morbid/idl_compiler/generator/concept_generator.hpp>

#include <morbid/idl_parser/interface_def.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace boost { namespace spirit { namespace traits {

template <typename Domain>
struct extract_from_container<std::vector< ::morbid::idl_parser::types::scoped_name>
                              , std::vector< ::morbid::idl_parser::type_spec>, Domain>
{
  typedef std::vector< ::morbid::idl_parser::types::scoped_name> from_type;
  typedef std::vector< ::morbid::idl_parser::type_spec> type;

  template <typename Context>
  static type call(from_type const& attr, Context& ctx)
  {
    type r(attr.begin(), attr.end());
    std::cout << "Converting ";
    std::copy(r.begin(), r.end(), std::ostream_iterator<type::value_type>(std::cout, ", "));
    std::endl(std::cout);
    return r;
  }
};

} } }

namespace morbid { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
header_concept_generator<OutputIterator>::header_concept_generator()
  : header_concept_generator::base_type(start)
{
  namespace spirit = boost::spirit;
  namespace phoenix = boost::phoenix;
  using karma::_1;
  using karma::_val;
  using karma::_a;
  using karma::_r1; using karma::_r2; using karma::_r3;
  using karma::eol;
  using phoenix::at_c;

  concept_class =
    karma::eps[_a = at_c<0>(_val)]
    << eol
    << "struct " << wave_string[_1 = _a] << "_concept" << eol
    << eol << "{" << eol
    << indent << "// Start of operations defined in IDL" << eol
    << (*(operation(_r1) << eol))
    [_1 = at_c<1>(_val)]
    << indent << "// End of operations defined in IDL" << eol
    << indent << "// Start of attributes defined in IDL" << eol
    << (*(attribute(_r1) << eol))
    [_1 = at_c<2>(_val)]
    << indent << "// End of attributes defined in IDL" << eol
    << requirements[_1 = _val] << eol
    << indent << "typedef boost::mpl::vector< "
    <<  (
         (base_spec(_r1) % ", ") [_1 = phoenix::at_c<6>(_val)]
         | "::morbid::object_concept"
        )
    << "> bases;" << eol
    << public_members(_r2, _a)[_1 = at_c<0>(_val)] << eol
    << indent << "typedef ::morbid::interface_tag _morbid_type_kind;" << eol
    << indent << "template <typename T> struct proxy_reference;" << eol
    << indent << "typedef " << wave_string[_1 = _a] << "_ref remote_reference;" << eol
    << "};" << eol << eol
    ;
  start =
    karma::eps[_a = at_c<0>(_val)]
    << "struct " << wave_string[_1 = _a] << "_concept;" << eol
    << "struct " << wave_string[_1 = _a] << "_ref;" << eol
    << "typedef ::morbid::reference< " << wave_string[_1 = _a] << "_concept> " << wave_string[_1 = _a] << ';' << eol
    << concept_class(_r1, _r2)[_1 = _val]
    ;
  operation_name = karma::stream[_1 = at_c<1>(_val)];
  args = "arg" << karma::lit(_r1);
  in_tag = karma::string[_1 = " ::morbid::type_tag::in_tag"];
  out_tag = karma::string[_1 = " ::morbid::type_tag::out_tag"];
  inout_tag = karma::string[_1 = " ::morbid::type_tag::inout_tag"];
  arguments = " ::morbid::type_tag::value_type_tag< "
    << type_spec
    (
      at_c<1>(_r1)[at_c<1>(_val)]
    )
    [_1 = at_c<1>(_val)]
    << " , " << (in_tag | out_tag | inout_tag)[_1 = at_c<0>(_val)]
    << " >"
    ;
  attribute = 
    indent
    << "template <typename C = ::boost::type_erasure::_self>" << eol
    << "  struct _get_" << wave_string[_1 = at_c<2>(_val)] << eol
    << "  {\n"
    //    "    template <typename>\n"
    //    "    struct result;\n"
    //    "    template <typename This_>\n"
    //    "    struct result< " << wave_string[_1 = at_c<2>(_val)] << "(This_&)>" << eol
    // << "    {\n"
    //    "      typedef T type;\n"
    //    "    };\n"
    //    "    template <typename This_>\n"
    //    "    struct result< " << wave_string[_1 = at_c<2>(_val)] << "(This_&, T const&)>" << eol
    // << "    {\n"
    //    "      typedef void type;\n"
    //    "    };\n"
    << "    typedef typename ::morbid::lazy_eval< "
    << return_
       (
        at_c<1>(_r1)[at_c<1>(_val)]
       )
       [_1 = at_c<1>(_val)]
    << ", C>::type result_type;\n"
       "    static result_type apply(C const& self)\n"
       "    {\n"
       "      return self._get_" << wave_string[_1 = at_c<2>(_val)] << "();\n"
       "    }\n"
       // "    static void apply(C& self, T const& obj)\n"
       // "    {\n"
       // "      self." << wave_string[_1 = at_c<2>(_val)] << "(obj);\n"
       // "    }\n"
       "    template <typename This_>\n"
       "    result_type operator()(This_* self)\n"
       "    {\n"
       "      return apply(*self);\n"
       "    }\n"
       // "    template <typename This_>\n"
       // "    void operator()(This_* self, T const& obj)\n"
       // "    {\n"
       // "      apply(*self, obj);\n"
       // "    }\n"
       "    inline static const char* name() { return \"_get_" << wave_string[_1 = at_c<2>(_val)] << "\"; }\n"
       "    typedef ::boost::mpl::vector0<> arguments;\n"
       "  };\n"
    ;
  operation =
      indent << "template <typename C = ::boost::type_erasure::_self, typename R = "
      << return_
      (
       at_c<1>(_r1)[at_c<0>(_val)]
      )
      [_1 = at_c<0>(_val)]
      << (*(", typename A" << karma::lit(_a++) << " = "
            << parameter_select(_r1)))[_1 = at_c<2>(_val)]
      << " >" << eol
      << indent << "struct " << operation_name[_1 = _val] << eol
      << indent << '{' << eol
      << indent << indent << "typedef R result_type;" << eol
      // apply
      << indent << indent << "static result_type apply(C& self"
      << karma::eps[_a = 0]
      << -(
           (*(", A"
              << karma::lit(_a)
              << karma::attr_cast<idl_parser::param_decl>(karma::eps)
              << " arg" << karma::lit(++_a)))[_1 = at_c<2>(_val)]
          )
      << ")" << eol
      << indent << indent << "{" << eol
      << indent << indent << indent << "return self." << karma::stream[_1 = at_c<1>(_val)]
      << "(" << karma::eps[_a = 0] << -(args(++_a) % ", ")[_1 = at_c<2>(_val)] << ");" << eol
      << indent << indent << "}" << eol
      // operator()
      << karma::eps[_a = 0]
      << indent << indent << "template <typename This_>" << eol
      << indent << indent << "result_type operator()(This_* self"
      << -(
           (*(", A"
              << karma::lit(_a)
              << karma::attr_cast<idl_parser::param_decl>(karma::eps)
              << " arg" << karma::lit(++_a)))[_1 = at_c<2>(_val)]
          )
      << ")" << eol
      << indent << indent << "{" << eol
      << indent << indent << indent << "return self->" << karma::stream[_1 = at_c<1>(_val)]
      << "(" << karma::eps[_a = 0] << -(args(++_a) % ", ")[_1 = at_c<2>(_val)] << ");" << eol
      << indent << indent << "}" << eol
      // end of operator()
      << indent << indent << "inline static const char* name() { return \"" << operation_name[_1 = _val] << "\"; }" << eol
      << indent << indent << "typedef ::boost::mpl::vector< "
      << -(arguments(_r1) % ", ")[_1 = at_c<2>(_val)] << " > arguments;" << eol
      << indent << "};" << eol
    ;
  base_spec %=
    type_spec(at_c<1>(_r1)[_val])
    << "_concept"
    ;
  attribute_name = "_get_" << wave_string[_1 = at_c<2>(_val)]
    ;
  requirements =
    indent
    << "typedef boost::mpl::vector< "
    << -((operation_name << "<>") % ", ")[_1 = at_c<1>(_val)]
    // << -karma::buffer
    //     [
    //      spirit::eps(!phoenix::empty(at_c<1>(_val)))
    //      [karma::string[_1 = ", "]]
    //      << ((attribute_name << "<>") % ", ")[_1 = at_c<2>(_val)]
    //     ]
    << " >" << eol
    << indent << indent << "requirements;" << eol
    << indent
    << "typedef boost::mpl::vector< ::boost::type_erasure::copy_constructible<>, ::boost::type_erasure::relaxed"
    << (*(", " << operation_name << "<>"))[_1 = at_c<1>(_val)]
    << (*(", " << attribute_name << "<>"))[_1 = at_c<2>(_val)]
    << " >" << eol
    << indent << indent << "regular_requirements;" << eol
    ;
  parameter_select %= parameter(at_c<1>(_r1)[at_c<1>(_val)]);

  public_members = 
    indent
    << "inline static const char* type_id() { return \"IDL:"
    << (*(wave_string << '/'))[_1 = _r1] << wave_string[_1 = _r2] << ":1.0\"; }" << eol
    ;
  indent = karma::space << karma::space;
  wave_string %= karma::string;
}

} } }

#endif
