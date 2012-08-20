/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <tecorb/idl_parser/tokenizer.hpp>
#include <tecorb/idl_parser/grammar/interface_def.hpp>
#include <tecorb/idl_compiler/generator/stub_generator.hpp>
#include <tecorb/idl_compiler/generator/local_stub_generator.hpp>
#include <tecorb/idl_compiler/generator/remote_stub_generator.hpp>
#include <tecorb/idl_compiler/generator/poa_stub_generator.hpp>

#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

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

        typedef boost::spirit::lex::lexertl::token<std::vector<char>::const_iterator> token_type;
        typedef boost::spirit::lex::lexertl::actor_lexer<token_type> lexer_type;
        
        tecorb::idl_parser::tokens<lexer_type> lexer;

        std::vector<char>::const_iterator buffer_begin = buffer.begin()
          , buffer_end = buffer.end();

        typedef lexer_type::iterator_type iterator_type;
         iterator_type iterator = lexer.begin(buffer_begin, buffer_end)
          , last = lexer.end();

        tecorb::idl_parser::grammar::interface_definition<iterator_type> grammar(lexer);
        tecorb::idl_parser::interface_def<iterator_type> interface;
        typedef tecorb::idl_parser::op_decl<iterator_type> op_decl;
        using tecorb::idl_parser::param_decl;
        tecorb::idl_parser::skipper<iterator_type> skipper(lexer);

        bool r = boost::spirit::qi::phrase_parse(iterator, last, grammar
                                                 , skipper, interface);
        if(r)
        {
          std::cout << "Parsed successfully" << std::endl;

          interface.repoids.push_back("IDL:omg.org/CORBA/Object:1.0");
          interface.repoids.push_back("IDL:" + interface.name + ":1.0");
          assert(interface.repoids.size() == 2);

          {
            op_decl is_a_op_decl = {"bool", "_is_a"};
            param_decl param = {"in", "string"};
            is_a_op_decl.params.push_back(param);
            is_a_op_decl.user_defined = false;
            interface.op_decls.push_back(is_a_op_decl);
          }

          std::cout << "Generating stubs for interface " << interface.name << std::endl;

          std::ofstream header("file.h");
          std::ofstream cpp("file.cpp");
          if(header.is_open() && cpp.is_open())
          {
            namespace karma = boost::spirit::karma;
            typedef std::ostream_iterator<char> output_iterator_type;
            {
              output_iterator_type iterator(header);

              tecorb::idl_compiler::header_stub_generator
                <output_iterator_type, iterator_type>
                header_stub_generator;
              tecorb::idl_compiler::header_local_stub_generator
                <output_iterator_type, iterator_type>
                header_local_stub_generator;
              tecorb::idl_compiler::header_poa_stub_generator
                <output_iterator_type, iterator_type>
                header_poa_stub_generator;

              bool r = karma::generate
                (iterator
                 ,  karma::lit("#include <tecorb/poa.hpp>") << karma::eol
                 << karma::lit("#include <tecorb/handle_request_body.hpp>") << karma::eol
                 << karma::lit("#include <tecorb/reply.hpp>") << karma::eol
                 << karma::eol
                 << "class POA_" << interface.name << ";" << karma::eol << karma::eol
                );
              if(!r) std::cout << "Failed generating #includes for header" << std::endl;

              r = karma::generate(iterator, header_stub_generator, interface);
              if(!r) std::cout << "Failed generating header_stub_generator" << std::endl;
              r = karma::generate(iterator, header_local_stub_generator, interface);
              if(!r) std::cout << "Failed generating header_local_stub_generator" << std::endl;
              r = karma::generate(iterator, header_poa_stub_generator, interface);
              if(!r) std::cout << "Failed generating header_poa_stub_generator" << std::endl;
            }
            {
              output_iterator_type iterator(cpp);
              tecorb::idl_compiler::cpp_stub_generator
                <output_iterator_type, iterator_type>
                cpp_stub_generator;
              tecorb::idl_compiler::cpp_local_stub_generator
                <output_iterator_type, iterator_type>
                cpp_local_stub_generator;
              tecorb::idl_compiler::header_remote_stub_generator
                <output_iterator_type, iterator_type>
                header_remote_stub_generator;
              tecorb::idl_compiler::cpp_remote_stub_generator
                <output_iterator_type, iterator_type>
                cpp_remote_stub_generator;
              tecorb::idl_compiler::cpp_poa_stub_generator
                <output_iterator_type, iterator_type>
                cpp_poa_stub_generator;

              karma::generate
                (iterator
                 , karma::lit("#include \"file.h\"") << karma::eol
                 << karma::eol
                );

              bool r;
              r = karma::generate(iterator, header_remote_stub_generator, interface);
              if(!r) std::cout << "Failed generating header_remote_stub_generator for cpp" << std::endl;
              r = karma::generate(iterator, cpp_remote_stub_generator, interface);
              if(!r) std::cout << "Failed generating cpp_remote_stub_generator for cpp" << std::endl;
              r = karma::generate(iterator, cpp_stub_generator, interface);
              if(!r) std::cout << "Failed generating cpp_stub_generator for cpp" << std::endl;
              r = karma::generate(iterator, cpp_local_stub_generator, interface);
              if(!r) std::cout << "Failed generating cpp_local_stub_generator for cpp" << std::endl;
              assert(interface.repoids.size() == 2);
              r = karma::generate(iterator, cpp_poa_stub_generator, interface);
              if(!r) std::cout << "Failed generating cpp_poa_stub_generator for cpp" << std::endl;
            }
          }
          else
          {
            std::cout << "Couldn't open output file" << std::endl;
          }
        }
        else
        {
          std::cout << "Failed parsing" << std::endl;
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
