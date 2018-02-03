fjorge
======

`fjorge` is a command-line program for manually crafting and recording forged
HTTP(S) queries to be sent to a web server's TCP (HTTP) and TLS (HTTPS) ports.
It's intended to be like a `netcat` for both HTTP and TLS insofar as it allows
as much user control over the protocl datum as possible. Most HTTP command-line
tools like `curl` and `wget` can't make simple modifications to requests such as
non-standard methods, URI's that don't start with a slash, etc.

For example:

`./fjorge bing.com:443 MYTEST . HT/9.9`

```
*<* MYTEST . HT/9.9
*<*
*>* Content-Type: text/html; charset=us-ascii
*>* Date: Sat, 13 Jan 2018 12:44:11 GMT
*>* Connection: close
*>* Content-Length: 311
*>* <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN""http://www.w3.org/TR/html4/strict.dtd">
*>* <HTML><HEAD><TITLE>Bad Request</TITLE>
*>* <META HTTP-EQUIV="Content-Type" Content="text/html; charset=us-ascii"></HEAD>
*>* <BODY><h2>Bad Request</h2>
*>* <hr><p>HTTP Error 400. The request is badly formed.</p>
*>* </BODY></HTML>
```

- This tool is written in C11 and has been tested on Ubuntu 16.04.3 LTS.

decal@UFO:~/GIT/decal/fjorge$ ./fjorge
*!* Expected arguments after command name

usage: ./fjorge HOST[:PORT] VERB PATH VERS [HHST[:HPRT]] [-d] [-s] [-v] [-y]
[-z] [-n D.OM] [-h 'NAME: VALU'] [-o OFIL] [-B U:PW] [-D [[NAME,DUPS]|[DUPS,N
AME]]
 HOST  numeric IP address or DNS hostname of target web server
 PORT  TCP port number to connect to on destination server
 VERB  HTTP protocol method: GET, HEAD, POST, OPTIONS, CONNECT
 PATH  relative path string or fully qualified URL
 VERS  protocol version string such as: HTTP/1.0, HTTP/2.0, etc.
 HHST  the hostname part of a custom Host header
 HPRT  the port number part of a custom Host header
 NAME  HTTP request header field name
 VALU  string value paired up with the new header name
 OFIL  path name of output file to write HTTP traffic to
 DUPS  number of duplicate HTTP request headers to create
 D.OM  hostname for SNI (Server Name Indication)
 U:PW  Basic Authentication string in user:password format

 -?  display the command line usage info being shown now
 -d  debug level (may be used more than once)
 -e  ensure that these characters are encoded (base64, cgi, html)
 -o  output the request and response text to a file
 -s  enable transport stream security with HTTPS
 -t  SSL/TLS version number for initial handshake
 -v  verbosity level (may be used more than once)
 -h  create an HTTP request header, duplicates permitted
 -n  Set the TLS SNI (Server Name Indication) extension in ClientHello
 -w  display custom-formatted output string template based on results
 -y  verify server-side certificate chain
 -z  fuzz request line and/or request headers
 -a  Basic authentication string in user:password format
 -b  brief ouput without document body; minimizes displayed text
 -c  provide cipher preferences according to CIPHER STRINGS section of cipher
s(1) manual
 -A  specify one or more attack types to unleash against the target
 -D  duplicate HTTP request headers (name,number OR number,name)
 -E  do not encode this set of characters (base64, cgi, html) -F  enable rand
om fuzzing of HTTP application layer protocol data -V  show detailed version
information and exit

ex. ./fjorge www.google.com:80 GET /apps HTTP/1.1 localhost:80
ex. ./fjorge office365.com:443 HEAD / HTTP/1.0 -s
