fjorge
======

`fjorge` is a command-line program for manually crafting and recording forged
HTTP(S) queries to be sent to a web server's TCP (HTTP) and TLS (HTTPS) ports.
It's intended to be like a `netcat` for the Hyper Text Transfer Protocol insofar 
as it allows as much user control over the protocol data as possible. Most HTTP 
command line tools like `curl` and `wget` can't make simple modifications to
requests such as non-standard methods, URI's that don't start with a slash, etc.

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
