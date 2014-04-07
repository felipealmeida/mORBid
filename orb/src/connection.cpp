/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/poa/connection.hpp>
#include <morbid/giop/grammars/message_1_0.hpp>
#include <morbid/giop/grammars/request_1_0.hpp>
#include <morbid/iiop/all.hpp>
#include <morbid/ior/hex_directive.hpp>

#include <boost/asio/ip/tcp.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/utility.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/phoenix.hpp>
#include <boost/phoenix/fusion/at.hpp>
#include <boost/asio.hpp>

#include <iomanip>

namespace std {

std::ostream& operator<<(std::ostream& os, std::vector<char>::const_iterator it)
{
  return os << "[std::vector<char>::const_iterator pointing " << (unsigned int)(unsigned char)*it << "]";
}

}

namespace morbid { namespace poa {

void connection::start()
{
  std::cout << "connection::start " << read_buffer.size() << std::endl;
  socket.async_read_some(boost::asio::mutable_buffers_1(&read_buffer[0], read_buffer.size())
                         , boost::bind(&connection::handle_read, shared_from_this()
                                       , _1, _2));
}

void connection::close()
{
  socket.close();
}

void connection::handle_read(boost::system::error_code ec, std::size_t bytes_read)
{
  std::cout << "handle_read " << bytes_read << std::endl;
  boost::system::error_code no_error;
  if(!ec)
  {
    std::cout << "bytes read: " << bytes_read << std::endl;

    processing_buffer.insert(processing_buffer.end()
                             , read_buffer.begin()
                             , boost::next(read_buffer.begin(), bytes_read));
    process_input();
  }
  else
  {
    assert(ec != no_error);
    std::cout << "Error " << ec.message() << std::endl;
  }
}

void connection::process_input()
{
  std::cout << "Should try to understand what is in processing_buffer: " << processing_buffer.size() << std::endl;

  typedef std::vector<char>::iterator iterator_type;
  iterator_type iterator = processing_buffer.begin(), last = processing_buffer.end();
  // while(iterator != last)
  // {
  //   std::cout.fill('0');
  //   std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
  //   if(++iterator == last) break;
  //   std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
  //   if(++iterator == last) break;
  //   std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
  //   if(++iterator == last) break;
  //   std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
  //   if(++iterator == last) break;
  //   std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
  //   if(++iterator == last) break;
  //   std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
  //   if(++iterator == last) break;
  //   std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
  //   if(++iterator == last) break;
  //   std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
  //   if(++iterator == last) break;
  //   std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
  //   if(++iterator == last) break;
  //   std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
  //   if(++iterator == last) break;
  //   std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
  //   if(++iterator == last) break;
  //   std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
  //   if(++iterator == last) break;
  //   std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
  //   if(++iterator == last) break;
  //   std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
  //   if(++iterator == last) break;
  //   std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
  //   if(++iterator == last) break;
  //   std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
  //   if(++iterator == last) break;
  //   std::endl(std::cout);
  // }
  // std::endl(std::cout);
  // std::dec(std::cout);

  namespace qi = boost::spirit::qi;
  namespace fusion = boost::fusion;
  iterator = processing_buffer.begin();

  typedef std::vector<fusion::vector2<unsigned int, std::vector<char> > > service_context_list;
  typedef fusion::vector8<service_context_list
                          , unsigned int, bool
                          , std::vector<char>, std::string, std::vector<char>
                          , std::vector<char>, bool>
    request_attribute_type;
  typedef fusion::vector1<fusion::vector1<request_attribute_type> >
    message_request_attribute_type;

  typedef giop::grammars::message_1_0<iiop::parser_domain
                                      , iterator_type, message_request_attribute_type
                                      , 0u /* request */>
    message_request_grammar;
  typedef giop::grammars::request_1_0<iiop::parser_domain
                                      , iterator_type, request_attribute_type>
    request_grammar;
  
  request_grammar request_grammar_(iiop::buffer & iiop::save_endian);
  message_request_grammar message_request_grammar_(request_grammar_);

  message_request_attribute_type message_request_attribute;
  if(qi::parse(iterator, last
               , giop::compile<iiop::parser_domain>(message_request_grammar_)
               , message_request_attribute))
  {
    request_attribute_type& attr = fusion::at_c<0u>(fusion::at_c<0>(message_request_attribute));
    // std::cout << "message header was parsed and it is a request" << std::endl;
    std::cout << "request id " << fusion::at_c<1u>(attr) << std::endl;
    std::cout << "expects response " << fusion::at_c<2u>(attr) << std::endl;
    std::cout << "object key " << boost::make_iterator_range
      (fusion::at_c<3u>(attr).begin(), fusion::at_c<3u>(attr).end()) << std::endl;
    std::cout << "method " << fusion::at_c<4u>(attr) << std::endl;

    std::cout << "arguments buffer size: " << fusion::at_c<6u>(attr).size() << std::endl;
    std::cout << "endianness: " << fusion::at_c<7u>(attr) << std::endl;
    std::cout << "arguments offset: " << std::distance(processing_buffer.begin(), iterator) << std::endl;
    std::size_t align_offset = std::distance(processing_buffer.begin(), iterator) - fusion::at_c<6u>(attr).size();
    processing_buffer.erase(processing_buffer.begin(), iterator);

    std::vector<char>& object_key = fusion::at_c<3u>(attr);
    // std::cout << "POA still alive" << std::endl;
    std::size_t id;
    assert(object_key.size() == sizeof(std::size_t));
    std::memcpy(&id, &object_key[0], object_key.size());

    if(boost::shared_ptr<orb_impl> orb = orb_.lock())
    {
      std::cout << "Searching object " << id << std::endl;
      orb_impl::object_map_iterator iterator = orb->object_map.find(id);
      if(iterator != orb->object_map.end())
      {
        std::cout << "Found object" << std::endl;

        std::vector<char>& arguments = fusion::at_c<6u>(attr);
        const char* arg_first = 0, *arg_last = 0;
        if(!arguments.empty())
        {
          arg_first = &arguments[0];
          arg_last = arg_first + arguments.size();
        }

        reply r = {fusion::at_c<1u>(attr)};
        iterator->second.obj.call(fusion::at_c<4u>(attr), align_offset
                                  , arg_first, arg_last, fusion::at_c<7u>(attr), r);
        assert(arg_first == arg_last);
        assert(r.reply_body.size() != 0);
        boost::system::error_code ec;
        int rs = boost::asio::write(socket, boost::asio::buffer(r.reply_body)
                                    , boost::asio::transfer_all(), ec);
        int size = r.reply_body.size();
        assert(rs == size);
        if(!ec)
        {
          std::cout << "Successful transfer ";
          boost::algorithm::hex(r.reply_body.begin(), r.reply_body.end()
                                , std::ostream_iterator<char>(std::cout));
          std::endl(std::cout);
          start();
          return;
        }
        else
        {
          std::cout << "Failed transfering " << std::endl;
          boost::algorithm::hex(r.reply_body.begin(), r.reply_body.end()
                                , std::ostream_iterator<char>(std::cout));
          std::endl(std::cout);
        }
      }
    }
    std::cout << "Closed connection" << std::endl;
    close();
    return;
  }
  else
  {
    std::cout << "Failed parsing for request" << std::endl;
  }
  start();
}

} }
