/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include <morbid/idl_compiler/parse_functions.hpp>

#include <morbid/idl_compiler/generate_functions.hpp>

#include <morbid/idl_compiler/generate_header_appendage.hpp>
#include <morbid/idl_compiler/module.hpp>
#include <morbid/idl_compiler/errors.hpp>
#include <morbid/idl_compiler/generate_header_modules_visitor.hpp>

#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

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

        namespace idlc = morbid::idlc;

        idlc::context_type ctx(buffer.begin(), buffer.end(), input_file.string().c_str());
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
        }

        idlc::parser_iterator_type iterator = ctx.begin(), last = ctx.end();

        boost::filesystem::path header_path;
        if(vm.count("output"))
        {
          header_path = vm["output"].as<std::string>() + ".hpp";
        }
        else
        {
          header_path = input_file;
          header_path.replace_extension(".hpp");
        }

        boost::filesystem::ofstream header(header_path);
        if(!header.is_open())
        {
          std::cout << "Couldn't open output file" << std::endl;
          return -1;
        }
        
        idlc::output_iterator output_iterator(header);

        idlc::generate_header_prefix(output_iterator);

        typedef morbid::idlc::module module_type;
        using morbid::idlc::module_property_type;
        module_property_type module_property;
        idlc::modules_tree_type modules_tree;

        using morbid::idlc::vertex_descriptor;
        module_property_type p1(boost::shared_ptr<module_type>(new module_type("")));
        vertex_descriptor global_module = add_vertex
          (p1, modules_tree);
        module_property_type p2(boost::shared_ptr<module_type>(new module_type("::morbid")));
        vertex_descriptor primitive_types_module = add_vertex
          (p2, modules_tree);

        std::vector<vertex_descriptor> current_module;
        current_module.push_back(primitive_types_module);
        current_module.push_back(global_module);

        // bool r = karma::generate
        //   (output_iterator
        //    , karma::lit("// -*- c++ -*-") << karma::eol
        //    << "// Generated header. DO NOT EDIT" << karma::eol << karma::eol
        //    << karma::lit("#include <morbid/handle_request_body.hpp>") << karma::eol
        //    << karma::lit("#include <morbid/reply.hpp>") << karma::eol
        //    << karma::lit("#include <morbid/structured_ior.hpp>") << karma::eol
        //    << karma::lit("#include <morbid/in_out_traits.hpp>") << karma::eol << karma::eol
        //    << karma::lit("#include <morbid/synchronous_call.hpp>") << karma::eol << karma::eol
        //    << karma::lit("#include <morbid/orb.hpp>") << karma::eol << karma::eol
        //    << karma::lit("#include <morbid/reference.hpp>") << karma::eol << karma::eol
        //    << karma::lit("#include <morbid/object.hpp>") << karma::eol << karma::eol
        //    << karma::lit("#include <morbid/lazy_eval.hpp>") << karma::eol << karma::eol
        //    << karma::lit("#include <boost/integer.hpp>") << karma::eol
        //    << karma::lit("#include <boost/spirit/home/karma.hpp>") << karma::eol
        //    << karma::lit("#include <boost/fusion/include/vector10.hpp>") << karma::eol
        //    << karma::lit("#include <boost/fusion/include/vector20.hpp>") << karma::eol
        //    << karma::lit("#include <boost/fusion/include/vector30.hpp>") << karma::eol
        //    << karma::lit("#include <boost/fusion/include/vector40.hpp>") << karma::eol
        //    << karma::lit("#include <boost/fusion/include/vector50.hpp>") << karma::eol
        //    << karma::lit("#include <boost/type_erasure/any.hpp>") << karma::eol
        //    << karma::lit("#include <boost/type_erasure/member.hpp>") << karma::eol
        //    << karma::lit("#include <boost/type_erasure/concept_interface.hpp>") << karma::eol
        //    << karma::eol
        //    );
        // if(!r) 
        //   throw std::runtime_error("Failed generating #includes for header");
        
        idlc::module_map map = get(idlc::module_property_t(), modules_tree);
        do
        {
          try
          {
            morbid::idl_parser::wave_string module_open;
            morbid::idl_parser::interface_def interface;
            morbid::idl_parser::typedef_def typedef_;
            morbid::idl_parser::constant constant;
            morbid::idl_parser::exception_def exception;
            morbid::idl_parser::struct_def struct_;
            
            // Checking for many different paterns
            if(idlc::parse_module(iterator, last, module_open))
            {
              std::cout << "Opened module " << module_open << std::endl;
              idlc::generate_module(output_iterator, module_open, current_module, modules_tree, map);
            }
            else if(idlc::parse_module_close(iterator, last))
            {
              std::cout << "Closing module " << current_module.back() << std::endl;
              idlc::generate_module_close(output_iterator, current_module);
            }
            else if(idlc::parse_typedef(iterator, last, typedef_))
            {
              std::cout << "typedef " << typedef_ << std::endl;
              idlc::generate_typedef(output_iterator, typedef_, current_module, modules_tree);
            }
            else if(idlc::parse_exception(iterator, last, exception))
            {
              std::cout << "exception " << exception << std::endl;
            }
            else if(idlc::parse_struct(iterator, last, struct_))
            {
              std::cout << "struct " << struct_ << std::endl;
              idlc::generate_struct(output_iterator, struct_, current_module, modules_tree);
            }
            else if(idlc::parse_constant(iterator, last, constant))
            {
              std::cout << "Constant name " << constant.name << " = " << constant.value << std::endl;
            }
            else if(idlc::parse_interface(iterator, last, interface))
            {
              std::cout << "interface " << interface << std::endl;
              idlc::generate_interface(output_iterator, interface, current_module, modules_tree);
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
          catch(idlc::compilation_error const& e)
          {
            std::cout << e.file_position().get_file()
                      << ":" << e.file_position().get_line() 
                      << ":" << e.file_position().get_column()
                      << ": " << e.error_message()
                      << ". Error compiling"
                      << std::endl;
            return 1;
          }
        }
        while(!idlc::parse_end_of_file(iterator, last));

        std::cout << "Finished parsing file" << std::endl;
        if(current_module.size() != 2)
          throw std::runtime_error("Error: Not closed all modules");
        
        typedef std::map<vertex_descriptor, boost::default_color_type> color_map_container_t;
        typedef boost::associative_property_map<color_map_container_t> color_map_t;

        color_map_container_t color_map_container;
        color_map_t color_map(color_map_container);

        idlc::generate_header_modules_visitor
          header_modules_visitor(output_iterator);
        depth_first_visit(modules_tree, global_module, header_modules_visitor
                          , color_map);

        color_map_container.clear();
        idlc::generate_reference_model_visitor
          generate_reference_model_visitor(output_iterator);
        depth_first_visit(modules_tree, global_module, generate_reference_model_visitor
                          , color_map);
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
