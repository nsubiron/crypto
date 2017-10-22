// Copyright (c) 2017 N Subiron Montoro. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#include "crypto/output.h"

#include <cstdio>
#include <memory>

namespace crypto {

  std::string to_hex_string(const_buffer_view data) {
    auto lenght = data.size();
    auto buffer = std::make_unique<char[]>(2u * lenght + 1u);
    for (auto i = 0u; i < lenght; ++i)
      sprintf(&buffer[2u * i], "%02x", data[i]);
    return std::string(buffer.get());
  }

} // namespace crypto
