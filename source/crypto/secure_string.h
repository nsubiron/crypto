// Copyright (c) 2017 N Subiron Montoro. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "crypto/crypto.h"

#include <cstring>
#include <memory>
#include <string>

namespace crypto {

  /// A super basic secure string. Its contents are zeroized on destruction. It
  /// cannot be resized.
  ///
  /// Cannot be constructed directly, use @a clean_buffer_and_make or
  /// @a unsafe_make.
  class secure_string {
  public:

    using value_type = char;
    using size_type = std::size_t;

    /// Creates a secure_string and zeroizes the given buffer.
    static secure_string clean_buffer_and_make(mutable_buffer_view buffer) {
      auto the_new_string = secure_string(buffer);
      ::crypto::zeroize(buffer);
      return the_new_string;
    }

#if __cplusplus > 201703L // C++17
    /// Creates a secure_string and zeroizes the given string.
    static secure_string clean_buffer_and_make(std::string &string) {
      return clean_buffer_and_make(buffer_view::make_mutable(string));
    }
#endif

    /// Creates a secure_string but does not zeroize the given buffer.
    static secure_string unsafe_make(const_buffer_view buffer) {
      return secure_string(buffer);
    }

    /// Creates a secure_string but does not zeroize the given data.
    static secure_string unsafe_make(const char *data) {
      return secure_string(buffer_view::make_const(data));
    }

    /// Creates a secure_string but does not zeroize the given string.
    static secure_string unsafe_make(const std::string &string) {
      return secure_string(buffer_view::make_const(string));
    }

  private:

    explicit secure_string(const char *buffer, size_type length)
      : _length(length),
        _buffer(std::make_unique<char[]>(_length + 1u)) {
      std::memcpy(_buffer.get(), buffer, length);
      _buffer[_length] = '\0';
    }

    explicit secure_string(
        const char *buffer0, size_type length0,
        const char *buffer1, size_type length1)
      : _length(length0 + length1),
        _buffer(std::make_unique<char[]>(_length + 1u)) {
      std::memcpy(_buffer.get(), buffer0, length0);
      std::memcpy(_buffer.get() + length0, buffer1, length1);
      _buffer[_length] = '\0';
    }

    explicit secure_string(const_buffer_view buffer)
      : secure_string(reinterpret_cast<const char*>(buffer.data()), buffer.size()) {}

    explicit secure_string(const_buffer_view buffer0, const_buffer_view buffer1)
      : secure_string(
          reinterpret_cast<const char*>(buffer0.data()), buffer0.size(),
          reinterpret_cast<const char*>(buffer1.data()), buffer1.size()) {}

  public:

    secure_string(const secure_string &rhs)
      : secure_string(rhs.buffer()) {}

    secure_string(secure_string &&rhs)
      : _length(rhs._length),
        _buffer(std::move(rhs._buffer)) {
      rhs._length = 0u;
    }

    ~secure_string() {
      zeroize();
    }

    size_type size() const {
      return _length;
    }

    char *data() {
      return _buffer.get();
    }

    const char *data() const {
      return _buffer.get();
    }

    const char *c_str() const {
      return _buffer.get();
    }

    mutable_buffer_view buffer() {
      return buffer_view::make_mutable(data(), size());
    }

    const_buffer_view buffer() const {
      return buffer_view::make_const(data(), size());
    }

    void clear() {
      zeroize();
    }

    secure_string &operator=(secure_string rhs) {
      clear();
      _length = rhs._length;
      _buffer = std::move(rhs._buffer);
      return *this;
    }

    friend secure_string operator+(secure_string rhs, secure_string lhs) {
      return secure_string(rhs.buffer(), lhs.buffer());
    }

  private:

    void zeroize() {
      if (_buffer != nullptr) {
        ::crypto::zeroize(buffer());
        _length = 0u;
        _buffer = nullptr;
      }
    }

    size_type _length;

    std::unique_ptr<char[]> _buffer;
  };

} // namespace crypto
