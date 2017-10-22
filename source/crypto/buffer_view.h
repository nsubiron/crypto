// Copyright (c) 2017 N Subiron Montoro. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "crypto/array_view.h"

#include <cstring>
#include <string>

namespace crypto {

  using byte = unsigned char;

  /// A view over a C-style mutable buffer. Encapsulates the array and its size,
  /// but does NOT own the data.
  class mutable_buffer_view : public mutable_array_view<byte> {
  public:

    mutable_buffer_view(mutable_buffer_view &rhs)
      : mutable_array_view<byte>(rhs) {}

    mutable_buffer_view(const mutable_buffer_view &rhs)
      : mutable_array_view<byte>(rhs) {}

    mutable_buffer_view(byte *data, size_type size)
      : mutable_array_view<byte>(data, size) {}

    mutable_buffer_view(void *data, size_type size)
      : mutable_buffer_view(reinterpret_cast<byte *>(data), size) {}

    mutable_buffer_view(char *data, size_type size)
      : mutable_buffer_view(reinterpret_cast<byte *>(data), size) {}

    mutable_buffer_view(char *data)
      : mutable_buffer_view(reinterpret_cast<byte *>(data), std::strlen(data)) {}

#if __cplusplus > 201703L // C++17
    mutable_buffer_view(std::string &str)
      : mutable_buffer_view(str.data(), str.size()) {}
#endif

    template <typename T>
    mutable_buffer_view(T &container)
      : mutable_buffer_view(
          reinterpret_cast<byte *>(container.data()),
          sizeof(typename T::value_type) * container.size()) {
      static_assert(
          detail::is_contiguous_container<T>::value,
          "can only make buffer_view from contiguous container");
    }
  };


  /// A view over a C-style const buffer. Encapsulates the array and its size,
  /// but does NOT own the data.
  class const_buffer_view : public const_array_view<byte> {
  public:

    const_buffer_view(const const_buffer_view &rhs)
      : const_array_view<byte>(rhs) {}

    const_buffer_view(mutable_buffer_view rhs)
      : const_array_view<byte>(rhs) {}

    const_buffer_view(const byte *data, size_type size)
      : const_array_view<byte>(data, size) {}

    const_buffer_view(const void *data, size_type size)
      : const_buffer_view(reinterpret_cast<const byte *>(data), size) {}

    const_buffer_view(const char *data, size_type size)
      : const_buffer_view(reinterpret_cast<const byte *>(data), size) {}

    const_buffer_view(const char *data)
      : const_buffer_view(reinterpret_cast<const byte *>(data), std::strlen(data)) {}

    const_buffer_view(const std::string &str)
      : const_buffer_view(str.data(), str.size()) {}

    template <typename T>
    const_buffer_view(const T &container)
      : const_buffer_view(
          reinterpret_cast<const byte *>(container.data()),
          sizeof(typename T::value_type) * container.size()) {
      static_assert(
          detail::is_contiguous_container<T>::value,
          "can only make buffer_view from contiguous container");
    }
  };

namespace buffer_view {

  template <typename ... ARGS>
  static inline auto make_mutable(ARGS&& ... args) {
    return mutable_buffer_view(std::forward<ARGS>(args)...);
  }

  template <typename ... ARGS>
  static inline auto make_const(ARGS&& ... args) {
    return const_buffer_view(std::forward<ARGS>(args)...);
  }

} // namespace buffer_view
} // namespace crypto
