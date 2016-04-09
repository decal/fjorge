fjorge
======

fjorge is a command-line program for manually testing and recording possible
SSRF (Server-Side Request Forgery) attack vectors on a web server's TCP and TLS
ports.  It's intended to be similar to netcat insofar as it allows as much user
control over the input data as possible.  Of course other tools like curl, wbox,
HTTP-Traceroute.py and others exist, but they all fall short with regards to
this increasingly important use case in information security.  SSRF is an area
of the network attack taxonomy where it overlaps between two distinct assessment
types: web application penetration testing and external vulnerability assessment
of network infrastructure.

This tool is written in C11 for speed and portability.
