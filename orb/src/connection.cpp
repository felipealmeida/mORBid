/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <tecorb/poa/connection.hpp>
#include <tecorb/iiop/message_header.hpp>
#include <tecorb/iiop/grammar/message_header.hpp>
#include <tecorb/iiop/grammar/request_header_1_1.hpp>

#include <boost/asio/ip/tcp.hpp>
#include <boost/bind.hpp>
#include <boost/utility.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

#include <iomanip>

namespace std {

std::ostream& operator<<(std::ostream& os, std::vector<char>::const_iterator it)
{
  return os << "[std::vector<char>::const_iterator pointing " << (unsigned int)(unsigned char)*it << "]";
}

}

namespace tecorb { namespace poa {

void connection::start()
{
  socket.async_read_some(boost::asio::mutable_buffers_1(&read_buffer[0], read_buffer.size())
                         , boost::bind(&connection::handle_read, shared_from_this()
                                       , _1, _2));
}

void connection::handle_read(boost::system::error_code const& ec
                             , std::size_t bytes_read)
{
  if(!ec)
  {
    std::cout << "bytes read: " << bytes_read << std::endl;

    processing_buffer.insert(processing_buffer.end()
                             , read_buffer.begin()
                             , boost::next(read_buffer.begin(), bytes_read));
    process_input();
  }
  else
    std::cout << "Error " << ec << std::endl;
}

void connection::process_input()
{
  std::cout << "Should try to understand what is in processing_buffer: " << processing_buffer.size() << std::endl;

  std::vector<char>::const_iterator iterator = processing_buffer.begin()
    , last = processing_buffer.end();
  while(iterator != last)
  {
    std::cout.fill('0');
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::endl(std::cout);
  }
  std::endl(std::cout);
  std::dec(std::cout);

  namespace qi = boost::spirit::qi;
  iterator = processing_buffer.begin();

  iiop::message_header message_header;
  iiop::grammar::message_header<std::vector<char>::const_iterator> message_header_grammar;

  if(qi::parse(iterator, last, message_header_grammar, message_header))
  {
    unsigned char size_tmp = message_header.message_size;
    std::size_t size = size_tmp;
    if(std::distance(iterator, last) >= size)
    {
      std::vector<char>::const_iterator begin = processing_buffer.begin();
      std::vector<char>::iterator last
        = boost::next(processing_buffer.begin()
                      , std::distance(begin, iterator) + size);
      bool success = false;
      switch(message_header.message_type)
      {
      case 0: // Request
        success = handle_request(iterator, last, (message_header.flags & 1) == 1);
        break;
      case 1: // Reply
        break;
      case 2: // CancelRequest
        break;
      case 3: // LocateRequest
        break;
      case 4: // LocateReply
        break;
      case 5: // CloseConnection
        break;
      case 6: // MessageError
        break;
      case 7: // Fragment
        break;
      default:
        std::cout << "Unknown message_type" << std::endl;
        return;
      }
      if(success)
        processing_buffer.erase(processing_buffer.begin(), last);
    }    
  }
  else if(processing_buffer.size() >= 4)
  {
    std::cout << "No Magic word!" << std::endl;
    return;
  }
  start();
}

bool connection::handle_request(std::vector<char>::const_iterator first
                                , std::vector<char>::const_iterator last
                                , bool little_endian)
{
  std::cout << "Handle request for " << std::distance(first, last) << " bytes" << std::endl;
  typedef std::vector<char>::const_iterator iterator;
  namespace qi = boost::spirit::qi;
  namespace phoenix = boost::phoenix;

  iiop::request_header request_header;
  iiop::grammar::request_header_1_1<iterator> request_header_grammar;

  if(qi::parse(first, last, request_header_grammar(phoenix::val(first), little_endian), request_header))
  {
    std::cout << "Parsed request header correctly" << std::endl;
    std::cout << "Request id: " << request_header.request_id << std::endl;
    std::cout << "Response expected: " << request_header.response_expected << std::endl;
    std::cout << "Object key size: " << request_header.object_key.size() << std::endl;
    std::cout << "Object key |";
    std::copy(request_header.object_key.begin(), request_header.object_key.end()
              , std::ostream_iterator<char>(std::cout));
    std::cout << "|" << std::endl;
    std::cout << "operation |";
    std::copy(request_header.operation.begin(), request_header.operation.end()
              , std::ostream_iterator<char>(std::cout));
    std::cout << "|" << std::endl;
    
    if(boost::shared_ptr<POA> poa = poa_.lock())
    {
      std::cout << "POA still alive" << std::endl;
      std::vector<char>::const_iterator first
        = request_header.object_key.begin()
        , last = request_header.object_key.end();
      std::string poa_name;
      std::size_t impl_;
      if(qi::parse(first, last, +(qi::char_ - qi::char_('/'))
                   >> qi::omit[qi::char_] >> qi::uint_parser<std::size_t, 16u>()
                   , poa_name, impl_))
      {
        std::cout << "POA name " << poa_name << std::endl;
        ServantBase* impl = 0;
        std::memcpy(&impl, &impl_, sizeof(impl));
        std::cout << "ServantBase pointer " << impl << std::endl;
        if(poa->object_map.find(impl) != poa->object_map.end())
        {
          std::cout << "Found servant" << std::endl;
        }
        else
        {
          std::cout << "Servant not found, should throw OBJECT_NOT_EXIST" << std::endl;
        }
      }
    }
  }
  else
    std::cout << "Failed parsing service_context_list" << std::endl;

  return false;
}

} }
