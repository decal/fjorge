cmake_minimum_required(VERSION 2.8.12.2) 
project(fjorge)

# Compiler command-line flags under development
# SET(CMAKE_C_FLAGS "-g -pg -Iinclude -O0 -Wall -ansi -pedantic -std=c11 -DDEBUG -D_FORTIFY_SOURCE=2 -D__STDC_VERSION__=201112L -D__GNUC__")

# Compiler command-line flags in production
SET(CMAKE_C_FLAGS "-Iinclude -fopenmp -I/usr/local/include -O0 -Wall -ansi -pedantic -std=c11 -DDEBUG -D_FORTIFY_SOURCE=2")
SET(CFLAGS CMAKE_C_FLAGS)
SET(CMAKE_LIBRARY_PATH_FLAG "-L/usr/lib/x86_64-linux-gnu -L/usr/local/lib -lstrglob")

SET(ENV{LD_LIBRARY_PATH} "/usr/local/lib:/usr/lib/x86_64-linux-gnu:$ENV{LD_LIBRARY_PATH}")
add_definitions(-DDBNG_PREFIX=\"${CMAKE_INSTALL_PREFIX}\")

add_custom_target(distclean
  rm -f -- ./CMakeCache.txt
  rm -f -- ./CMakeFiles
  rm -f -- ./cmake_install.cmake
  rm -f -- ./Makefile
  COMMENT "Cleaning up build files" VERBATIM)

add_executable(
	fjorge
  src/add_header.c
  src/auth_basic.c
  src/callback_info.c
  src/callback_message.c
  src/callback_ocsp.c
  src/callback_verify.c
  src/dup_headers.c
  src/encode_base64.c
  src/encode_hex.c
  src/fjputs_callback.c
  src/fjprintf_callback.c
  src/fjputs_debug.c
  src/fjprintf_debug.c
  src/fjputs_error.c
  src/fjprintf_error.c
  src/fjputs_verbose.c
  src/fjprintf_verbose.c
  src/main_function.c
  src/pack_protover.c
  src/parse_cmdline.c
  src/output_x509nm.c
  src/print_options.c
  src/print_trace.c
  src/create_serial.c
  src/create_sockbio.c
  src/signal_handler.c
  src/strcat_glob.c
  src/connect_tcp.c
  src/connect_tls.c
  src/error_tcp.c
  src/error_tls.c
  src/recv_response.c
  src/send_request.c
  src/unpack_protover.c
  src/describe_usage.c
  include/fjorge.h)

find_library(SSL ssl)
find_library(CRYPTO crypto)
find_library(STRGLOB strglob)

link_directories(/usr/lib/x86_64-linux-gnu /usr/local/lib)

add_library(ssl SHARED IMPORTED)
set_property(TARGET ssl PROPERTY IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libssl.so)

add_library(crypto SHARED IMPORTED)
set_property(TARGET crypto PROPERTY IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libcrypto.so)

add_library(strglob SHARED IMPORTED)
set_property(TARGET strglob PROPERTY IMPORTED_LOCATION /usr/local/lib/libstrglob.so)

target_link_libraries(fjorge ssl crypto strglob)

install(TARGETS fjorge DESTINATION bin)
