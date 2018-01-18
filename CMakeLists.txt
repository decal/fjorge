cmake_minimum_required(VERSION 2.8.12.2)
project(fjorge)
# SET(CMAKE_C_FLAGS "-g -pg -Iinclude -O0 -Wall -ansi -pedantic -std=c11 -DDEBUG -D_FORTIFY_SOURCE=2 -D__STDC_VERSION__=201112L -D__GNUC__")
SET(CMAKE_C_FLAGS "-g -pg -Iinclude -O1 -Wall -ansi -pedantic -std=c11 -DDEBUG -D_FORTIFY_SOURCE=2")
SET(CFLAGS CMAKE_C_FLAGS)
SET(CMAKE_LIBRARY_PATH_FLAG "-L/usr/local/lib")
SET(ENV{LD_LIBRARY_PATH} "/usr/local/lib:$ENV{LD_LIBRARY_PATH}")
add_definitions(-DDBNG_PREFIX=\"${CMAKE_INSTALL_PREFIX}\")

add_executable(
	fjorge
  src/add_header.c
  src/array_length.c
  src/array_ports.c
  src/auth_basic.c
  src/count_ports.c
  src/dup_headers.c
  src/encode_base64.c
  src/fjputs_callback.c
  src/fjprintf_callback.c
  src/fjputs_debug.c
  src/fjprintf_debug.c
  src/fjputs_error.c
  src/fjprintf_error.c
  src/fjputs_verbose.c
  src/fjprintf_verbose.c
  src/info_callback.c
  src/main_function.c
  src/make_hostnames.c
  src/match_cn.c
  src/match_san.c
  src/pack_protover.c
  src/parse_cmdline.c
  src/parse_ports.c
  src/cbprint_cnname.c
  src/error_callback.c
  src/cbprint_sanname.c
  src/print_hostnames.c
  src/print_options.c
  src/print_trace.c
  src/recv_response.c
  src/send_request.c
  src/signal_handler.c
  src/close_tcp.c
  src/connect_tcp.c
  src/connect_tls.c
  src/error_tls.c
  src/recv_tls.c
  src/send_tls.c
  src/unpack_protover.c
  src/decode_url.c
  src/encode_url.c
  src/usage_desc.c
  src/validate_hostname.c
  src/verify_callback.c
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
