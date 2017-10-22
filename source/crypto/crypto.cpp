// Copyright (c) 2017 N Subiron Montoro. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#include "crypto/crypto.h"

#include <stdexcept>

#include <openssl/evp.h>

namespace crypto {

  template <typename D, size_t SIZE>
  static void do_digest(const_buffer_view buffer, std::array<byte, SIZE> &digest, D &&alg) {
    if (static_cast<int>(digest.size()) != EVP_MD_size(alg)) {
      throw std::runtime_error("invalid digest size");
    }

    EVP_MD_CTX *mdctx = EVP_MD_CTX_create();
    if (mdctx == nullptr) {
      throw std::runtime_error("openssl failed to create digest context");
    }

    if (1 != EVP_DigestInit_ex(mdctx, alg, nullptr)) {
      throw std::runtime_error("error generating digest sha256");
    }

    if (1 != EVP_DigestUpdate(mdctx, buffer.data(), buffer.size())) {
      throw std::runtime_error("error generating digest sha256");
    }

    auto lenght = 0u;
    if(1 != EVP_DigestFinal_ex(mdctx, digest.data(), &lenght)) {
      throw std::runtime_error("error generating digest sha256");
    }

    EVP_MD_CTX_destroy(mdctx);
  }

  void digest(const_buffer_view buffer, sha256_digest &digest) {
    ::crypto::do_digest(buffer, digest, EVP_sha256());
  }

  void digest(const_buffer_view buffer, sha512_digest &digest) {
    ::crypto::do_digest(buffer, digest, EVP_sha512());
  }

  void zeroize(mutable_buffer_view buffer) {
    OPENSSL_cleanse(buffer.data(), buffer.size());
  }

} // namespace crypto
