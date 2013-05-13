/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/idl_parser/tokenizer.hpp>
#include <morbid/idl_parser/grammar/interface_def.hpp>
#include <morbid/idl_parser/grammar/module_open.hpp>
#include <morbid/idl_parser/grammar/typedef_def.hpp>
#include <morbid/idl_parser/grammar/exception_def.hpp>
#include <morbid/idl_parser/grammar/struct_def.hpp>
#include <morbid/idl_compiler/interface.hpp>
#include <morbid/idl_compiler/module.hpp>
#include <morbid/idl_compiler/lookup.hpp>
#include <morbid/idl_compiler/generate_header_modules_visitor.hpp>
#include <morbid/idl_compiler/common_types.hpp>

#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/lex/qi.hpp>
#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>

#include <iostream>

//     struct include_paths
//     {
//       include_paths() : seen_separator(false) {}

//       std::vector<std::string> paths;       // stores user paths
//       std::vector<std::string> syspaths;    // stores system paths
//       bool seen_separator;        // command line contains a '-I-' option

//       // Function which validates additional tokens from command line.
//       static void 
//       validate(boost::any &v, std::vector<std::string> const &tokens)
//       {
//         if (v.empty())
//           v = boost::any(include_paths());

//         include_paths *p = boost::any_cast<include_paths>(&v);

//         BOOST_ASSERT(p);
//         // Assume only one path per '-I' occurrence.
//         std::string t = tokens[0];
//         if (t == "-")
//         {
//           // found -I- option, so switch behaviour
//           p->seen_separator = true;
//         } 
//         else if (p->seen_separator)
//         {
//           // store this path as a system path
//           p->syspaths.push_back(t); 
//         } 
//         else
//         {
//           // store this path as an user path
//           p->paths.push_back(t);
//         }            
//       }
//     };

// std::ostream& operator<<(std::ostream& os, include_paths const&)
// {
//   return os << "include_paths";
// }
// std::istream& operator>>(std::istream& is, include_paths&)
// {
//   return is;
// }

// namespace boost { namespace program_options {

//   void validate(boost::any &v, std::vector<std::string> const &s,
//       ::include_paths *, int) 
//   {
//       ::include_paths::validate(v, s);
//   }

// }}  // namespace boost::program_options

int main(int argc, char** argv)
{
  boost::program_options::options_description desc("Allowed options");
  desc.add_options()
    ("help", "Shows this message")
    ("input,i", boost::program_options::value<std::string>(), "input IDL file")
    ("output,o", boost::program_options::value<std::string>(), "output base filenames")
    ("include,I", boost::program_options::value<std::vector<std::string> >()
     , "specify an additional include directory")
    ;

  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc)
                                , vm);
  boost::program_options::notify(vm);

  if(vm.count("help") || !vm.count("input"))
  {
    std::cout << desc << std::endl;
    return 1;
  }

  boost::filesystem::path input_file = vm["input"].as<std::string>();
  if(boost::filesystem::exists(input_file)
     && boost::filesystem::is_regular_file(input_file))
  {
    boost::filesystem::ifstream ifs(input_file);
    if(ifs.is_open())
    {
      ifs.seekg(0, std::ios::end);
      std::vector<char> buffer(ifs.tellg());
      ifs.seekg(0, std::ios::beg);
      if(!buffer.empty())
      {
        ifs.rdbuf()->sgetn(&buffer[0], buffer.size());

        typedef boost::wave::cpplexer::lex_iterator<
          boost::wave::cpplexer::lex_token<> >
          lex_iterator_type;
        typedef boost::wave::context<
          std::vector<char>::const_iterator, lex_iterator_type>
          context_type;

        context_type ctx(buffer.begin(), buffer.end(), input_file.string().c_str());
        ctx.set_language
          (static_cast<boost::wave::language_support>
           (
            (int)ctx.get_language()
            & (int)~boost::wave::support_option_emit_line_directives
            // & (int)~boost::wave::support_option_insert_whitespace
            & (int)~boost::wave::support_option_emit_pragma_directives
           ));

        // add include directories to the include search paths
        if (vm.count("include"))
        {
          std::vector<std::string> ip = vm["include"].as<std::vector<std::string> >();
          std::vector<std::string>::const_iterator end = ip.end();

          for (std::vector<std::string>::const_iterator cit = ip.begin(); 
               cit != end; ++cit)
          {
            ctx.add_include_path((*cit).c_str());
          }
          
          // // if on the command line was given -I- , this has to be propagated
          // if (ip.seen_separator) 
          //   ctx.set_sysinclude_delimiter();
                 
          // // add system include directories to the include path
          // vector<string>::const_iterator sysend = ip.syspaths.end();
          // for (vector<string>::const_iterator syscit = ip.syspaths.begin(); 
          //      syscit != sysend; ++syscit)
          // {
          //   ctx.add_sysinclude_path((*syscit).c_str());
          // }
        }

        typedef context_type::iterator_type iterator_type;
        iterator_type iterator = ctx.begin(), last = ctx.end();

        morbid::idl_parser::grammar::interface_definition<iterator_type> interface_grammar;
        morbid::idl_parser::grammar::module_open<iterator_type> module_open_grammar;
        morbid::idl_parser::grammar::typedef_definition<iterator_type> typedef_grammar;
        morbid::idl_parser::grammar::exception_definition<iterator_type> exception_grammar;
        morbid::idl_parser::grammar::struct_definition<iterator_type> struct_grammar;
        morbid::idl_parser::grammar::skipper<iterator_type> skipper;
        namespace qi = boost::spirit::qi;
        namespace lex = boost::spirit::lex;
        namespace phoenix = boost::phoenix;

        typedef morbid::idl_compiler::module module_type;
        typedef morbid::idl_compiler::module_property_t module_property_t;
        typedef morbid::idl_compiler::module_property_type module_property_type;
        module_property_type module_property;
        typedef morbid::idl_compiler::modules_tree_type modules_tree_type;
        modules_tree_type modules_tree;

        typedef morbid::idl_compiler::vertex_descriptor vertex_descriptor;
        module_property_type p1(boost::shared_ptr<module_type>(new module_type("")));
        vertex_descriptor global_module = add_vertex
          (p1, modules_tree);
        module_property_type p2(boost::shared_ptr<module_type>(new module_type("::morbid")));
        vertex_descriptor primitive_types_module = add_vertex
          (p2, modules_tree);

        std::vector<vertex_descriptor> current_module;
        current_module.push_back(primitive_types_module);
        current_module.push_back(global_module);

        typedef boost::property_map<modules_tree_type, module_property_t>
          ::type module_map;
        
        do
        {
          try
          {
          morbid::idl_parser::wave_string module_open;
          morbid::idl_compiler::interface_def_type interface;
          morbid::idl_compiler::typedef_def_type typedef_;
          morbid::idl_parser::exception_def exception;
          morbid::idl_compiler::struct_def_type struct_;
          if(boost::spirit::qi::phrase_parse(iterator, last, module_open_grammar
                                             , skipper, module_open))
          {
            std::cout << "Opened module " << module_open << std::endl;
            current_module.push_back
              (add_vertex(module_property_type
                          (boost::shared_ptr<module_type>(new module_type(module_open.c_str())))
                          , modules_tree));
            add_edge(*boost::prior(current_module.end(), 2)
                     , current_module.back(), modules_tree);
          }
          else if(boost::spirit::qi::parse(iterator, last, qi::char_('}') >> ';'))
          {
            if(current_module.size() > 1)
            {
              std::cout << "Closing module " << current_module.back() << std::endl;
              current_module.pop_back();
            }
            else
              throw std::runtime_error("Error, closing non existant module");
          }
          else if(boost::spirit::qi::parse(iterator, last, typedef_grammar >> qi::omit[';'], typedef_))
          {
            std::cout << "typedef " << typedef_ << std::endl;

            typedef morbid::idl_compiler::typedef_ typedef_type;
            typedef_type t(typedef_);
            t.lookup = morbid::idl_compiler::lookup_type_spec
              (typedef_.alias, current_module, modules_tree);

            module_map map = get(module_property_t(), modules_tree);
            boost::get(map, current_module.back())
              ->typedefs.push_back(t);
          }
          else if(boost::spirit::qi::phrase_parse
                  (iterator, last
                   , (exception_grammar[phoenix::ref(exception) = qi::_1]
                      >> morbid::idl_parser::token_value(";"))
                   , skipper))
          {
            std::cout << "exception " << exception << std::endl;
          }
          else if(boost::spirit::qi::parse(iterator, last
                                           , struct_grammar[phoenix::ref(struct_) = qi::_1] >> ';'))
          {
            std::cout << "struct " << struct_ << std::endl;

            typedef morbid::idl_compiler::struct_ struct_type;
            typedef morbid::idl_compiler::struct_member_type struct_member_type;
            struct_type s(struct_);

            for(std::vector<struct_member_type>::const_iterator
                  first = s.definition.members.begin()
                  , last = s.definition.members.end()
                  ;first != last; ++first)
            {
              if(s.lookups.find(first->type) == s.lookups.end())
              {
                s.lookups.insert(std::make_pair
                                 (first->type
                                  , morbid::idl_compiler::lookup_type_spec
                                  (first->type, current_module, modules_tree)));
              }
            }

            module_map map = get(module_property_t(), modules_tree);
            boost::get(map, current_module.back())
              ->structs.push_back(s);
          }
          else if(boost::spirit::qi::parse(iterator, last, interface_grammar >> qi::omit[';'], interface))
          {
            std::cout << "interface " << interface << std::endl;
            typedef morbid::idl_compiler::module module;
            typedef morbid::idl_compiler::interface_ interface_type;
            typedef morbid::idl_compiler::op_decl_type op_decl_type;
            typedef morbid::idl_compiler::param_decl param_decl;
            interface_type i(interface);

            // op_decl_type is_a_op_decl = {morbid::idl_parser::types::boolean(), "_is_a"};
            // morbid::idl_parser::types::scoped_name string_scoped_name = {false};
            // string_scoped_name.identifiers.push_back("string");
            // param_decl param = {morbid::idl_parser::direction::in(), string_scoped_name};
            // is_a_op_decl.params.push_back(param);
            // is_a_op_decl.user_defined = false;
            // i.definition.op_decls.push_back(is_a_op_decl);

            module_map map = get(module_property_t(), modules_tree);
            std::vector<std::string> modules_names;
            for(std::vector<vertex_descriptor>::const_iterator
                  first = boost::next(current_module.begin(), 2)
                  , last = current_module.end()
                  ;first != last; ++first)
            {
              module const* mx = &*boost::get(map, *first);
              modules_names.push_back(mx->name);
            }

            modules_names.push_back(interface.name);
            i.definition.repoids.push_back("IDL:omg.org/CORBA/Object:1.0");
            std::string repoid;
            namespace karma = boost::spirit::karma;
            if(!karma::generate(std::back_insert_iterator<std::string>(repoid)
                                , "IDL:" << (karma::string % '/') << ":1.0"
                                , modules_names))
              throw std::runtime_error("Failed constructing RepoID");
            i.definition.repoids.push_back(repoid);
            assert(i.definition.repoids.size() == 2);

            for(std::vector<op_decl_type>::const_iterator
                  first = i.definition.op_decls.begin()
                  , last = i.definition.op_decls.end()
                  ;first != last; ++first)
            {
              std::cout << "method" << std::endl;
              std::cout << "return type " << first->type << std::endl;
              if(i.lookups.find(first->type) == i.lookups.end())
              {
                i.lookups.insert(std::make_pair
                                 (first->type
                                  , morbid::idl_compiler::lookup_type_spec
                                  (first->type, current_module, modules_tree)));
              }
              typedef morbid::idl_compiler::param_decl param_decl;
              for(std::vector<param_decl>::const_iterator
                    pfirst = first->params.begin()
                    , plast = first->params.end()
                    ;pfirst != plast; ++pfirst)
              {
                if(i.lookups.find(pfirst->type) == i.lookups.end())
                {
                  i.lookups.insert(std::make_pair
                                   (pfirst->type
                                    , morbid::idl_compiler::lookup_type_spec
                                    (pfirst->type, current_module, modules_tree)));
                }
              }
            }

            boost::get(map, current_module.back())
              ->interfaces.push_back(i);
          }
          else
          {
            std::cout << iterator->get_position().get_file()
                      << ":" << iterator->get_position().get_line() 
                      << ":" << iterator->get_position().get_column()
                      << ": syntax error"
                      << std::endl;
            return 1;
          }
          }
          catch(boost::wave::preprocess_exception const& e)
          {
            std::cout << e.file_name()
                      << ":" << e.line_no() 
                      << ":" << e.column_no()
                      << ": failed preprocessing file " << e.description()
                      << std::endl;
            return 1;
          }
        }
        while(!boost::spirit::qi::parse(iterator, last, qi::eoi));

        std::cout << "Finished file" << std::endl;
        if(current_module.size() != 2)
          throw std::runtime_error("Error: Not closed all modules");

        boost::filesystem::path header_path, impl_path;
        if(vm.count("output"))
        {
          header_path = vm["output"].as<std::string>() + ".hpp";
        }
        else
        {
          header_path = input_file;
          header_path.replace_extension(".hpp");
          // impl_path = input_file;
          // impl_path.replace_extension(".cpp");
        }

        boost::filesystem::ofstream header(header_path);
        // boost::filesystem::ofstream cpp(impl_path);
        typedef std::map<vertex_descriptor, boost::default_color_type> color_map_container_t;
        typedef boost::associative_property_map<color_map_container_t> color_map_t;
        if(header.is_open()/* && cpp.is_open()*/)
        {
          namespace karma = boost::spirit::karma;
          using morbid::idl_compiler::output_iterator_type;
          {
            output_iterator_type iterator(header);

            bool r = karma::generate
              (iterator
               , karma::lit("// -*- c++ -*-") << karma::eol
               << "// Generated header. DO NOT EDIT" << karma::eol << karma::eol
               << karma::lit("#include <morbid/handle_request_body.hpp>") << karma::eol
               << karma::lit("#include <morbid/reply.hpp>") << karma::eol
               << karma::lit("#include <morbid/structured_ior.hpp>") << karma::eol
               << karma::lit("#include <morbid/in_out_traits.hpp>") << karma::eol << karma::eol
               << karma::lit("#include <morbid/synchronous_call.hpp>") << karma::eol << karma::eol
               << karma::lit("#include <morbid/orb.hpp>") << karma::eol << karma::eol
               << karma::lit("#include <morbid/reference.hpp>") << karma::eol << karma::eol
               << karma::lit("#include <boost/integer.hpp>") << karma::eol
               << karma::lit("#include <boost/fusion/include/vector10.hpp>") << karma::eol
               << karma::lit("#include <boost/fusion/include/vector20.hpp>") << karma::eol
               << karma::lit("#include <boost/fusion/include/vector30.hpp>") << karma::eol
               << karma::lit("#include <boost/fusion/include/vector40.hpp>") << karma::eol
               << karma::lit("#include <boost/fusion/include/vector50.hpp>") << karma::eol
               << karma::lit("#include <boost/type_erasure/any.hpp>") << karma::eol
               << karma::lit("#include <boost/type_erasure/member.hpp>") << karma::eol
               << karma::lit("#include <boost/type_erasure/concept_interface.hpp>") << karma::eol
               << karma::eol
               );
            if(!r) 
              throw std::runtime_error("Failed generating #includes for header");
            {
              color_map_container_t color_map_container;
              color_map_t color_map(color_map_container);
              morbid::idl_compiler::generate_header_modules_visitor header_modules_visitor (iterator);
              breadth_first_visit(modules_tree, global_module
                                  , boost::visitor(header_modules_visitor)
                                  .color_map(color_map));

              typedef boost::property_map<modules_tree_type, module_property_t>
                ::type module_map;
              module_map map = get(module_property_t(), modules_tree);
              for(std::size_t l = header_modules_visitor.state->opened_modules.size() - 1
                    ;l != 0;--l)
              {
                morbid::idl_compiler::module const& m
                  = *boost::get(map, header_modules_visitor.state->opened_modules[l]);
                
                *iterator++ = '}';
                *iterator++ = ' ';
                *iterator++ = '/';
                *iterator++ = '/';
                iterator = std::copy(m.name.begin(), m.name.end(), iterator);
                karma::generate(iterator, karma::eol);
              }
            }

            {
              color_map_container_t color_map_container;
              color_map_t color_map(color_map_container);
              morbid::idl_compiler::generate_header_poa_modules_visitor header_poa_modules_visitor (iterator);
              breadth_first_visit(modules_tree, global_module
                                  , boost::visitor(header_poa_modules_visitor)
                                  .color_map(color_map));
            }
          }                  
        }
        else
        {
          std::cout << "Couldn't open output file" << std::endl;
        }

        return 0;
      }
      else
      {
        std::cout << "File " << vm["input"].as<std::string>() << " is empty" << std::endl;
      }
    }
    else
    {
      std::cout << "File " << vm["input"].as<std::string>() << " couldn't be open" << std::endl;
    }
  }
  else
  {
    std::cout << "File " << vm["input"].as<std::string>() << " doesnt exist or is not a regular file" << std::endl;
  }
  return -1;
}
