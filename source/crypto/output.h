// Copyright (c) 2017 N Subiron Montoro. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "crypto/buffer_view.h"

#include <string>

namespace crypto {

  std::string to_hex_string(const_buffer_view buffer);

} // namespace crypto
