// Copyright (c) 2017 N Subiron Montoro. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <type_traits>
#include <vector>

namespace crypto {

namespace detail {

  /// A view over an C-style array. Encapsulates the array and its size, but
  /// does NOT own the data.
  ///
  /// Do not use array_view_tmpl directly, use mutable_array_view or
  /// const_array_view.
  template <typename T>
  class array_view_tmpl {
  public:

    using value_type = T;
    using mutable_value_type = std::remove_const_t<T>;
    using const_value_type = const std::remove_const_t<T>;
    using size_type = std::size_t;
    using iterator = value_type*;
    using const_iterator = const_value_type*;

    explicit array_view_tmpl(value_type *data, size_type size)
        : _data(data),
          _size(size) {}

    template <typename O>
    array_view_tmpl(array_view_tmpl<O> rhs)
        : _data(rhs.data()),
          _size(rhs.size()) {}

    bool empty() const {
      return _size == 0u;
    }

    size_type size() const {
      return _size;
    }

    iterator begin() {
      return _data;
    }

    iterator end() {
      return _data + _size;
    }

    const_iterator begin() const {
      return _data;
    }

    const_iterator end() const {
      return _data + _size;
    }

    value_type *data() {
      return _data;
    }

    const_value_type *data() const {
      return _data;
    }

    value_type &operator[](size_type i) {
      return _data[i];
    }

    const_value_type &operator[](size_type i) const {
      return _data[i];
    }

  private:

    value_type *_data;

    size_type _size;
  };

  template<typename T>
  struct is_contiguous_container : std::false_type {};

  template<typename T>
  struct is_contiguous_container<array_view_tmpl<T>> : std::true_type {};

  template<typename T, typename U>
  struct is_contiguous_container<std::vector<T, U>> : std::true_type {};

  template<typename T>
  struct is_contiguous_container<std::vector<bool, T>> : std::false_type {};

  template<typename T, typename U, typename V>
  struct is_contiguous_container<std::basic_string<T, U, V>> : std::true_type {};

  template<typename T, std::size_t N>
  struct is_contiguous_container<std::array<T, N>> : std::true_type {};

} // namespace detail

  template <typename T>
  using mutable_array_view = detail::array_view_tmpl<std::remove_const_t<T>>;

  template <typename T>
  using const_array_view = detail::array_view_tmpl<const std::remove_const_t<T>>;

namespace array_view {

  template <typename T, typename V = mutable_array_view<T>>
  static inline auto make_mutable(T *data, typename V::size_type size) {
    return V(data, size);
  }

  template <typename T, typename V = mutable_array_view<typename T::value_type>>
  static inline auto make_mutable(T &container) {
    static_assert(detail::is_contiguous_container<T>::value, "can only make array_view from contiguous container");
    return V(container.data(), container.size());
  }

  template <typename T, typename V = const_array_view<T>>
  static inline auto make_const(const T *data, typename V::size_type size) {
    return V(data, size);
  }

  template <typename T, typename V = const_array_view<typename T::value_type>>
  static inline auto make_const(const T &container) {
    static_assert(detail::is_contiguous_container<T>::value, "can only make array_view from contiguous container");
    return V(container.data(), container.size());
  }

} // namespace array_view

} // namespace crypto
