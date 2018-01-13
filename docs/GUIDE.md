fjorge user's guide
===================

* Attack
  - `-A xss,x-forwarded-for:`
  - `-A x-forwarded-for:`
  - `-A xss`
  - `-A x-forwarded-for:,xss`

* Duplicate 
  - -D connection:,1
  - -D connection
  - -D connection,1
  - -D connection:
  - -D 1,connection
  - -D 1,connection:
  - -D 1
  - -D 10,:
  - -D :,10
  - -D 10,
  - -D ,10
  - -D

* Fuzz
  - request-line (method SP request-target SP http-version CRLF)
  - header-line (field-name ":" OWS field-value OWS)
  - ..check rfc..

* Reorder
  - alphabetic name
  - alphabetic value
  - random (shuffle)
  - swap

* Smuggle

- Need consistent field seperation syntax and meaning..
