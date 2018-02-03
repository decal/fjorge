cmake_minimum_required(VERSION 2.8.12.2)
project(fjorge)
# SET(CMAKE_C_FLAGS "-g -pg -Iinclude -O0 -Wall -ansi -pedantic -std=c11 -DDEBUG -D_FORTIFY_SOURCE=2 -D__STDC_VERSION__=201112L -D__GNUC__")
# SET(CMAKE_C_FLAGS "-g -pg -Iinclude -O1 -Wall -ansi -pedantic -std=c11 -DDEBUG -D_FORTIFY_SOURCE=2")
SET(CMAKE_C_FLAGS "-g -Iinclude -O0 -Wall -ansi -pedantic -std=c11 -DDEBUG")
SET(CFLAGS CMAKE_C_FLAGS)
SET(CMAKE_LIBRARY_PATH_FLAG "-L/usr/local/lib")
SET(ENV{LD_LIBRARY_PATH} "/usr/local/lib:$ENV{LD_LIBRARY_PATH}")
add_definitions(-DDBNG_PREFIX=\"${CMAKE_INSTALL_PREFIX}\")

add_executable(
	fjorge
  src/add_header.c
  src/array_length.c
  src/array_portlist.c
  src/array_portstr.c
  src/auth_basic.c
  src/callback_info.c
  src/callback_message.c
  src/callback_ocsp.c
  src/callback_verify.c
  src/count_portstr.c
  src/count_portlist.c
  src/dup_headers.c
  src/encode_base64.c
  src/encode_hex.c
  src/extract_header.c
  src/fjputs_callback.c
  src/fjprintf_callback.c
  src/fjputs_debug.c
  src/fjprintf_debug.c
  src/fjputs_error.c
  src/fjprintf_error.c
  src/fjputs_verbose.c
  src/fjprintf_verbose.c
  src/main_function.c
  src/make_hostnames.c
  src/pack_protover.c
  src/parse_cmdline.c
  src/parse_portstr.c
  src/output_x509nm.c
  src/print_hostnames.c
  src/print_options.c
  src/print_trace.c
  src/rewrite_arg2head.c
  src/create_serial.c
  src/create_sockbio.c
  src/setcb_cookie.c
  src/signal_handler.c
  src/close_tcp.c
  src/connect_tcp.c
  src/connect_tls.c
  src/error_tcp.c
  src/error_tls.c
  src/recv_response.c
  src/send_request.c
  src/unpack_protover.c
  src/decode_url.c
  src/encode_url.c
  src/describe_usage.c
  include/fjorge.h)

find_library(SSL ssl)
find_library(CRYPTO crypto)
link_directories(/usr/local/lib)
add_library(ssl SHARED IMPORTED)
set_property(TARGET ssl PROPERTY IMPORTED_LOCATION /usr/local/lib/libssl.so)
add_library(crypto SHARED IMPORTED)
set_property(TARGET crypto PROPERTY IMPORTED_LOCATION /usr/local/lib/libcrypto.so)
target_link_libraries(fjorge ssl crypto)
install(TARGETS fjorge DESTINATION bin)
