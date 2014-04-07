/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_CONCEPT_GENERATOR_IPP
#define TECORB_IDL_COMPILER_CONCEPT_GENERATOR_IPP

#include <morbid/idl_compiler/generator/proxy_reference_generator.ipp>
#include <morbid/idl_compiler/generator/wave_string.hpp>
#include <morbid/idl_compiler/generator/concept_generator.hpp>

#include <morbid/idl_parser/interface_def.hpp>
#include <boost/spirit/home/support.hpp>
#include <boost/phoenix.hpp>
#include <boost/phoenix/fusion/at.hpp>
#include <boost/spirit/home/karma.hpp>

#include <boost/phoenix/operator/self.hpp>

// #include <boost/spirit/home/support/terminal.hpp>
// #include <boost/spirit/home/support/common_terminals.hpp>
// #include <boost/spirit/home/support/argument.hpp>
// #include <boost/spirit/home/support/context.hpp>
// #include <boost/spirit/home/karma/auxiliary/attr_cast.hpp>
// #include <boost/spirit/home/karma/string.hpp>

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

namespace morbid { namespace idlc { namespace generator {

template <typename OutputIterator>
header_concept_generator<OutputIterator>::header_concept_generator()
  : header_concept_generator::base_type(start)
{
  namespace spirit = boost::spirit;
  namespace karma = spirit::karma;
  namespace phoenix = boost::phoenix;

  using karma::_a;

  spirit::_1_type const _1 = spirit::_1_type();
  // spirit::_a_type const _a = spirit::_a_type();
  spirit::_val_type const _val = spirit::_val_type();
  typedef phoenix::actor<spirit::attribute<1> > _r1_type;
  typedef phoenix::actor<spirit::attribute<2> > _r2_type;
  spirit::_r1_type const _r1 = spirit::_r1_type();
  spirit::_r2_type const _r2 = spirit::_r2_type();
  spirit::eol_type const eol = spirit::eol_type();
  spirit::eps_type const eps = spirit::eps_type();
  spirit::ascii::space_type const space = spirit::ascii::space_type();
  spirit::ascii::string_type const string = spirit::ascii::string_type();
  spirit::stream_type const stream = spirit::stream_type();
  spirit::lit_type const lit = spirit::lit_type();
  wave_string_type const wave_string = wave_string_type();

  using spirit::attr_cast;
  using phoenix::at_c;
  using phoenix::find;
  using phoenix::second;

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsequenced"
#endif

  concept_class =
    eps[_a = at_c<0>(_val)]
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
    eps[_a = at_c<0>(_val)]
    <<
    "struct "
    << wave_string[_1 = _a] << "_concept;" << eol
    << "template <typename T = void> struct " << wave_string[_1 = _a] << "_ref_impl;" << eol
    << "typedef " << wave_string[_1 = _a] << "_ref_impl<void> " << wave_string[_1 = _a] << "_ref;" << eol
    << "typedef ::morbid::reference< " << wave_string[_1 = _a] << "_concept> " << wave_string[_1 = _a] << ';' << eol
    << 
    concept_class(_r1, _r2)[_1 = _val]
    ;
  operation_name = stream[_1 = at_c<1>(_val)];
  args = "arg" << lit(_r1);
  in_tag = string[_1 = " ::morbid::type_tag::in_tag"];
  out_tag = string[_1 = " ::morbid::type_tag::out_tag"];
  inout_tag = string[_1 = " ::morbid::type_tag::inout_tag"];
  arguments = " ::morbid::type_tag::value_type_tag< "
    << type_spec
    (
      second(*find(at_c<1>(_r1), at_c<1>(_val)))
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
        second(*find(at_c<1>(_r1), at_c<1>(_val)))
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
    indent
    << "template <typename C = ::boost::type_erasure::_self, typename R = "
    << return_
    (
     second(*find(at_c<1>(_r1), at_c<0>(_val)))
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
    << eps[_a = 0]
    << -(
         (*(", A"
            << lit(_a)
            << attr_cast<idl_parser::param_decl>(eps)
            << " arg" << lit(++_a)))[_1 = at_c<2>(_val)]
        )
    << ")" << eol
    << indent << indent << "{" << eol
    << indent << indent << indent << "return self." << stream[_1 = at_c<1>(_val)]
    << "(" << eps[_a = 0] << -(args(++_a) % ", ")[_1 = at_c<2>(_val)] << ");" << eol
    << indent << indent << "}" << eol
    // operator()
    << eps[_a = 0]
    << indent << indent << "template <typename This_>" << eol
    << indent << indent << "result_type operator()(This_* self"
    << -(
         (*(", A"
            << lit(_a)
            << attr_cast<idl_parser::param_decl>(eps)
            << " arg" << lit(++_a)))[_1 = at_c<2>(_val)]
        )
    << ")" << eol
    << indent << indent << "{" << eol
    << indent << indent << indent << "return self->" << stream[_1 = at_c<1>(_val)]
    << "(" << eps[_a = 0] << -(args(++_a) % ", ")[_1 = at_c<2>(_val)] << ");" << eol
    << indent << indent << "}" << eol
    // end of operator()
    << indent << indent << "inline static const char* name() { return \"" << operation_name[_1 = _val] << "\"; }" << eol
    << indent << indent << "typedef ::boost::mpl::vector< "
    << -(arguments(_r1) % ", ")[_1 = at_c<2>(_val)] << " > arguments;" << eol
    << indent << "};" << eol
    ;
  base_spec %=
    type_spec(second(*find(at_c<1>(_r1), _val)))
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
  parameter_select %= parameter(second(*find(at_c<1>(_r1), at_c<1>(_val))));

  public_members = 
    indent
    << "inline static const char* type_id() { return \"IDL:"
    << (*(wave_string << '/'))[_1 = _r1] << wave_string[_1 = _r2] << ":1.0\"; }" << eol
    ;
  indent = space << space;

#ifdef __clang__
#pragma clang diagnostic pop
#endif
}

} } }

#endif
