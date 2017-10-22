cmake_minimum_required(VERSION 3.5.0)
project(crypto)

set(CRYPTO_ROOT_PATH "${PROJECT_SOURCE_DIR}")

include_directories("${CRYPTO_ROOT_PATH}/source")

# Define targets.
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(Crypto_Lib_Target cryptod)
  set(Crypto_Test_Target crypto_test_debug)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
  set(Crypto_Lib_Target crypto)
  set(Crypto_Test_Target crypto_test_release)
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

find_package(Threads)

# Setup openssl.
find_package(OpenSSL)
if (NOT OPENSSL_FOUND)
  message(FATAL_ERROR "OpenSSL not found")
endif (NOT OPENSSL_FOUND)

# Setup googletest.
set(GOOGLETEST_INCLUDE_DIR "${GTEST_INSTALL_PATH}/include")
set(GOOGLETEST_LIBRARY "${GTEST_INSTALL_PATH}/lib/libgtest.a")

# Crypto library.
file(GLOB crypto_INCLUDE "${CRYPTO_ROOT_PATH}/source/crypto/*.h")
file(GLOB crypto_SRC "${CRYPTO_ROOT_PATH}/source/crypto/*.cpp")
add_library(${Crypto_Lib_Target} STATIC ${crypto_INCLUDE} ${crypto_SRC})
target_include_directories(${Crypto_Lib_Target} PRIVATE ${OPENSSL_INCLUDE_DIR})
install(FILES ${crypto_INCLUDE} DESTINATION include/crypto)
install(TARGETS ${Crypto_Lib_Target} DESTINATION lib)

# Unit tests.
if (IS_DIRECTORY ${GTEST_INSTALL_PATH})
  file(GLOB crypto_test_SRC
      "${CRYPTO_ROOT_PATH}/source/test/*.h"
      "${CRYPTO_ROOT_PATH}/source/test/*.cpp")
  add_executable(${Crypto_Test_Target} ${crypto_test_SRC})
  target_include_directories(${Crypto_Test_Target} PRIVATE "${GTEST_INSTALL_PATH}/include")
  target_link_libraries(${Crypto_Test_Target}
      "${GTEST_INSTALL_PATH}/lib/libgtest.a"
       ${Crypto_Lib_Target}
       ${OPENSSL_CRYPTO_LIBRARY}
       ${CMAKE_THREAD_LIBS_INIT})
endif (IS_DIRECTORY ${GTEST_INSTALL_PATH})
