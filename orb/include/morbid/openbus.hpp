/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_OPENBUS_HPP
#define MORBID_OPENBUS_HPP

#include <morbid/orb.hpp>
#include <morbid/object.hpp>
#include <morbid/reference_cast.hpp>

#include <openssl/rsa.h>
#include <openssl/pem.h>

namespace morbid {

orb::orb(openbus_type, std::string const& host, unsigned short port
         , std::string const& username, std::string const& password)
  : impl(new orb_impl)
{
  structured_ior sior = {"IDL:scs/core/IComponent:1.0"};
  const char openbus_key[] = "OpenBus_2_0";
  std::vector<char> ob(openbus_key, openbus_key + sizeof(openbus_key)-1);
  iiop::profile_body pb(host, port, ob);
  sior.structured_profiles.push_back(pb);
  ::scs::core::IComponent_ref openbus_component_ref(*this, sior);
  object obj = openbus_component_ref.getFacet("IDL:tecgraf/openbus/core/v2_0/services/access_control/AccessControl:1.0");
  namespace access_control = ::tecgraf::openbus::core::v2_0::services::access_control;
  access_control::AccessControl acs = reference_cast<access_control::AccessControl>(obj, *this);

  std::vector<unsigned char> buskey_args = acs._get_buskey();
  std::cout << "buskey_args: " << buskey_args.size() << std::endl;

  EVP_PKEY* bus_key = 0;
  {
    unsigned char const* buf = &buskey_args[0];
    bus_key = d2i_PUBKEY(0, &buf, buskey_args.size());
    assert(bus_key != 0);
  }

  EVP_PKEY* key = 0;
  {
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, 0);
    int r = EVP_PKEY_keygen_init(ctx);
    assert(r == 1);
    r = EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048);
    assert(r == 1);
    r = EVP_PKEY_keygen(ctx, &key);
    assert((r == 1) && key);
  }

  boost::array<unsigned char, 32> public_key_hash;
  std::vector<unsigned char> public_key_buffer;

  {
    unsigned char* key_buffer = 0; // leaks
    std::size_t len = i2d_PUBKEY(key, &key_buffer);
    SHA256(key_buffer, len, &public_key_hash[0]);
    public_key_buffer.insert(public_key_buffer.end(), key_buffer, key_buffer + len);
  }

  boost::array<unsigned char, 256> encrypted_block;
  std::memset(&encrypted_block[0], 0, encrypted_block.size());
  {
    std::vector<unsigned char> password_vector(password.begin(), password.end());
    std::vector<unsigned char> block;
    typedef giop::forward_back_insert_iterator<std::vector<unsigned char> > output_iterator_type;
    output_iterator_type iterator(block);
    bool g = karma::generate(iterator, giop::compile<iiop::generator_domain>
                             (giop::endianness(giop::native_endian)
                              [+giop::octet & giop::sequence[giop::octet] ]
                             )
                             , fusion::make_vector(public_key_hash, password_vector));

    assert(!!g);

    std::cout << "block " << block.size() << std::endl;
      
    {
      EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(bus_key, 0);
      assert(!!ctx);
      int r = EVP_PKEY_encrypt_init(ctx);
      assert(r == 1);
      std::size_t encrypted_size = 0;
      r = EVP_PKEY_encrypt(ctx, 0, &encrypted_size, &block[0], block.size());
      assert(r == 1);
      assert(encrypted_size == encrypted_block.size());
      assert(encrypted_block.size() == encrypted_size);
      r = EVP_PKEY_encrypt(ctx, &encrypted_block[0], &encrypted_size
                           , &block[0], block.size());
      assert(r == 1);

      std::cout << "Encrypted Block size: " << encrypted_block.size() << std::endl;
    }
  }

  morbid::unsigned_long validity_time = 0;
  access_control::LoginInfo li = acs.loginByPassword(username, public_key_buffer, encrypted_block, validity_time);
  std::cout << "li.id: " << li.id << " li.entity: " << li.entity << " validity_time: " << validity_time << std::endl;
}

}

#endif
