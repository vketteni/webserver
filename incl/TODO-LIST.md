# Mandatory

## **Message Body**

**TODO**
- **Detect the presence of a message body**:
  - For **requests**, the presence of a message body is signaled by either a `Content-Length` or `Transfer-Encoding` header field.
    - `Request message framing is independent of method semantics. Even if the method doesn't define a use for a message body, it may still have one.`

**TODO**
- **Handle message body in responses based on method and status code**:
  - Responses to **HEAD requests** must not include a message body.
    - The response headers like `Transfer-Encoding` and `Content-Length` indicate what their values would have been for a GET request.
  - **CONNECT requests** with 2xx (Successful) responses switch to **tunnel mode** and do not include a message body.
  - **No message body is included** in:
    - **1xx (Informational)** responses
    - **204 (No Content)** responses
    - **304 (Not Modified)** responses
  - All other responses **must include a message body**, even if it is of zero length.

**TODO**
- **Handle responses with zero-length bodies** appropriately:
  - Ensure that responses without content still correctly reflect a zero-length body when applicable.

**TODO**
- **Handle message body transformations**:
  - The message body is identical to the payload body unless a **transfer coding** is applied (handled in Section 3.3.1).



## **HTTP Headers**
- `header-field   = field-name ":" OWS field-value OWS`

**TODO** 
- **Function**: Remove optional white space (OWS)
- `std::string trim_func(std::string target_chars) {}`

**TODO**
- Reject request field-names that contain white space
- `400 (Bad Request)`

**TODO**
- Reject messages with white space between start-line and headers
- `   A sender MUST NOT send whitespace between the start-line and the
   first header field.  A recipient that receives whitespace between the
   start-line and the first header field MUST either reject the message
   as invalid or consume each whitespace-preceded line without further
   processing of it (i.e., ignore the entire line, along with any
   subsequent lines preceded by whitespace, until a properly formed
   header field is received or the header section is terminated).`

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

**TODO**
- Figure out what to do about `userinfo` subcomponent
- ` A sender MUST NOT
   generate the userinfo subcomponent (and its "@" delimiter) when an
   "http" URI reference is generated within a message as a request
   target or header field value.  Before making use of an "http" URI
   reference received from an untrusted source, a recipient SHOULD parse
   for userinfo and treat its presence as an error; it is likely being
   used to obscure the authority for the sake of phishing attacks.`

**TODO**
- Normalize URIs
- https://www.rfc-editor.org/rfc/rfc3986#section-6
- `In testing for equivalence, applications should not directly compare
   relative references; the references should be converted to their
   respective target URIs before comparison.  When URIs are compared to
   select (or avoid) a network action, such as retrieval of a
   representation, fragment components (if any) should be excluded from
   the comparison.`

## **Request Line**

**TODO**
- Ensure that the **method** in the request-line is case-sensitive.
  - Reference: Method definitions in RFC7231 Section 4.

**TODO**
- Validate that no whitespace is present in the request-line components (method, request-target, and HTTP version).
  - `A recipient MUST reject any request-line containing disallowed whitespace characters.`

**TODO**
- Respond with a `400 (Bad Request)` if an invalid request-line is detected.
  - `Optionally respond with a 301 (Moved Permanently) if it can be corrected by proper encoding of the request-target.`

## **Request Line Length**

**TODO**
- Implement support for a minimum request-line length of **8000 octets**.
  - `RECOMMENDED: All HTTP senders and recipients SHOULD support request-line lengths of at least 8000 octets.`

**TODO**
- Handle **method length** in the request-line.
  - If the **method** in the request-line exceeds the supported length for any method, respond with `501 (Not Implemented)`.

**TODO**
- Ensure proper handling of **overly long request-targets**.
  - `A server that receives a request-target longer than it can handle MUST respond with a 414 (URI Too Long) status code.`


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


Here's the actionable TODO list based on the information about obs-folding in HTTP header fields:

markdown
Copy code
## **Header Field Line Folding (obs-fold)**

**TODO**
- **Deprecate obs-fold**: Ensure that no message generated by the server includes **obs-folding** in header fields, except when packaging within the `message/http` media type.
  - `A sender MUST NOT generate a message that includes obs-fold, except for message/http media type.`

**TODO**
- Handle **obs-folding** in received requests:
  - If a request contains **obs-folding**, the server MUST:
    - **Reject the message** with a `400 (Bad Request)` status code, and preferably provide a response explaining that obsolete line folding is not allowed.
    - Alternatively, the server MAY replace the **obs-fold** with one or more space (`SP`) characters before processing the header field or forwarding the message downstream.

**TODO**
- Ensure **obs-fold** detection and handling in the request pipeline:
  - Implement logic to detect and handle **obs-folding** in HTTP header fields during parsing.
  - Decide on rejection or normalization based on server preferences.

## **Header Field Character Sets**

**TODO**
- Ensure that newly defined header fields **limit their field values to US-ASCII** octets.
  - `Newly defined header fields SHOULD use only US-ASCII charset.`

**TODO**
- Handle **non-US-ASCII characters (obs-text)** in received HTTP header field values.
  - **Treat non-US-ASCII octets as opaque data**.
  - `A recipient SHOULD treat octets outside US-ASCII (obs-text) as opaque data and avoid interpreting or altering their meaning.`

**TODO**
- Decide on handling legacy encodings:
  - **Do not rely on ISO-8859-1 or RFC2047** encoding for newly defined headers.
  - Continue supporting them only where backward compatibility is required.

**TODO**
- Ensure **robust parsing**:
  - Implement robust parsing mechanisms that can safely handle any non-ASCII data in HTTP header fields without corrupting or misinterpreting the content.

## **Request Header Field Size**

**TODO**
- **Set limits on header field sizes**:
  - Implement a maximum size for individual request header fields or the total size of all header fields the server is willing to process.

**TODO**
- **Respond with an appropriate 4xx status code** when the header field size exceeds the limit.
  - If a request header field, or set of fields, is larger than the server can handle, respond with a `4xx (Client Error)` status code, such as `413 (Payload Too Large)`.

**TODO**
- **Mitigate request smuggling attacks**:
  - Ensure that any oversized or unexpected header fields are not ignored, as this could increase the risk of **request smuggling** attacks (RFC 7230, Section 9.5).
  - Validate header fields and immediately reject any that exceed size limits to maintain security.

**TODO**
- Provide clear **error response** in case of large headers:
  - Optionally include a response body that explains the header size limitation to the client.

## **Field Value Components**

**TODO**
- **Parse tokens** correctly in header field values:
  - Ensure tokens consist only of valid `tchar` characters:
    - `tchar = "!" / "#" / "$" / "%" / "&" / "'" / "*" / "+" / "-" / "." / "^" / "_" / "`" / "|" / "~" / DIGIT / ALPHA`
    - Exclude delimiters: `DQUOTE and "(),/:;<=>?@[\]{}"`

**TODO**
- **Support quoted-strings** in header field values:
  - Correctly parse text enclosed in double quotes (`DQUOTE`) as a single field value.
  - Ensure the value consists of valid `qdtext` or `quoted-pair`:
    - `qdtext = HTAB / SP / %x21 / %x23-5B / %x5D-7E / obs-text`
    - `quoted-pair = "\" ( HTAB / SP / VCHAR / obs-text )`

**TODO**
- **Process comments** in header fields when allowed:
  - Comments are allowed only in header fields explicitly permitting them.
  - Correctly parse comments enclosed in parentheses `()` using `ctext` and `quoted-pair` rules:
    - `ctext = HTAB / SP / %x21-27 / %x2A-5B / %x5D-7E / obs-text`

**TODO**
- **Handle quoted-pairs** in `quoted-strings` and `comments`:
  - Recipients MUST process `quoted-pair` by replacing the backslash `\` and the following octet with the octet itself.

**TODO**
- **Avoid unnecessary quoted-pairs**:
  - A sender SHOULD NOT generate a `quoted-pair` in a `quoted-string` unless quoting `DQUOTE` or `\`.
  - A sender SHOULD NOT generate a `quoted-pair` in a `comment` unless quoting parentheses `(` or `)` or backslash `\`.

**TODO**
- **Ensure robust parsing** of `obs-text`:
  - Handle `obs-text` (non-ASCII octets: `0x80-FF`) correctly in field values, quoted-strings, and comments.

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