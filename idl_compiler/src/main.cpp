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
#include <morbid/idl_compiler/generate_cpp_modules_visitor.hpp>
#include <morbid/idl_compiler/common_types.hpp>

#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/lex/qi.hpp>

#include <iostream>

int main(int argc, char** argv)
{
  boost::program_options::options_description desc("Allowed options");
  desc.add_options()
    ("help", "Shows this message")
    ("input,i", boost::program_options::value<std::string>(), "input IDL file")
    ("output,o", boost::program_options::value<std::string>(), "output base filenames")
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

        using morbid::idl_compiler::lexer_type;
        
        morbid::idl_parser::tokens<lexer_type> lexer;

        std::vector<char>::const_iterator buffer_begin = buffer.begin()
          , buffer_end = buffer.end();

        typedef morbid::idl_compiler::parser_iterator_type iterator_type;
        iterator_type iterator = lexer.begin(buffer_begin, buffer_end)
          , last = lexer.end();


        morbid::idl_parser::grammar::interface_definition<iterator_type> interface_grammar(lexer);
        morbid::idl_parser::grammar::module_open<iterator_type> module_open_grammar(lexer);
        morbid::idl_parser::grammar::typedef_definition<iterator_type> typedef_grammar(lexer);
        morbid::idl_parser::grammar::exception_definition<iterator_type> exception_grammar(lexer);
        morbid::idl_parser::grammar::struct_definition<iterator_type> struct_grammar(lexer);
        morbid::idl_parser::skipper<iterator_type> skipper(lexer);
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

        typedef typename boost::property_map<modules_tree_type, module_property_t>
          ::type module_map;
        
        do
        {
          std::string module_open;
          morbid::idl_compiler::interface_def_type interface;
          morbid::idl_compiler::typedef_def_type typedef_;
          morbid::idl_compiler::exception_def_type exception;
          morbid::idl_compiler::struct_def_type struct_;
          if(boost::spirit::qi::phrase_parse(iterator, last, module_open_grammar, skipper, module_open))
          {
            std::cout << "Opened module " << module_open << std::endl;
            current_module.push_back
              (add_vertex(module_property_type
                          (boost::shared_ptr<module_type>(new module_type(module_open)))
                          , modules_tree));
            add_edge(*boost::prior(current_module.end(), 2)
                     , current_module.back(), modules_tree);
          }
          else if(boost::spirit::qi::phrase_parse(iterator, last, qi::char_('}') >> ';', skipper))
          {
            if(current_module.size() > 1)
            {
              std::cout << "Closing module " << current_module.back() << std::endl;
              current_module.pop_back();
            }
            else
              throw std::runtime_error("Error, closing non existant module");
          }
          else if(boost::spirit::qi::phrase_parse(iterator, last, typedef_grammar >> qi::omit[';'], skipper, typedef_))
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
          else if(boost::spirit::qi::phrase_parse(iterator, last
                                                  , exception_grammar[phoenix::ref(exception) = qi::_1] >> ';'
                                                  , skipper))
          {
            std::cout << "exception " << exception << std::endl;
          }
          else if(boost::spirit::qi::phrase_parse(iterator, last
                                                  , struct_grammar[phoenix::ref(struct_) = qi::_1] >> ';'
                                                  , skipper))
          {
            std::cout << "struct " << struct_ << std::endl;
          }
          else if(boost::spirit::qi::phrase_parse(iterator, last, interface_grammar >> qi::omit[';'], skipper, interface))
          {
            std::cout << "interface " << interface << std::endl;
            typedef morbid::idl_compiler::interface_ interface_type;
            typedef morbid::idl_compiler::op_decl_type op_decl_type;
            typedef morbid::idl_compiler::param_decl param_decl;
            interface_type i(interface);

            op_decl_type is_a_op_decl = {morbid::idl_parser::types::boolean(), "_is_a"};
            morbid::idl_parser::types::scoped_name string_scoped_name = {false};
            string_scoped_name.identifiers.push_back("string");
            param_decl param = {morbid::idl_parser::direction::in(), string_scoped_name};
            is_a_op_decl.params.push_back(param);
            is_a_op_decl.user_defined = false;
            i.definition.op_decls.push_back(is_a_op_decl);

            i.definition.repoids.push_back("IDL:omg.org/CORBA/Object:1.0");
            i.definition.repoids.push_back("IDL:" + interface.name + ":1.0");
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

            module_map map = get(module_property_t(), modules_tree);
            boost::get(map, current_module.back())
              ->interfaces.push_back(i);
          }
          else
          {
            throw std::runtime_error("Error: Nothing matched");
          }
        }
        while(!boost::spirit::qi::phrase_parse(iterator, last, qi::eoi, skipper));

        std::cout << "Finished file" << std::endl;
        if(current_module.size() != 2)
          throw std::runtime_error("Error: Not closed all modules");

        boost::filesystem::path header_path, impl_path;
        if(vm.count("output"))
        {
          header_path = vm["output"].as<std::string>() + ".h";
          impl_path = vm["output"].as<std::string>() + ".cpp";
        }
        else
        {
          header_path = input_file;
          header_path.replace_extension(".h");
          impl_path = input_file;
          impl_path.replace_extension(".cpp");
        }

        boost::filesystem::ofstream header(header_path);
        boost::filesystem::ofstream cpp(impl_path);
        typedef boost::property_map<modules_tree_type, boost::vertex_color_t>::type
          color_map_t;
        typedef boost::queue<vertex_descriptor> queue_t;
        if(header.is_open() && cpp.is_open())
        {
          namespace karma = boost::spirit::karma;
          using morbid::idl_compiler::output_iterator_type;
          {
            output_iterator_type iterator(header);

            bool r = karma::generate
              (iterator
               , karma::lit("// -*- c++ -*-") << karma::eol
               << "// Generated header. DO NOT EDIT" << karma::eol << karma::eol
               << karma::lit("#include <morbid/poa.hpp>") << karma::eol
               << karma::lit("#include <morbid/handle_request_body.hpp>") << karma::eol
               << karma::lit("#include <morbid/reply.hpp>") << karma::eol
               << karma::lit("#include <morbid/in_out_traits.hpp>") << karma::eol
               << karma::lit("#include <CORBA.h>") << karma::eol << karma::eol
               << karma::lit("#include <boost/integer.hpp>") << karma::eol
               << karma::eol
               // << "class POA_" << interface.name << ";" << karma::eol << karma::eol
               );
            if(!r) 
              throw std::runtime_error("Failed generating #includes for header");
            {
              color_map_t color_map;
              queue_t queue;
              morbid::idl_compiler::generate_header_modules_visitor header_modules_visitor (iterator);
              breadth_first_visit(modules_tree, global_module, queue
                                  , header_modules_visitor
                                  , color_map);

              typedef typename boost::property_map<modules_tree_type, module_property_t>
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
              color_map_t color_map;
              queue_t queue;
              morbid::idl_compiler::generate_header_poa_modules_visitor header_poa_modules_visitor (iterator);
              breadth_first_visit(modules_tree, global_module, queue
                                  , header_poa_modules_visitor
                                  , color_map);
            }
          }                  

          {
            output_iterator_type iterator(cpp);
          //     morbid::idl_compiler::generator::cpp_stub_generator
          //       <output_iterator_type, iterator_type>
          //       cpp_stub_generator;
          //     morbid::idl_compiler::generator::cpp_local_stub_generator
          //       <output_iterator_type, iterator_type>
          //       cpp_local_stub_generator;
          //     morbid::idl_compiler::generator::header_remote_stub_generator
          //       <output_iterator_type, iterator_type>
          //       header_remote_stub_generator;
          //     morbid::idl_compiler::generator::cpp_remote_stub_generator
          //       <output_iterator_type, iterator_type>
          //       cpp_remote_stub_generator;
          //     morbid::idl_compiler::generator::cpp_poa_stub_generator
          //       <output_iterator_type, iterator_type>
          //       cpp_poa_stub_generator;

            bool r = karma::generate
              (iterator
               , karma::lit("// -*- c++ -*-") << karma::eol
               << "// Generated implementation. DO NOT EDIT" << karma::eol << karma::eol
               << "#include \"" << karma::lit(header_path.filename().native()) << "\"" << karma::eol
               << "#include <morbid/synchronous_call.hpp>" << karma::eol
               << karma::eol
               );
            if(!r) 
              throw std::runtime_error("Failed generating #includes for cpp");

            {
              color_map_t color_map;
              queue_t queue;
              morbid::idl_compiler::generate_cpp_modules_visitor cpp_modules_visitor (iterator);
              breadth_first_visit(modules_tree, global_module, queue
                                  , cpp_modules_visitor
                                  , color_map);
              typedef typename boost::property_map<modules_tree_type, module_property_t>
                ::type module_map;
              module_map map = get(module_property_t(), modules_tree);
              for(std::size_t l = cpp_modules_visitor.state->opened_modules.size() - 1
                    ;l != 0;--l)
              {
                morbid::idl_compiler::module const& m
                  = *boost::get(map, cpp_modules_visitor.state->opened_modules[l]);
                
                *iterator++ = '}';
                *iterator++ = ' ';
                *iterator++ = '/';
                *iterator++ = '/';
                iterator = std::copy(m.name.begin(), m.name.end(), iterator);
                karma::generate(iterator, karma::eol);
              }
            }

            {
              color_map_t color_map;
              queue_t queue;
              morbid::idl_compiler::generate_cpp_poa_modules_visitor cpp_poa_modules_visitor (iterator);
              breadth_first_visit(modules_tree, global_module, queue
                                  , cpp_poa_modules_visitor
                                  , color_map);
            }
          //     bool r;
          //     r = karma::generate(iterator, header_remote_stub_generator, interface);
          //     if(!r) std::cout << "Failed generating header_remote_stub_generator for cpp" << std::endl;
          //     r = karma::generate(iterator, cpp_remote_stub_generator, interface);
          //     if(!r) std::cout << "Failed generating cpp_remote_stub_generator for cpp" << std::endl;
          //     r = karma::generate(iterator, cpp_stub_generator, interface);
          //     if(!r) std::cout << "Failed generating cpp_stub_generator for cpp" << std::endl;
          //     r = karma::generate(iterator, cpp_local_stub_generator, interface);
          //     if(!r) std::cout << "Failed generating cpp_local_stub_generator for cpp" << std::endl;
          //     assert(interface.repoids.size() == 2);
          //     r = karma::generate(iterator, cpp_poa_stub_generator, interface);
          //     if(!r) std::cout << "Failed generating cpp_poa_stub_generator for cpp" << std::endl;
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
