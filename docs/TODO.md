__To-Do List__
==========

* [Context options and parameters](https://secure.php.net/manual/en/context.php "php.net Language Reference")

* add ANSI colors

* merge recv_response and recv_tls so parsing code is all the same!

* doxygen everything

* make vcmd a regular object instead of a pointer

* make CLI flag for callbacks take string to distinguish between one or more callback function types 

* SSL_session_has_ticket

* SSL_CTX_set_tlsext_status_cb

* renegotiate with different SNI name

* update OPTIONS: debugging output

* add CONNECT method cases to `bin/canproxys` script

* implement chunked encoding for responses first, then requests

* act as HTTP(S) server daemon

* request body crafting, i.e. https://www.exploit-db.com/exploits/22106/

* output formatting similar to `curl -o`

* implement HTTP/2.0

* make sure each option passed to `getopt()` is implemented

* document each command line option in the user's guide

* make documentation more understandable for n00blet llamahs like Elih

* expand upon code within callback functions 

* add support for `getopt_long()` command line flags

* make usage similar to `lwp-request`, `curl`, `wbox`, `netcat`, etc. ?

* implement WebDAV and other methods 

* support chunked transfer and gzip encoding (`Transfer-Encoding` HTTP request header)

* follow HTTP response `Location` header redirects a set number of times

* display `Content-Length` with brief mode in case reply codes are all 200

* create a manual page with `nroff(1)`

* -b, --cookie STRING/FILE  Read cookies from STRING/FILE (H)

*  -c, --cookie-jar FILE  Write cookies to FILE after operation (H)

* --create-dirs   Create necessary local directory hierarchy

* --crlf          Convert LF to CRLF in upload

* [Guile Reference Manual](https://www.gnu.org/software/guile/manual/html_node/Dynamic-FFI.html#Dynamic-FFI "Dynamic FFI")

