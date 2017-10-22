// Copyright (c) 2017 N Subiron Montoro. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "crypto/crypto.h"
#include "crypto/output.h"
#include "crypto/secure_string.h"

#include <memory>

namespace crypto {

  /// A password digest object to store digested passwords. Allows comparison
  /// with other password digests and clear passwords.
  ///
  /// This object can be copied around without copying the holding data. A
  /// single const copy of the data is shared among all the copies of the
  /// password_digest using shared_ptr.
  template <typename D = sha256_digest>
  class password_digest {
  public:

    using digest_type = D;

    explicit password_digest(const secure_string &password)
      : _digest([&](){
        auto ptr = std::make_shared<digest_type>();
        ::crypto::digest(password.buffer(), *ptr);
        return ptr;
      }()) {}

    password_digest(const password_digest &) = default;
    password_digest(password_digest &&) = default;
    password_digest &operator=(const password_digest &) = default;
    password_digest &operator=(password_digest &&) = default;

    bool operator==(const password_digest &rhs) const {
      return (_digest == rhs._digest) || (*_digest == *rhs._digest);
    }

    bool operator!=(const password_digest &rhs) const {
      return !(*this == rhs);
    }

    bool operator==(const secure_string &password) const {
      return (*this) == password_digest(password);
    }

    bool operator!=(const secure_string &password) const {
      return !(*this == password);
    }

    constexpr size_t size() const {
      return _digest->size();
    }

    std::string to_hex_string(size_t count) const {
      auto buffer = buffer_view::make_const(_digest->data(), std::min(count, size()));
      return ::crypto::to_hex_string(buffer);
    }

    std::string to_hex_string() const {
      return to_hex_string(size());
    }

  private:

    std::shared_ptr<const digest_type> _digest;
  };

} // namespace crypto
