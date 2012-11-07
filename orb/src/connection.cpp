/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/poa/connection.hpp>
#include <morbid/iiop/message_header.hpp>
#include <morbid/iiop/grammar/message_header.hpp>
#include <morbid/iiop/grammar/request_header_1_1.hpp>
#include <morbid/iiop/generator/reply_header.hpp>
#include <morbid/iiop/generator/message_header.hpp>

#include <boost/asio/ip/tcp.hpp>
#include <boost/bind.hpp>
#include <boost/utility.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>
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
    std::cout << "message header was parsed" << std::endl;
    unsigned char size_tmp = message_header.message_size;
    int size = size_tmp;
    std::cout << "message is size " << size << std::endl;
    if(std::distance(iterator, last) >= size)
    {
      std::cout << "message was completely read" << std::endl;
      std::vector<char>::const_iterator begin = processing_buffer.begin();
      std::vector<char>::iterator last
        = boost::next(processing_buffer.begin()
                      , std::distance(begin, iterator) + size);
      bool success = false;
      switch(message_header.message_type)
      {
      case 0: // Request
        std::cout << "message is a request" << std::endl;
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

  std::vector<char>::const_iterator first_first = first;

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
    if(boost::distance(request_header.operation) != 0
       && *boost::prior(request_header.operation.end()) == 0)
    {
      if(boost::shared_ptr<POA> poa = poa_.lock())
      {
        std::cout << "POA still alive" << std::endl;
        std::vector<char>::const_iterator header_first
          = request_header.object_key.begin()
          , header_last = request_header.object_key.end();
        std::string poa_name;
        std::size_t impl_;
        std::cout << "== " << std::distance(first, last) << std::endl;
        if(qi::parse(header_first, header_last, +(qi::char_ - qi::char_('/'))
                     >> qi::omit[qi::char_] >> qi::uint_parser<std::size_t, 16u>()
                     , poa_name, impl_))
        {
          std::cout << "POA name " << poa_name << std::endl;
          ServantBase* impl = 0;
          std::memcpy(&impl, &impl_, sizeof(impl));
          std::cout << "ServantBase pointer " << impl << std::endl;
          if(poa->object_map.find(impl) != poa->object_map.end())
          {
            std::cout << "Found servant " << std::distance(first, last) << std::endl;
            try
            {
              reply r = {request_header.request_id};
              impl->_dispatch(&*request_header.operation.begin(), &*first_first, &*first
                              , &*first + std::distance(first, last)
                              , little_endian, r);

              if(request_header.response_expected)
              {
                namespace karma = boost::spirit::karma;
                std::cout << "Reply body size: " << r.reply_body.size() << std::endl;
                std::vector<char> reply_buffer(12 + 12);
                reply_buffer.insert(reply_buffer.end(), r.reply_body.begin()
                                    , r.reply_body.end());
                std::vector<char>::iterator iterator = reply_buffer.begin();
                iiop::generator::message_header<std::vector<char>::iterator> message_header_grammar;
                if(karma::generate(iterator, message_header_grammar(little_endian, 1, reply_buffer.size())))
                {
                  std::cout << "generated message header" << std::endl;
                  iiop::generator::reply_header<std::vector<char>::iterator> reply_header_grammar;
                  if(karma::generate(iterator, reply_header_grammar(little_endian, request_header.request_id)))
                  {
                    std::cout << "generated reply header grammar" << std::endl;
                    boost::system::error_code ec;
                    std::cout << "reply_buffer " << reply_buffer.size() << std::endl;
                    int rs =
                    boost::asio::write(socket, boost::asio::buffer(reply_buffer)
                                       , boost::asio::transfer_all(), ec);
                    int size = reply_buffer.size();
                    assert(rs == size);
                    if(!ec)
                    {
                      std::cout << "Successful transfer" << std::endl;
                      return true;
                    }
                    else
                    {
                      std::cout << "Failed transfering" << std::endl;
                    }
                  }
                }
              }
            }
            catch(std::runtime_error const& e)
            {
              std::cout << "Error running dispatch " << e.what() << " name: " << typeid(e).name() << std::endl;
            }
          }
          else
          {
            std::cout << "Servant not found, should throw OBJECT_NOT_EXIST" << std::endl;
          }
        }
      }
    }
  }
  else
    std::cout << "Failed parsing service_context_list" << std::endl;

  return false;
}

} }
