# Mandatory
## **HTTP Headers**
- `header-field   = field-name ":" OWS field-value OWS`

**TODO** 
- **Function**: Remove optional white space (OWS)
- `std::string trim_func(std::string target_chars) {}`

**TODO**
- Reject request field-names that contain white space
- `400 (Bad Request)`

**TODO**
- Handle the `date` header field
- `An origin server MAY
   send a Date header field if the response is in the 1xx
   (Informational) or 5xx (Server Error) class of status codes.  An
   origin server MUST send a Date header field in all other cases.`
- `A recipient with a clock that receives a response message without a
   Date header field MUST record the time it was received and append a
   corresponding Date header field to the message's header section if it
   is cached or forwarded downstream.`

## **Request Methods**
**TODO**
- `A server that receives a
   method longer than any that it implements SHOULD respond with a 501
   (Not Implemented) status code.`

## **Request Uri**
**TODO**
- `A server that receives a request-target longer than any URI it wishes to parse MUST respond
   with a 414 (URI Too Long) status code`

**TODO**
- Recognize and reject bad "http" URI
- `A sender MUST NOT generate an "http" URI with an empty host
   identifier.  A recipient that processes such a URI reference MUST
   reject it as invalid.`
- https://www.rfc-editor.org/rfc/rfc7230#section-6.3

## **Request Line**
**TODO**
- Let's implement at 8000 octets
- `Various ad hoc limitations on request-line length are found in
   practice.  It is RECOMMENDED that all HTTP senders and recipients
   support, at a minimum, request-line lengths of 8000 octets.`

## **Status Codes**
**TODO**
- Handle each status codes appropriately 
- https://www.rfc-editor.org/rfc/rfc7231#section-6
```
   -  1xx (Informational): The request was received, continuing process

   -  2xx (Successful): The request was successfully received,
      understood, and accepted

   -  3xx (Redirection): Further action needs to be taken in order to
      complete the request

   -  4xx (Client Error): The request contains bad syntax or cannot be
      fulfilled
   -  5xx (Server Error): The server failed to fulfill an apparently
      valid request
```

**TODO**
- Implement reason phrase for each status code

___
___

# Bonus
## **Cookie Header Value** 
- https://www.rfc-editor.org/rfc/rfc7230#section-6.3
- ` the "Set-Cookie" header field ([RFC6265]) often
      appears multiple times in a response message and does not use the
      list syntax, violating the above requirements on multiple header
      fields with the same name.  Since it cannot be combined into a
      single field-value, recipients ought to handle "Set-Cookie" as a
      special case while processing header fields.`


**TODO** 
- Handle cookie header values seperately 