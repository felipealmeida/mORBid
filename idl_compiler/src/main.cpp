/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <tecorb/idl_parser/tokenizer.hpp>
#include <tecorb/idl_parser/grammar/interface_def.hpp>

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

        tecorb::idl_parser::grammar::interface_definition<iterator_type> grammar;
        tecorb::idl_parser::interface_def<iterator_type> interface;
        typedef tecorb::idl_parser::op_decl<iterator_type> op_decl;

        bool r = boost::spirit::qi::phrase_parse(iterator, last, grammar
                                                 , tecorb::idl_parser::skipper<iterator_type>()
                                                 , interface);
        if(r)
        {
          std::cout << "Parsed successfully" << std::endl;

          std::cout << "Generating stubs for interface " << interface.name << std::endl;

          std::ofstream ofs("file.h");
          if(ofs.is_open())
          {
            ofs << "#ifndef SOMEGUARD\n#define SOMEGUARD\n\n";

            ofs << "#include <tecorb/object.hpp>\n";
            ofs << "#include <tecorb/narrow.hpp>\n";

            {
              ofs << "\nclass " << interface.name << " : public ::tecorb::narrow<"
                  << interface.name
                  << ", ::boost::mpl::vector1< ::tecorb::Object> >"
                  << "\n{\npublic:\n";

              for(std::vector<op_decl>::const_iterator first = interface.op_decls.begin()
                    , last = interface.op_decls.end(); first != last; ++first)
              {
                ofs << first->type << " " << first->name << "();\n";
              }

              ofs << "\n};\n";
              ofs << "typedef ::boost::shared_ptr<" << interface.name << "> "
                  << interface.name << "_ptr;\n";
              ofs << "typedef ::tecorb::var<" << interface.name << "> "
                  << interface.name << "_var;\n\n";
            }

            {
              ofs << "\nclass POA_" << interface.name << " : public ::tecorb::narrow<"
                  << interface.name
                  << ", ::boost::mpl::vector1< ::tecorb::poa::ServantBase> >"
                  << "\n{\npublic:\n";

              for(std::vector<op_decl>::const_iterator first = interface.op_decls.begin()
                    , last = interface.op_decls.end(); first != last; ++first)
              {
                ofs << "virtual " << first->type << " " << first->name << "() = 0;\n";
              }

              ofs << "\n};\n";
            }


            ofs << "\n#endif\n";
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
