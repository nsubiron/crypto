crypto
======

Few handy wrappers around [OpenSSL](https://www.openssl.org/)'s libcrypto.

Includes

  * Simple secure string
  * Password digest class

Build
-----

Build with cmake. For default cmake configuration use the provided Makefile.
Requires C++14 or later.

Unit tests
----------

The unit tests require [Google Test](https://github.com/google/googletest), the
setup script downloads and compiles GTest to use with the provided Makefile

    $ ./Setup.sh
    $ make check

Usage
-----

```cpp
#include "crypto/password_digest.h"

#include <iostream>
#include <unistd.h>

int main() {
  using namespace crypto;

  char *unsafe_buffer = getpass("Password: ");
  auto password = secure_string::clean_buffer_and_make(unsafe_buffer);

  auto digest = password_digest<sha256_digest>(password);

  unsafe_buffer = getpass("Confirm password: ");
  auto confirm_password = secure_string::clean_buffer_and_make(unsafe_buffer);

  if (digest != confirm_password)
    std::cerr << "Passwords do not match!" << std::endl;
}
```
