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

## **Transfer-Encoding**

**TODO**
- **Handle the `Transfer-Encoding` header**:
  - Parse the `Transfer-Encoding` header field which lists transfer codings applied to the payload body.
  - Ensure support for multiple transfer codings (e.g., `gzip, chunked`) applied in sequence, as per `Transfer-Encoding = 1#transfer-coding`.

**TODO**
- **Implement mandatory chunked transfer coding**:
  - Recipients MUST be able to parse **chunked** transfer coding since it is crucial for framing messages when the payload size is unknown.
  - Ensure that **chunked transfer coding** is applied as the final transfer coding for:
    - Requests with other transfer codings (e.g., `gzip`).
    - Responses with other transfer codings, or ensure termination by closing the connection.

**TODO**
- **Enforce rules for transfer coding application**:
  - A sender MUST NOT apply chunked transfer coding more than once to a message body.
  - Ensure that a **server** does NOT send a `Transfer-Encoding` header in:
    - Responses with status code **1xx (Informational)** or **204 (No Content)**.
    - **2xx (Successful)** responses to a **CONNECT** request.
  - Clients MUST NOT send a `Transfer-Encoding` header in requests unless they are certain that the server supports HTTP/1.1 or later.

**TODO**
- **Handle transfer encoding in specific responses**:
  - Transfer-Encoding MAY be included in responses to a **HEAD request** or **304 (Not Modified)** response, to indicate what transfer coding would have been applied if the request was an unconditional GET.
  - Ensure proper handling of this header in such cases, though it is not mandatory to send.

**TODO**
- **Handle unknown transfer codings**:
  - If a server receives a request with a transfer coding it does not understand, it SHOULD respond with a **501 (Not Implemented)** status code.

**TODO**
- **Differentiate Transfer-Encoding from Content-Encoding**:
  - `Transfer-Encoding` is a property of the **message** and not the **representation**. Recipients MAY decode or apply additional transfer codings and adjust the `Transfer-Encoding` field accordingly.

**TODO**
- **Ensure HTTP version compatibility**:
  - A server MUST NOT send `Transfer-Encoding` in a response unless the corresponding request uses **HTTP/1.1** or later.
  - Ensure that clients do not use `Transfer-Encoding` in requests unless the server supports **HTTP/1.1**.

## **Content-Length**

**TODO**
- **Handle the `Content-Length` header**:
  - Ensure the `Content-Length` header field provides the anticipated size (in octets) of the payload body when no `Transfer-Encoding` is present.
  - Parse `Content-Length` as a decimal number to determine where the message body ends.

**TODO**
- **Avoid conflicts between `Content-Length` and `Transfer-Encoding`**:
  - A sender MUST NOT send a `Content-Length` header field if the message contains a `Transfer-Encoding` header.
  
**TODO**
- **Handle `Content-Length` in responses correctly**:
  - A server MAY send a `Content-Length` header in responses to **HEAD** requests, but it MUST equal the number of octets that would have been sent if the request were **GET**.
  - A server MAY send `Content-Length` in a **304 (Not Modified)** response but only if it equals the size that would have been sent in a **200 (OK)** response.
  - A server MUST NOT send a `Content-Length` header in:
    - **1xx (Informational)** or **204 (No Content)** responses.
    - **2xx (Successful)** responses to a **CONNECT** request.
    
**TODO**
- **Send `Content-Length` for known payload body sizes**:
  - When no `Transfer-Encoding` is present and the payload body size is known, the origin server SHOULD send a `Content-Length` header to help downstream recipients know when the message is complete and potentially reuse the connection.

**TODO**
- **Anticipate large Content-Length values**:
  - Ensure your system can handle potentially large `Content-Length` values to prevent integer overflow issues during parsing.

**TODO**
- **Handle multiple or duplicate Content-Length header fields**:
  - If a message is received with multiple `Content-Length` header fields containing identical decimal values, or a single `Content-Length` field with a list of identical values (e.g., `Content-Length: 42, 42`), then the recipient MUST either:
    - Reject the message as invalid, OR
    - Replace the duplicate values with a single valid `Content-Length` field before determining the message body length or forwarding the message.
  
**TODO**
- **Handle invalid or non-identical `Content-Length` values**:
  - If multiple `Content-Length` header fields with differing values are received, the message SHOULD be treated as invalid and rejected.

## **Message Body Length**

**TODO**
- **Determine message body length based on the following rules (in order of precedence):**
  1. **HEAD Requests, 1xx, 204, and 304 Responses**:
     - These messages cannot contain a body. Terminate the message after the first empty line following the header fields.
  
  2. **2xx Response to a CONNECT Request**:
     - The connection becomes a tunnel after the header fields. Ignore any `Content-Length` or `Transfer-Encoding` header fields.

  3. **Transfer-Encoding is Present**:
     - If `Transfer-Encoding` has chunked as the final coding, determine the body length by reading and decoding the chunked data until completion.
     - If the final coding is **not chunked**:
       - In responses, determine the body length by reading until the connection is closed.
       - In requests, respond with `400 (Bad Request)` and close the connection.
     - If both `Transfer-Encoding` and `Content-Length` are present, `Transfer-Encoding` takes precedence, and `Content-Length` should be removed before forwarding to avoid potential attacks like request smuggling or response splitting.

  4. **Multiple or Invalid Content-Length Fields**:
     - If there are multiple `Content-Length` fields with differing values or invalid data, the framing is invalid.
       - For requests, respond with `400 (Bad Request)` and close the connection.
       - For responses:
         - **Proxies** must close the connection to the server, discard the response, and send a `502 (Bad Gateway)` to the client.
         - **User agents** must close the connection to the server and discard the response.

  5. **Valid Content-Length and No Transfer-Encoding**:
     - The message body length is defined by the `Content-Length` value in octets. If the connection is closed prematurely, treat the message as incomplete and close the connection.

  6. **No Content-Length or Transfer-Encoding in Requests**:
     - If none of the above conditions apply to a request, assume the body length is zero (no body present).

  7. **Response Without Declared Message Length**:
     - If a response does not declare its body length via `Content-Length` or `Transfer-Encoding`, the body length is determined by the number of octets received before the server closes the connection.

**TODO**
- **Generate length-delimited or encoded messages**:
  - Whenever possible, servers SHOULD use transfer encoding or a `Content-Length` header to ensure that downstream recipients can determine when the message ends.
  - Avoid using close-delimited messages unless for backward compatibility with HTTP/1.0.

**TODO**
- **Handle requests without `Content-Length`**:
  - A server MAY reject requests that have a body but do not include a `Content-Length` by responding with `411 (Length Required)`.

**NOTE**:
- Be cautious about handling extra data received after the final response, as incorrect behavior could expose vulnerabilities such as cache poisoning.

# Handling Incomplete Messages


## **Incomplete Response Message**
**TODO**
- Record and handle incomplete response messages correctly
  - `A client that receives an incomplete response message, which can occur when a connection is closed prematurely or when decoding a supposedly chunked transfer coding fails, MUST record the message as incomplete.`
  - Reference: [Section 3 of RFC7234](https://www.rfc-editor.org/rfc/rfc7234#section-3)

**TODO**
- Determine completeness of a message using chunked transfer coding
  - `A message body that uses the chunked transfer coding is incomplete if the zero-sized chunk that terminates the encoding has not been received.`

**TODO**
- Validate completeness of a message using Content-Length
  - `A message that uses a valid Content-Length is incomplete if the size of the message body received (in octets) is less than the value given by Content-Length.`

**TODO**
- Finalize message with neither chunked transfer coding nor Content-Length by connection closure
  - `A response that has neither chunked transfer coding nor Content-Length is terminated by closure of the connection and, thus, is considered complete regardless of the number of message body octets received, provided that the header section was received intact.`

# Transfer Codings

## **Transfer Coding Implementation**

**TODO**
- Implement parsing logic for transfer-coding names
  - `transfer-coding = "chunked" / "compress" / "deflate" / "gzip" / transfer-extension`
  - Ensure backward whitespace (BWS) is handled appropriately for parameters: `transfer-parameter = token BWS "=" BWS (token / quoted-string)`
  
**TODO**
- Handle `chunked` transfer coding
  - Refer to [Section 4.1](https://www.rfc-editor.org/rfc/rfc7230#section-4.1)
  
**TODO**
- Handle `compress` transfer coding
  - Refer to [Section 4.2.1](https://www.rfc-editor.org/rfc/rfc7230#section-4.2.1)

**TODO**
- Handle `deflate` transfer coding
  - Refer to [Section 4.2.2](https://www.rfc-editor.org/rfc/rfc7230#section-4.2.2)
  
**TODO**
- Handle `gzip` transfer coding
  - Refer to [Section 4.2.3](https://www.rfc-editor.org/rfc/rfc7230#section-4.2.3)

**TODO**
- Validate transfer-coding against HTTP Transfer Coding registry
  - Refer to [Section 8.4](https://www.rfc-editor.org/rfc/rfc7230#section-8.4)

**TODO**
- Implement support for transfer extensions and parameters
  - Example: `transfer-extension = token *( OWS ";" OWS transfer-parameter )`
  
**TODO**
- Ensure all transfer-coding names are case-insensitive in implementation

**TODO**
- Integrate transfer-coding logic with TE and Transfer-Encoding header fields
  - Refer to [Section 4.3](https://www.rfc-editor.org/rfc/rfc7230#section-4.3) and [Section 3.3.1](https://www.rfc-editor.org/rfc/rfc7230#section-3.3.1) for details

---

This section focuses on the different types of transfer codings, their specifications, and the implementation details needed for parsing and handling encoding transformations.
## **Chunked Transfer Coding**

**TODO**
- Implement parsing logic for chunked transfer coding
  - Ensure that payload is transferred as a series of chunks
  - Each chunk should be processed with its size indicator followed by chunk-data.

**TODO**
- Implement handling for last-chunk to end the chunked payload
  - Acknowledge and process trailer-part if present
  - Terminate the payload when a chunk-size of zero is encountered, followed by an empty line

**TODO**
- Create functions to parse chunk-size as hex digits and accurately determine the data size in octets

**Function Sketch**
- `size_t parse_chunk_size(const std::string& chunk_size_hex) { /* Hex to size_t conversion logic */ }`

**TODO**
- Validate incoming messages for proper chunked transfer coding
  - Ensure the receipt of all sequences as defined in the chunked-body, including chunk-data and last-chunk

**TODO**
- Implement handling for optional trailer containing header fields
  - Properly process the trailer-part following the last-chunk, ensuring seamless completion of data transfer
## **Chunked Transfer Encoding**

### **Chunk Extensions**

**TODO**

- Implement support for chunk extensions.
  - A chunk extension allows for additional metadata or control information per chunk.
  - `chunk-ext = *( ";" chunk-ext-name [ "=" chunk-ext-val ] )`
  - `chunk-ext-name = token`
  - `chunk-ext-val = token / quoted-string`

- Ensure the system ignores unrecognized chunk extensions.
  - `A recipient MUST ignore unrecognized chunk extensions.`

- Implement logic to limit the total length of received chunk extensions.
  - `A server ought to limit the total length of chunk extensions received in a request to an amount reasonable for the services provided.`
  - Generate an appropriate 4xx (Client Error) response if the limit is exceeded.

- Be mindful that chunked encoding is connection-specific and may be altered by intermediaries.
  - `The chunked encoding is specific to each connection and is likely to be removed or recoded by each recipient (including intermediaries) before any higher-level application would have a chance to inspect the extensions.`

- Recognize that chunk extensions are generally used in specialized HTTP services.
  - Examples include "long polling" or for padding within an end-to-end secured connection.
## **Chunked Trailer Part**
**TODO**
- Implement support for trailers in chunked messages
  - **Function**: Ensure the sender does not include fields necessary for message framing, routing, request modifiers, authentication, response control data, or payload processing in the trailer.
  - **Function**: `process_trailer(std::vector<std::string> trailer_fields) {}`
    - Validate and process trailer fields following the regulations mentioned in RFC 7230.
    - Ensure any fields forbidden to be sent in a trailer are ignored or treated as an error.

**TODO**
- Implement checks for "TE" Header Field
  - **Function**: Ensure a server only generates trailer fields when the "TE" header field includes "trailers".
  - This ensures compatibility with intermediaries forwarding responses to HTTP/1.0 recipients without extra buffering.
# Decoding Chunked Transfer Coding

## **Chunked Transfer Decoding Logic**
**TODO**
- Implement chunked transfer decoding based on the RFC guidance 
- Use the provided pseudo-code as a reference for implementation:
  - Initialize `length := 0`
  - In a loop, read `chunk-size`, `chunk-ext` (if any), and `CRLF`
    - While (`chunk-size > 0`):
        - Read `chunk-data` and `CRLF`
        - Append `chunk-data` to `decoded-body`
        - Update `length := length + chunk-size`
        - Read `chunk-size`, `chunk-ext` (if any), and `CRLF`
  - After the loop, process the trailer fields:
    - While (`trailer field is not empty`):
        - If `trailer field is allowed to be sent in a trailer`, append it to existing header fields
        - Read `trailer-field`
  - Set `Content-Length := length`
  - Remove "chunked" from Transfer-Encoding
  - Remove "Trailer" from existing header fields

**REFERENCE**
- https://www.rfc-editor.org/rfc/rfc7230#section-4.1.3

By incorporating this decoding logic, ensure compliance with the specifications for handling chunked transfer encoding.
# Mandatory
## **Compression Codings**
**TODO**
- Implement support for compression codings to compress the payload of a message.
- Evaluate and choose appropriate compression algorithms such as gzip, deflate, or brotli.
- Ensure compliant implementation of Content-Encoding and Accept-Encoding headers to handle compressed data exchange between client and server.
- `A client includes the "gzip" header in requests to indicate it can decode a "Content-Encoding: gzip" response.  A server sends a response with the "gzip" header if the payload is compressed using the gzip format.`
## **Compress Coding**
**TODO**
- Implement support for "compress" coding in HTTP responses
- `The "compress" coding is an adaptive Lempel-Ziv-Welch (LZW) coding that is commonly produced by the UNIX file compression program "compress".`
- Consider "x-compress" as equivalent to "compress" when processing
- `A recipient SHOULD consider "x-compress" to be equivalent to "compress".`
## **Deflate Coding**
**TODO**
- Implement support for `deflate` coding
  - Ensure compliance with "zlib" data format as per [RFC1950](https://www.rfc-editor.org/rfc/rfc1950)
  - Handle "deflate" compressed data stream using Lempel-Ziv (LZ77) and Huffman coding as per [RFC1951](https://www.rfc-editor.org/rfc/rfc1951)
  
**TODO**
- Handle non-conformant implementations that send "deflate" compressed data without the zlib wrapper
  - Detect and process these implementations appropriately to ensure compatibility without compromising data integrity

**Reference:**
- [RFC1950 - ZLIB Compressed Data Format Specification version 3.3](https://www.rfc-editor.org/rfc/rfc1950)
- [RFC1951 - DEFLATE Compressed Data Format Specification version 1.3](https://www.rfc-editor.org/rfc/rfc1951)
## **Content Codings**
**TODO**
- Implement support for "gzip" coding
- `The "gzip" coding is an LZ77 coding with a 32-bit Cyclic Redundancy Check (CRC) commonly produced by the gzip file compression program [RFC1952].`
- Implement logic to treat "x-gzip" as equivalent to "gzip"
- `A recipient SHOULD consider "x-gzip" to be equivalent to "gzip".`
# TE Header

## **TE Header Field**

**TODO** 
- Parse and handle `TE` header field appropriately
- When parsing requests, extract the `TE` header field value, which may consist of a comma-separated list of transfer coding names and/or the keyword "trailers."

**TODO**
- Enforce restrictions on `TE` header
- Ensure that the chunked transfer coding name is not present in the `TE` header field.

**TODO**
- Handle client preferences for transfer codings
- Implement logic to respect the client's ranked preferences for transfer codings using the `q` parameter, where rank values range from 0.001 (least preferred) to 1 (most preferred).

**TODO**
- Manage chunked responses and trailers
- Implement support for the `trailers` keyword, which indicates the client's willingness to accept trailer fields in a chunked transfer coding.

**TODO**
- Ensure `TE` in Connection header
- Ensure that when sending a `TE` header field, a `TE` connection option is included in the `Connection` header field to prevent forwarding by intermediaries unaware of its semantics.
# Additional Actions

## **Message Trailer**
**TODO**
- Implement logic to handle and process Trailer header fields
- `When a message includes a message body encoded with the chunked transfer coding, the sender should generate a Trailer header field before the message body to indicate which fields will be present in the trailers.`

**TODO**
- Ensure the recipient can process trailer headers for streamed messages
- `This allows the recipient to prepare for receipt of that metadata before it starts processing the body, which is useful if the message is being streamed and the recipient wishes to confirm an integrity check on the fly.`


# Message Routing

## **Routing Logic**

**TODO**
- Implement message routing logic based on the target resource.
- Incorporate client's proxy configuration for routing decisions.
- Ensure that inbound connection establishment or reuse is considered in the routing workflow.
- Route the corresponding response back through the same connection chain.

## **Proxy Configuration**

**TODO**
- Develop functionality to parse and apply client's proxy configuration to determine routing path.
- `HTTP request message routing is influenced by the client's proxy settings, which need to be thoughtfully integrated into the routing logic to avoid misrouting of requests.`

## **Connection Chain**

**TODO**
- Ensure that responses follow the connection chain back to the client.
- Verify that both the request and corresponding response use the same connection path for consistency.
- `The response routing is expected to follow the same connection chain back to the client through which the request was sent.` 

## **Resource Targeting**

**TODO**
- Design routing mechanisms to accurately determine the resource target from the request.
- Use the target resource information to guide the request routing process.
- `Each client determines the target resource, which acts as a primary factor for routing HTTP requests.`
# Target Resource Identification

## **Target URI Resolution**

**TODO**
- Implement logic to resolve URI references to absolute form to obtain the target URI.
- `The purpose is a combination of request semantics, which are defined in [RFC7231], and a target resource upon which to apply those semantics.`

**TODO**
- Exclude fragment component from the target URI as it is reserved for client-side processing.
- Reference: [RFC3986, Section 3.5](https://www.rfc-editor.org/rfc/rfc3986#section-3.5)

___
___

**Note**: Below is the section hyperlinked for easy access:

- [RFC7231 - Semantics and Content](https://www.rfc-editor.org/rfc/rfc7231)
- [RFC7230 - Section 2.7](https://www.rfc-editor.org/rfc/rfc7230#section-2.7)
- [RFC3986 - Section 3.5](https://www.rfc-editor.org/rfc/rfc3986#section-3.5)
## **Connecting Inbound**
**TODO**
- Implement network request logic to determine if a network call is necessary after URI determination
-> Decide when requests should be directed to the client cache according to RFC7234

**TODO**
- Implement proxy configuration logic to handle requests that cannot be satisfied by cache
-> Implement logic for URI prefix matching or selective authority matching to determine proxy usage
-> Ensure that the client connects to the applicable proxy using the appropriate "http" or "https" URI

**TODO**
- Implement direct connection logic for requests when no proxy is applicable
-> Develop handler routine specific to the target URI's scheme to connect directly to the target resource authority

**TODO**
- Familiarize with and comply with HTTP connection management requirements as defined in RFC7230 Section 6
# Mandatory

## **Request Target**

**TODO**
- **Determine the Request Target Format**
  - Implement logic to detect and handle the four distinct formats of the request-target: origin-form, absolute-form, authority-form, and asterisk-form.
  - Depending on the HTTP method and target, decide which form applies and ensure correct parsing and handling.

- **Handle each Request Target Appropriately**
  - **Origin-Form:** Typically used by clients when connecting directly to the origin server.
  - **Absolute-Form:** Usually applies when making requests to a proxy.
  - **Authority-Form:** Used for CONNECT requests to establish a tunnel to the server.
  - **Asterisk-Form:** Used with the OPTIONS method to refer to the entire server.

- **Consider URL Resolution for Each Format**
  - Implement URL resolution logic to ensure that each format is correctly processed.
  - For **absolute-form**, ensure the entire URL is captured.
  - For **authority-form**, only capture the authority information.
  - For **asterisk-form**, check for `*` and process it accordingly.

- **Update Request Validation Logic**
  - Incorporate checks for valid format use depending on method and destination (origin vs. proxy).
  - Include proper validation and error handling—such as responding with `400 (Bad Request)` for malformed request-targets.
  - Log and handle any form-specific issues or edge cases that might arise during request processing.

- **Testing and Debugging**
  - Create tests to verify the correct handling of various request-target formats.
  - Ensure edge cases and potential ambiguities in format handling are well covered by tests.
## **Request Target: Origin-Form**
**TODO**
- Implement handling of the origin-form
- `origin-form = absolute-path [ "?" query ]`
- Ensure client sends only the absolute path and query components of the target URI as the request-target, except for CONNECT or server-wide OPTIONS requests
- If target URI's path component is empty, send "/" as the path within the origin-form
- Ensure a Host header field is included as detailed in [Section 5.4](https://www.rfc-editor.org/rfc/rfc7230#section-5.4)

**Example Implementation**
- A client retrieving http://www.example.org/where?q=now should establish a TCP connection to "www.example.org" on port 80 and send:
  - `GET /where?q=now HTTP/1.1`
  - `Host: www.example.org`
## **Request Target**
**TODO**
- Handle absolute-form requests
- `When making a request to a proxy, other than a CONNECT or server-wide OPTIONS request, a client MUST send the target URI in absolute-form as the request-target.`
- `An example absolute-form of request-line: GET http://www.example.org/pub/WWW/TheProject.html HTTP/1.1`

**TODO**
- Ensure server accepts absolute-form in requests
- `A server MUST accept the absolute-form in requests, even though HTTP/1.1 clients will only send them in requests to proxies.`
# Mandatory

## **5.3.3. Authority-Form**

### **TODO**
- Implement logic to handle CONNECT requests by extracting and using only the authority component of the target URI.
  - `When making a CONNECT request, a client MUST send only the target URI's authority component (excluding any userinfo and its "@" delimiter) as the request-target.`
  - Example: `CONNECT www.example.com:80 HTTP/1.1`
## **Request Targets**
**TODO**
- Implement handling for asterisk-form in OPTIONS requests
- `The asterisk-form of request-target is only used for a server-wide OPTIONS request`
- `When a client wishes to request OPTIONS for the server as a whole, the client MUST send only "*" (%x2A) as the request-target`
- Ensure proxies follow correct forwarding behavior with the OPTIONS method
- `If a proxy receives an OPTIONS request with an absolute-form of request-target in which the URI has an empty path and no query component, then the last proxy on the request chain MUST send a request-target of "*" when it forwards the request to the indicated origin server.`
## **Host Header**
**TODO**
- Ensure that a Host header field is sent in all HTTP/1.1 request messages
- `A client MUST send a Host header field in all HTTP/1.1 request messages.`

**TODO**
- Ensure the Host field-value matches the authority component of the target URI, excluding userinfo
- `If the target URI includes an authority component, then a client MUST send a field-value for Host that is identical to that authority component, excluding any userinfo subcomponent and its "@" delimiter.`

**TODO**
- Send Host header field with an empty field-value if the authority component is missing
- `If the authority component is missing or undefined for the target URI, then a client MUST send a Host header field with an empty field-value.`

**TODO**
- Host header should be the first header field after the request-line
- `A user agent SHOULD generate Host as the first header field following the request-line.`

**TODO**
- Replace Host header with request-target host when a proxy receives a request in absolute-form
- `When a proxy receives a request with an absolute-form of request-target, the proxy MUST ignore the received Host header field (if any) and instead replace it with the host information of the request-target.`

**TODO**
- Validate the Host field-value to prevent caching and redirection vulnerabilities
- `Since the Host header field acts as an application-level routing mechanism, ensure proper validation to guard against cache poisoning or redirection attacks.`

**TODO**
- Respond with a 400 (Bad Request) for missing or multiple Host header fields, or invalid values
- `A server MUST respond with a 400 (Bad Request) status code to any HTTP/1.1 request message that lacks a Host header field, contains more than one Host header field, or has an invalid field-value.`
## **Effective Request URI**

**TODO** 
- Construct Effective Request URI
  - Identify if the request-target is in absolute-form and use as the effective request URI.
  - If not absolute-form, construct the effective request URI using:
    - A fixed URI scheme from the server's configuration or "https" if received over a TLS-secured TCP, otherwise "http".
    - A fixed URI authority component from the server's configuration or derive it from the request-target in authority-form, the Host header, or the server's default name and port.
    - A combined path and query component derived from the request-target unless in authority-form or asterisk-form.

- **Example Implementation**:
  ```c++
  std::string construct_effective_request_uri(std::string request_target, std::string scheme, std::string host, int port, std::string path) {
      std::string effective_uri_scheme = scheme.empty() ? "http" : scheme;
      std::string effective_uri_host = host.empty() ? "default.server.name" : host;
      if (port != 80 || scheme != "http") {
          effective_uri_host += ":" + std::to_string(port);
      }
      std::string effective_request_uri = effective_uri_scheme + "://" + effective_uri_host + path;
      return effective_request_uri;
  }
  ```

- **Security Handling**:
  - Establish logic to validate if constructed URI matches configuration and connection expectations to prevent misrouting and security breaches.
  - Implement checks to handle HTTP/1.0 requests with missing Host headers, using heuristics if necessary.

___
## **Associating a Response to a Request**

**TODO**
- Implement logic to associate response messages to the correct request
- `A client that has more than one outstanding request on a connection MUST maintain a list of outstanding requests in the order sent and MUST associate each received response message on that connection to the highest ordered request that has not yet received a final (non-1xx) response.`
- Ensure order of response arrival corresponds to order of requests made on the same connection.
- Handle multiple informational (1xx) responses preceding a final response.
## **Message Forwarding**

**TODO**
- Implement Connection header handling for intermediaries
- `An intermediary not acting as a tunnel MUST implement the Connection header field, as specified in RFC 7230 Section 6.1, and exclude fields from being forwarded that are only intended for the incoming connection.`

**TODO**
- Prevent infinite request loops
- `An intermediary MUST NOT forward a message to itself unless it is protected from an infinite request loop. In general, an intermediary ought to recognize its own server names, including any aliases, local variations, or literal IP addresses, and respond to such requests directly.`

Reference: [RFC 7230 Section 2.3](https://www.rfc-editor.org/rfc/rfc7230#section-2.3)
# **Via Header Field**

## **Functionality & Requirements**

**TODO**
- Implement logic to handle "Via" header fields for both requests and responses
- A proxy MUST append an accurate Via header field in any message it forwards
- HTTP-to-HTTP gateway MUST append a Via header field in each inbound request message; it MAY append one for forwarded response messages

## **Received Protocol & By Information**

**TODO**
- Extract `received-protocol` and `received-by` information from Via header to track protocol capabilities and forwarding path

**TODO**
- Recognize absence of a port in `received-by` as indicating the default TCP port for the protocol, unless specified otherwise

**TODO**
- Handle host sensitivity by allowing pseudonyms to replace the real host in `received-by`

## **Comment Handling**

**TODO**
- Optionally generate comments in the Via header field to identify the software of each recipient; allow these comments to be stripped before forwarding for privacy

## **Handling Intermediary & Firewall**

**TODO**
- Ensure that intermediaries used as portals through network firewalls do not forward internal host details unless explicitly enabled; use pseudonyms if not enabled

## **Combining Via Entries**

**TODO**
- Allow for combining ordered subsequences of Via entries that share identical `received-protocol` values; ensure combination only occurs under the same organizational control and with hosts replaced by pseudonyms
- Avoid combining entries with different `received-protocol` values

---

**Example**
- Track intermediaries in the following manner:
  - Initial request through an internal proxy (e.g., "fred") to a public proxy (e.g., "p.example.net"): 
  - Resulting in a consolidated Via header at final destination (e.g., "www.example.com"):
    - `Via: 1.0 fred, 1.1 p.example.net`

Implement these tasks to align with the specifications outlined in [RFC 7230 Section 5.7.1](https://www.rfc-editor.org/rfc/rfc7230#section-5.7.1).
# Message Transformation Proxy Handling

## **Transforming Proxy Logic**
**TODO**
- Implement logic for transforming proxy that might include conversion between image formats or other transformations:
  - **Function**: Apply transformations to message payloads intended to save cache space or reduce traffic.

**Proxy Request and Forwarding Rules**
**TODO**
- Do not change the `host` name in a request-target if it contains a fully qualified domain name, otherwise you may append your domain.
- `proxy_host_handling(request) {}`

**Request-target Handling**
**TODO**
- Ensure the `absolute-path` and `query` parts of the request-target are forwarded without modification, except for empty path replacements.
- `proxy_path_query_handling(request) {}`

**Message Body Modification Rules**
**TODO**
- Apply or remove transfer coding when transforming the message body, if needed.
  
**No-transform Cache-control Directive Handling**
**TODO**
- Ensure payload of messages with a `no-transform` cache-control directive is never transformed.

**Transformation Notification**
**TODO**
- Add a `Warning` header field with a warn-code of 214 ("Transformation Applied") if the payload is transformed and the warning is not already present.

**Status Code Handling for Payload Transformation**
**TODO**
- If transforming the payload of a 200 (OK) response, consider changing status code to 203 (Non-Authoritative Information) to signal transformation application.

**Header Field Modification Rules**
**TODO**
- Refrain from modifying header fields that provide information about endpoints, resource state, or selected representation unless modification is allowed for privacy or security reasons.
## **Connection Management**

**TODO**
- Implement connection management logic
- `HTTP implementations are expected to engage in connection management, which includes maintaining the state of current connections, establishing a new connection or reusing an existing connection, processing messages received on a connection, detecting connection failures, and closing each connection.`

**TODO**
- Support multiple parallel connections
- `Most clients maintain multiple connections in parallel, including more than one connection per server endpoint. Most servers are designed to maintain thousands of concurrent connections, while controlling request queues to enable fair use and detect denial-of-service attacks.`

**TODO**
- Implement connection failure detection
- `HTTP messaging is independent of the underlying transport- or session-layer connection protocol(s). HTTP only presumes a reliable transport with in-order delivery of requests and corresponding in-order delivery of responses. Connection failures should be detected and handled appropriately.`

**TODO**
- Control request queues for fairness and detect denial-of-service attacks
- `Server designs must handle thousands of concurrent connections while controlling request queues to enable fair use and detect denial-of-service attacks.`
## **Connection Header**

**TODO**
- Implement logic to handle `Connection` header field
- Ensure the removal or replacement of any received connection options before forwarding the message.
- `A proxy or gateway MUST parse a received Connection header field before forwarding a message, remove any header field(s) from the message corresponding to each connection-option, and then remove or replace the Connection header field itself.`

**TODO**
- Develop mechanism to distinguish hop-by-hop headers from end-to-end headers
- `The Connection header field provides a declarative way of distinguishing header fields that are intended only for the immediate recipient ("hop-by-hop") from those fields intended for all recipients in the chain ("end-to-end").`

**TODO**
- Ensure connection options do not correspond to end-to-end header fields
- `A sender MUST NOT send a connection option corresponding to a header field that is intended for all recipients of the payload.`

**TODO**
- Handle `close` connection option
- Implement logic for the `close` option to signal connection closure after a response.
- `Connection: close` signifies the closure of the connection after the current response.
- A non-persistent client must send `Connection: close` in every request.
- A non-persistent server must send `Connection: close` in every response unless it's a 1xx status code.
## **Connection Establishment**

**TODO**
- Clarify the scope of our implementation regarding connection establishment
- Discuss with the team the required transport-layer protocols and the management of each connection as a single transport link.
- Confirm adherence to our project's scope and boundaries as this is beyond the scope of RFC 7230.
- Ensure connection-related logic aligns with specific transport protocol documentation if applicable.
## **HTTP Persistent Connections**

**TODO**
- Implement persistent connection logic
- `HTTP/1.1 defaults to the use of "persistent connections", allowing
   multiple requests and responses to be carried over a single
   connection.  The "close" connection option is used to signal that a
   connection will not persist after the current request/response.  HTTP
   implementations SHOULD support persistent connections.`

**TODO**
- Determine connection persistence status based on Connection header and HTTP version
- `A recipient determines whether a connection is persistent or not
   based on the most recently received message's protocol version and
   Connection header field (if any):
     - If the "close" connection option is present, the connection will
       not persist after the current response; else,
     - If the received protocol is HTTP/1.1 (or later), the connection
       will persist after the current response; else,
     - If the received protocol is HTTP/1.0, the "keep-alive" connection
       option is present, the recipient is not a proxy, and the recipient
       wishes to honor the HTTP/1.0 "keep-alive" mechanism, the
       connection will persist after the current response; otherwise,
     - The connection will close after the current response.`

**TODO**
- Ensure self-defined message lengths for persistence and handle connection closure
- `In order to remain persistent, all messages on a connection need to
   have a self-defined message length (i.e., one not defined by closure
   of the connection), as described in Section 3.3.`

**TODO**
- Read entire request and response bodies to maintain connection integrity
- `A server MUST read the entire request message body or close the
   connection after sending its response, since otherwise the remaining
   data on a persistent connection would be misinterpreted as the next
   request. Likewise, a client MUST read the entire response message
   body if it intends to reuse the same connection for a subsequent request.`

**TODO**
- Prohibit persistent connections for HTTP/1.0 clients
- `A proxy server MUST NOT maintain a persistent connection with an
   HTTP/1.0 client (see Section 19.7.1 of [RFC2068] for information and
   discussion of the problems with the Keep-Alive header field
   implemented by many HTTP/1.0 clients).`
- Reference Appendix A.1.2 for more information on HTTP/1.0 compatibility: https://www.rfc-editor.org/rfc/rfc7230#appendix-A.1.2
## **Retrying Requests**
### Request Retry Logic
**TODO**
- Implement recovery logic for asynchronous connection closures
  - `Connections can be closed at any time, with or without intention. Implementations ought to anticipate the need to recover from asynchronous close events.`

**TODO**
- Automatically retry idempotent requests on premature connection closure
  - `When an inbound connection is closed prematurely, a client MAY open a new connection and automatically retransmit an aborted sequence of requests if all of those requests have idempotent methods.`

**TODO**
- Ensure proxy adherence to request retry guidelines
  - `A proxy MUST NOT automatically retry non-idempotent requests.`

**TODO**
- Determine and handle request idempotency for user agents
  - `A user agent MUST NOT automatically retry a request with a non-idempotent method unless it has some means to know that the request semantics are actually idempotent, regardless of the method, or some means to detect that the original request was never applied.`

**TODO**
- Avoid multiple automatic retries for request failures
  - `A client SHOULD NOT automatically retry a failed automatic retry.`

### References
- [RFC 7231 Section 4.2.2: Idempotent methods](https://www.rfc-editor.org/rfc/rfc7231#section-4.2.2)
## **Pipelining**

**TODO** 
- Implement support for HTTP/1.1 persistent connections
- Ensure that requests with safe methods can be processed in parallel
- Ensure that responses are sent in the same order as requests when pipelining

**TODO**
- Ensure the client retries unanswered requests if a connection closes prematurely
- Avoid pipelining immediately after a new connection if requests in the previous pipeline might have failed
- Handle the TCP reset problem as described in [Section 6.6](https://www.rfc-editor.org/rfc/rfc7230#section-6.6)

**TODO**
- Disable pipelining after a non-idempotent method until receiving the final response status code
- Implement a mechanism to detect and recover from partial failure conditions in pipelined sequences

**TODO**
- For intermediaries handling pipelined requests:
  - Implement retry logic for sequences of requests with idempotent methods if the connection fails
  - Forward any received responses before closing connections, allowing outbound user agents to recover
## **Concurrency Management**

**TODO**
- Implement connection management logic to control the number of simultaneous open connections to a given server.
- `A client ought to limit the number of simultaneous open connections that it maintains to a given server.`
- Aim to avoid excessive open connections to prevent potential rejection by the server or unintentional denial-of-service indications. Be conservative while managing these connections and ensure efficiency in handling server resources.
- Consider strategies to mitigate "head-of-line blocking" while balancing the number of open connections to optimize both client-side performance and network congestion.
## **Failures and Timeouts**

**TODO**
- Implement connection timeout logic
- `Servers will usually have some timeout value beyond which they will no longer maintain an inactive connection. A client/server MAY close the transport connection at any time.`

**TODO**
- Implement graceful close on timeout
- `A client or server that wishes to time out SHOULD issue a graceful close on the connection. Implementations SHOULD constantly monitor open connections for a received closure signal and respond to it as appropriate.`

**TODO**
- Implement logic to monitor and manage persistent connections
- `A server SHOULD sustain persistent connections, when possible, and allow the underlying transport's flow-control mechanisms to resolve temporary overloads, rather than terminate connections with the expectation that clients will retry.`

**TODO**
- Monitor network connection when sending message body
- `A client sending a message body SHOULD monitor the network connection for an error response while transmitting. If the client sees a response that indicates the server does not wish to receive the message body and is closing the connection, the client SHOULD immediately cease transmitting the body and close its side of the connection.`
## **Connection Management**

**TODO**
- Handle `Connection` header field for connection closure  
- The `Connection` header field should recognize and process the "close" connection option as follows:
  - A sender that sends a "close" option MUST NOT send further requests and MUST close the connection after reading the final response.
  - A server receiving a "close" option MUST initiate closing the connection after sending the final response and SHOULD send a "close" option in its response.
  - A client receiving a "close" option MUST stop sending requests and close the connection after reading the response.

- Implement staged connection teardown to avoid TCP reset problems  
- To avoid potential TCP reset issues during connection teardown:
  - Perform a half-close by closing only the write side of the connection.
  - Continue reading from the connection until either the client performs a corresponding close or the server confirms the client's acknowledgment of its last response.
  - Fully close the connection once the above conditions are met. 

- Investigate whether the TCP reset issue might also be present in other transport protocols.
## **Upgrade Header Field**
**TODO**
- Implement support for `Upgrade` header field in requests
  - Allow clients to send a list of protocols via the `Upgrade` header to invite protocol switching
  - Process and possibly ignore the `Upgrade` field if the server opts to continue with the current protocol

**TODO**
- Implement protocol switching logic
  - Handle 101 (Switching Protocols) response
  - Ensure server sends an `Upgrade` header indicating the new protocol(s) upon sending 101 response
  - Confirm that protocol switching honors received message semantics

**TODO**
- Handle 426 (Upgrade Required) response
  - Implement logic to send an `Upgrade` header indicating acceptable protocols

**TODO**
- Advertise protocol upgrade capabilities in server responses
  - When relevant, include an `Upgrade` header in responses to indicate supported protocol upgrades for future requests

**TODO**
- Incorporate necessary headers during upgrade
  - Ensure an `Connection` header with "upgrade" option is sent along with an `Upgrade` header to prevent unintentional forwarding by intermediaries that may not support the listed protocols

**TODO**
- Define initial protocol name "HTTP" for use
  - Reference applicable HTTP version rules and updates for protocol naming conventions within the `Upgrade` implementation

**Best Practice**
- Ignore `Upgrade` header in HTTP/1.0 requests
- Send a 100 (Continue) response before a 101 response if both `Upgrade` and `Expect: 100-continue` headers are present
# Mandatory
## **ABNF List Extension**
**TODO**
- Implement support for parsing #rule constructs
- `A construct "#" is defined for defining comma-delimited lists of elements. Read RFC 7230 section 7 for detailed examples and rules.`

**TODO**
- Enforce rule: sender MUST NOT generate empty list elements 
- `Ensure lists follow the syntax: 1#element => element *( OWS "," OWS element )`

**TODO**
- Support parsing and ignoring a reasonable number of empty list elements
- `A recipient MUST accept lists that satisfy the syntax: #element => [ ( "," / element ) *( OWS "," [ OWS element ] ) ]`
- `Implement logic to ignore non-contributing empty elements, as outlined in the examples provided in RFC 7230 section 7.`
## **IANA Considerations**

**TODO**
- Review and ensure proper usage of registered protocol parameters. 
- `This document defines several new protocol parameters within the "Hypertext Transfer Protocol (HTTP) Parameters" registry provided by IANA, including HTTP status codes, methods, and header fields.`
- Keep track of changes or updates in the IANA HTTP parameter registries to ensure continued compliance with standard protocol parameters.

**TODO**
- Register any new custom methods, status codes, or header fields if necessary.
- `Any new method or status code introduced by the implementation that is not yet registered with IANA should be registered following the procedures defined by IANA.`

**TODO**
- Handle unregistered or experimental HTTP protocol parameters cautiously.
- `Stay vigilant about the use of unregistered or experimental methods, status codes, and headers. Consider potential compatibility and security implications when encountering or using such elements.`

**References**
- [IANA HTTP Status Code Registries](https://www.iana.org/assignments/http-status-codes/)
- [IANA HTTP Method Registries](https://www.iana.org/assignments/http-methods/)
- [IANA HTTP Header Field Registries](https://www.iana.org/assignments/message-headers/)
# Header Field Registration

HTTP header fields are registered within the "Message Headers" registry maintained at [IANA Message Headers](http://www.iana.org/assignments/message-headers/).

**TODO** 
- Ensure all relevant HTTP header fields are registered and maintained according to the registry.
- Pay special attention to the standard header fields defined in the document and update the "Permanent Message Header Field Names" registry as needed.

## HTTP Standard Header Fields
**Header Field Name** | **Protocol** | **Status** | **Reference**
----------------------|--------------|------------|------------
Connection            | http         | standard   | [Section 6.1](https://www.rfc-editor.org/rfc/rfc7230#section-6.1)
Content-Length        | http         | standard   | [Section 3.3.2](https://www.rfc-editor.org/rfc/rfc7230#section-3.3.2)
Host                  | http         | standard   | [Section 5.4](https://www.rfc-editor.org/rfc/rfc7230#section-5.4)
TE                    | http         | standard   | [Section 4.3](https://www.rfc-editor.org/rfc/rfc7230#section-4.3)
Trailer               | http         | standard   | [Section 4.4](https://www.rfc-editor.org/rfc/rfc7230#section-4.4)
Transfer-Encoding     | http         | standard   | [Section 3.3.1](https://www.rfc-editor.org/rfc/rfc7230#section-3.3.1)
Upgrade               | http         | standard   | [Section 6.7](https://www.rfc-editor.org/rfc/rfc7230#section-6.7)
Via                   | http         | standard   | [Section 5.7.1](https://www.rfc-editor.org/rfc/rfc7230#section-5.7.1)

**TODO**
- Handle the "Close" header field name which is reserved.
  - The header field-name "Close" is registered as "reserved" since using that name as an HTTP header field might conflict with the "close" connection option of the Connection header field ([Section 6.1](https://www.rfc-editor.org/rfc/rfc7230#section-6.1)).

## Reserved Header Field
**Header Field Name** | **Protocol** | **Status** | **Reference**
----------------------|--------------|------------|------------
Close                 | http         | reserved   | [Section 8.1](https://www.rfc-editor.org/rfc/rfc7230#section-8.1)

**TODO**
- Coordinate with the change controller, IETF (iesg@ietf.org), if updates or changes to the registry are necessary.
  
Remember: Proper registration and adherence to registry guidelines are crucial for maintaining protocol consistency and preventing conflicts in header field usage.
## **URI Scheme Registration**
**TODO**
- Track updates to the Permanent URI Schemes registry
- Follow URI Schemes listed under "Permanent URI Schemes" registry, specifically "http" and "https"
  - For details on each scheme refer to sections: 
    - [Section 2.7.1](https://www.rfc-editor.org/rfc/rfc7230#section-2.7.1) for "http"
    - [Section 2.7.2](https://www.rfc-editor.org/rfc/rfc7230#section-2.7.2)

**TODO**
- Ensure application conforms to URI scheme registration guidelines 
- Reference official IANA URI Scheme assignments: [IANA URI Schemes Registry](http://www.iana.org/assignments/uri-schemes/) 

**TODO**
- Monitor for new URI schemes that may impact application function  
- Regularly review updates to BCP115 guidelines regarding URI scheme registration and apply changes accordingly.
## **Internet Media Type Registration**
**TODO**
- Ensure compliance with IANA standards for media types
- `Reference BCP13 for media type specifications and registration procedures`
- ` http://www.iana.org/assignments/media-types`

**TODO**
- Implement support for the Internet media types "message/http" and "application/http"
- `This document serves as the specification for the Internet media types "message/http" and "application/http"`
## **Internet Media Type: message/http**

### **TODO**
- Implement handling for `media type message/http`
  - Enclose a single HTTP request or response message in compliance with MIME restrictions.
  - Adhere to line length and encoding limitations: support only "7bit", "8bit", or "binary" encodings.

### **Required Parameters**
- None

### **Optional Parameters**
- **version**: Implement logic to parse or default to HTTP-version from the first line if not specified.
- **msgtype**: Implement logic to determine or default to message type (either "request" or "response") from the first line if not specified.

### **Security Considerations**
- Review [Section 9 of RFC7230](https://www.rfc-editor.org/rfc/rfc7230#section-9) and incorporate relevant security mechanisms.

### **Additional Information**
- Understand that this specification does not include magic numbers, deprecated alias names, file extensions, or Macintosh file type codes.
- Stay updated with any changes dictated by the IESG.

---

### **Action Items**
- Integrate message/http media type support ensuring to handle optional parameters and security considerations.
- Conduct testing to ensure encoding adherence ("7bit", "8bit", or "binary").

---

This section ensures that our application can correctly process HTTP messages encapsulated as media type message/http, complying with all relevant RFC specifications.
# TODO: Internet Media Type application/http

## **Parsing HTTP Messages**
- **Function**: Parse the HTTP pipeline for request or response messages 
- `TODO: Implement parser to correctly distinguish between 'request' and 'response' from the first line of the body if 'msgtype' parameter is not present.`

## **Handling HTTP Versioning**
- **Action**: Determine HTTP version from the first line of the body if the 'version' parameter is missing
- `TODO: Implement logic to extract HTTP-version number based on the structure of the first line in the body as a fallback when 'version' parameter is absent.`

## **Content Transfer Encoding**
- **TODO**: Implement appropriate Content-Transfer-Encoding when HTTP messages are transmitted via email
- `The "application/http" type requires that HTTP messages are in "binary" format. Implement necessary logic to handle Content-Transfer-Encoding during email transmission.`

## **Security Considerations**
- **TODO**: Review Section 9 for security considerations regarding the use of "application/http" media type
- `https://www.rfc-editor.org/rfc/rfc7230#section-9`

## **Documentation and Usage Guidelines**
- **Action Item**: Ensure documentation is consistent with published specifications as per the section
- `https://www.rfc-editor.org/rfc/rfc7230#section-8.3.2`

This set of actionable items aims to extract relevant information for handling `application/http` media types in HTTP requests and responses pipeline efficiently, with a focus on parsing, versioning, encoding, and adhering to security specifications.
# Mandatory

## **Transfer Encoding**

**TODO**
- Implement handling for Transfer Coding 
- ` The "HTTP Transfer Coding Registry" defines the namespace for transfer coding names. Ensure to refer to the registry for valid names and specifications: http://www.iana.org/assignments/http-parameters `

**TODO**
- Recognize and correctly handle "chunked" transfer coding 
- `   When Transfer-Encoding includes "chunked" as a transfer coding, the data transfer length is determined by reading and decoding the chunked data until reaching the last chunk, which is denoted by a zero-length chunk. `

**TODO**
- Ensure compliance with transfer coding rules
- ` The chunked transfer coding is a method in HTTP/1.1 for ensuring that the receiver can reliably detect the end of message body and de-chunk it correctly. Implement logic to manage and enforce this.`  

**TODO**
- Implement error handling for invalid transfer codings 
- ` A recipient that does not recognize or cannot comply with the transfer coding MUST send a 501 (Not Implemented) status code and then close the connection. `

___

# Bonus

## **Transfer Coding Registry Reference** 

**TODO** 
- Regularly check for updates in the HTTP Transfer Coding Registry
- ` Keep the implementation updated with potential new transfer codings that might be registered. Periodically verify with the registry at http://www.iana.org/assignments/http-parameters `
# Mandatory

## **Transfer Coding Registration Procedure**
**TODO**
- Define and maintain a registration procedure following RFC guidelines
  - Include:
    - Name of the transfer coding
    - Description
    - Pointer to specification text
  
**TODO**
- Ensure transfer coding names do not overlap with content coding names unless encoding transformation is identical
  - Refer to [Section 3.1.2.1 of RFC7231](https://www.rfc-editor.org/rfc/rfc7231#section-3.1.2.1) and [Section 4.2 of RFC7230](https://www.rfc-editor.org/rfc/rfc7230#section-4.2).

**TODO**
- Only add new values to the namespace with IETF Review
  - Must conform to the purpose of transfer coding as defined in this specification
  - Refer to [Section 4.1 of RFC5226](https://www.rfc-editor.org/rfc/rfc5226#section-4.1).

**TODO**
- Discourage use of program names for identifying encoding formats in future encodings

___
___

# Bonus

## **Handling Transfer Codings**

**TODO**
- Implement checks to prevent overlap in coding names and transformations
- Develop guidelines ensuring seamless integration of new transfer codings following registrations

---

This section ensures that transfer coding registrations are handled appropriately, as per the specified requirements, while avoiding overlaps with content codings unless absolutely necessary.
# HTTP Transfer Coding

## **Transfer Coding Registry**

The "HTTP Transfer Coding Registry" has been updated with the registrations mentioned below. Ensure that our implementation supports and handles these transfer codings correctly.

**TODO**
- Implement logic to handle transfer encoding for each type listed.

| Name       | Description                                    | Reference                                         |
|------------|------------------------------------------------|---------------------------------------------------|
| chunked    | Transfer in a series of chunks                 | [Section 4.1](https://www.rfc-editor.org/rfc/rfc7230#section-4.1) |
| compress   | UNIX "compress" data format                    | [Section 4.2.1](https://www.rfc-editor.org/rfc/rfc7230#section-4.2.1) |
| deflate    | "deflate" compressed data                      | [Section 4.2.2](https://www.rfc-editor.org/rfc/rfc7230#section-4.2.2) ([RFC1951](https://www.rfc-editor.org/rfc/rfc1951)) inside the "zlib" data format ([RFC1950](https://www.rfc-editor.org/rfc/rfc1950)) |
| gzip       | GZIP file format                               | [Section 4.2.3](https://www.rfc-editor.org/rfc/rfc7230#section-4.2.3) ([RFC1952](https://www.rfc-editor.org/rfc/rfc1952)) |
| x-compress | Deprecated (alias for compress)                | [Section 4.2.1](https://www.rfc-editor.org/rfc/rfc7230#section-4.2.1) |
| x-gzip     | Deprecated (alias for gzip)                    | [Section 4.2.3](https://www.rfc-editor.org/rfc/rfc7230#section-4.2.3) |

**Action Items**
1. Implement support for each transfer coding as per their respective sections.
2. Recognize and appropriately handle deprecated transfer codings (`x-compress` and `x-gzip`).
3. Verify that our handling complies with the requirements of each specific section referenced above.

_Adjust as per our system architecture and ensure compatibility across all modules. This task is crucial for maintaining compliance with RFC 7230._
## **Content Coding Registration**
### **TODO**
- Implement logic to handle registered content codings.
- Existing content codings:
  - **compress**: UNIX "compress" data format ([Welch](https://www.rfc-editor.org/rfc/rfc7230#ref-Welch)).
    - Reference: [Section 4.2.1](https://www.rfc-editor.org/rfc/rfc7230#section-4.2.1)
  - **deflate**: "deflate" compressed data ([RFC1951](https://www.rfc-editor.org/rfc/rfc1951)) inside the "zlib" data format ([RFC1950](https://www.rfc-editor.org/rfc/rfc1950)).
    - Reference: [Section 4.2.2](https://www.rfc-editor.org/rfc/rfc7230#section-4.2.2)
  - **gzip**: GZIP file format ([RFC1952](https://www.rfc-editor.org/rfc/rfc1952)).
    - Reference: [Section 4.2.3](https://www.rfc-editor.org/rfc/rfc7230#section-4.2.3)
  - **x-compress**: Deprecated (alias for compress).
    - Reference: [Section 4.2.1](https://www.rfc-editor.org/rfc/rfc7230#section-4.2.1)
  - **x-gzip**: Deprecated (alias for gzip).
    - Reference: [Section 4.2.3](https://www.rfc-editor.org/rfc/rfc7230#section-4.2.3) 

- Ensure to check the "HTTP Content Coding Registry" at [IANA](http://www.iana.org/assignments/http-parameters) for updates.


## **Upgrade Token Registry**
**TODO**
- Implement support for the "Upgrade" header field.
- `The "Upgrade" header field is used to indicate a desired protocol to
   which the server can switch after completing the current request/response exchange.`
- Ensure compatibility with protocols listed in the "Hypertext Transfer Protocol (HTTP) Upgrade Token Registry".
- Reference the registry for supported protocol-name tokens: [HTTP Upgrade Tokens](http://www.iana.org/assignments/http-upgrade-tokens).
- Consider security and compatibility implications when enabling protocol upgrades.
## **Protocol Registration**
**TODO** 
- Implement the procedure for registering and handling protocol names aligned with these rules:
  - Ensure protocol-name token registration remains permanent upon successful registration.
  - Assign a responsible party and a point of contact for each protocol-name registration.
  - Optionally document a set of specifications associated with the token; public availability isn't mandatory.
  - Record and manage a set of expected "protocol-version" tokens linked to each registered name, if any, at the time of registration.
  - Allow the responsible party to amend the registration while maintaining a historical record of changes in collaboration with IANA.
  - Be prepared for the IESG to potentially reassign responsibility for a protocol token if necessary, especially when the responsible party is unreachable. 

- Make sure to adhere to the updated registration procedure of HTTP Upgrade Tokens, replacing the previous version established by RFC2817 Section 7.2.
## **Upgrade Token Registration**
**TODO**
- Implement upgrade token registration validation
- Ensure support for "HTTP" entry with version tokens in the format "DIGIT.DIGIT" (e.g., "2.0")
- Reference Section 2.6 for further details: [Section 2.6](https://www.rfc-editor.org/rfc/rfc7230#section-2.6)

**TODO**
- Maintain contact with responsible party for updates 
- "IETF (iesg@ietf.org) - Internet Engineering Task Force"

___
___
# Security Considerations

## **HTTP Message Syntax and Parsing**

**TODO**
- Validate header field lengths
- `Limit the length of each header field to mitigate buffer overflow attacks`

**TODO**
- Ensure robust error handling
- `Implement comprehensive error handling mechanisms to prevent malformed 
   requests from exploiting system vulnerabilities`

**TODO**
- Handle malformed or malicious request inputs
- `Implement input validation and sanitation to protect against injection 
   attacks and other vulnerabilities`

**TODO**
- Implement TLS for secure communications
- `Ensure transport layer security through the implementation of TLS to 
   protect data integrity and confidentiality`

## **Routing Concerns**

**TODO**
- Vet HTTP request paths
- `Validate and sanitize request paths to prevent path traversal attacks`

**TODO**
- Enforce strict host validation
- `Implement checks to ensure that requests are only accepted from trusted hosts to 
   prevent DNS rebinding attacks`

**TODO**
- Limit sensitive information exposure
- `Limit the amount and type of information revealed in HTTP headers to reduce 
   the risk of information leakage`
# Authority Establishment

## **Establishing Authority**

**TODO**
- Ensure authoritative responses are properly identified
  - `HTTP relies on the notion of an authoritative response: a response that has been determined by (or at the direction of) the authority identified within the target URI.`

**TODO**
- Implement mechanisms to mitigate phishing attack risks
  - `User agents can reduce the impact of phishing attacks by enabling users to easily inspect a target URI prior to making an action, by prominently distinguishing userinfo when present, and by not sending stored credentials and cookies when the referring document is from an unknown or untrusted source.`

**TODO**
- Investigate and strengthen defenses against DNS and network-based attacks on authority
  - `The user's choice of server for Domain Name Service (DNS) and the hierarchy of servers from which it obtains resolution results could impact the authenticity of address mappings; DNS Security Extensions (DNSSEC) are one way to improve authenticity.`

**TODO**
- Securely implement HTTPS to validate server authentication
  - `The "https" scheme is intended to prevent many of these potential attacks on establishing authority, provided that the negotiated TLS connection is secured and the client properly verifies that the communicating server's identity matches the target URI's authority component.`

- [RFC2818 - HTTP Over TLS](https://www.rfc-editor.org/rfc/rfc2818)
- Refer to [Georgiev - The Most Dangerous Code in the World: Validating SSL Certificates in Non-browser Software](https://www.rfc-editor.org/rfc/rfc7230#ref-Georgiev) for best practices.
# Intermediary Security

## **Risks of Intermediaries**
**TODO**
- Assess the security configuration and default settings for intermediaries.
- Ensure that the implementation of intermediaries considers privacy and security concerns.
- Inform users about the trust level of intermediaries, highlighting that their security is only as reliable as the operators.
- Implement and review measures to mitigate cache poisoning attacks, especially for intermediaries with shared caches. Reference: [RFC7234 Section 8](https://www.rfc-editor.org/rfc/rfc7234#section-8).
## **Security Considerations**

### **Protocol Element Length**

**TODO**
- Implement request-target length checks
  - Reject messages with excessively long request-targets. Respond with `414 (URI Too Long)` status code if necessary (RFC7231 Section 6.5.12).

**TODO**
- Implement payload length checks
  - Reject request payloads exceeding acceptable size. Respond with `413 (Payload Too Large)` status code if necessary (RFC7231 Section 6.5.11).

**TODO**
- Limit processing of protocol elements
  - Set maximum acceptable lengths for processing request methods, response status phrases, header field-names, numeric values, and body chunks to avoid buffer and arithmetic overflows.

**TODO**
- Monitor data streams
  - Identify and mitigate potential denial-of-service attacks by monitoring very long or slow data streams, especially in absence of predefined element lengths. 

**References**
- [RFC 7230 Section 3.1.1](https://www.rfc-editor.org/rfc/rfc7230#section-3.1.1)
- [RFC 7230 Section 3.2](https://www.rfc-editor.org/rfc/rfc7230#section-3.2)
- [RFC 7231 Section 6.5.12](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.12)
- [RFC 7231 Section 6.5.11](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.11)
- [RFC 6585](https://www.rfc-editor.org/rfc/rfc6585)
# Security
## **Response Splitting**
**TODO**
- Implement Response Splitting Defense Mechanisms
- **Filter Requests:** Inspect incoming requests for encoded CR (`%0D`) and LF (`%0A`) characters. Ensure these characters are not included in any parameters that are later echoed back in the response.
- **Restrict Outputs:** Ensure only core protocol libraries can send CR or LF characters in header sections. Allow application servers to interact with headers only through APIs designed to filter out unauthorized control characters.
- **Proper Encoding Practices:** Make sure any data, especially those derived from external inputs, is properly encoded before being placed into response headers.

- Further Reading: https://www.rfc-editor.org/rfc/rfc7230#ref-Klein
# Security Considerations

## **Request Smuggling**

**TODO**
- Implement strict request parsing to prevent request smuggling
  - Ensure adherence to the message framing requirements outlined in [Section 3.3.3](https://www.rfc-editor.org/rfc/rfc7230#section-3.3.3)
  - [RFC7230 - Linhart: Request Smuggling](https://www.rfc-editor.org/rfc/rfc7230#ref-Linhart)

**Action Items**
- Analyze protocol parsing consistency across deployment and software layers
- Identify any discrepancies that may enable request smuggling
- Test with various malformed requests to ensure parser robustness

**Recommended Practices**
- Conduct periodic security audits focusing on security issues like request smuggling
- Keep abreast with HTTP security advisories and updates

This section of the RFC underscores the need for rigorous parsing techniques to reduce vulnerabilities associated with HTTP request smuggling attacks.
# Message Integrity

## **HTTP Message Integrity**

**TODO**
- Evaluate methods to ensure HTTP message integrity leveraging existing transport protocols
- `HTTP relies on the error-detection ability of underlying transport protocols and length or chunk-delimited framing to detect message completeness and integrity.`

**TODO**
- Explore and potentially implement additional integrity mechanisms such as hash functions or digital signatures
- `Additional integrity mechanisms can be added to HTTP messages via extensible metadata header fields to verify message integrity.`

**TODO**
- Encourage and develop user agents to implement configurable mechanisms for detecting and reporting failures of message integrity
- `User agents should provide mechanisms that can be enabled in environments where message integrity is crucial, such as detecting when information is incomplete, expired, or corrupted during transfer.`

**TODO**
- Develop an indication mechanism within user agents for users to distinguish between complete and incomplete HTTP response messages
- `At a minimum, user agents should have indications to differentiate between complete and incomplete response messages per Section 3.4 of RFC 7230.`

**TODO**
- Research and identify user agent extensions or features to enable message integrity verification based on the presence of integrity metadata in responses

## **Metadata Header Fields**

**TODO**
- Define potential uses of metadata header fields for conveying message integrity information
- `Consider using metadata header fields to transmit hash values or signatures ensuring the integrity and authenticity of the message content.`
## **Message Confidentiality**

**TODO**
- Ensure transport protocol selection aligns with confidentiality requirements
- Verify that HTTP communications use a suitable transport protocol, such as HTTPS, for resources requiring confidentiality. Adapt program logic to check the URI scheme (e.g., HTTP vs HTTPS) to determine if a confidential connection is needed.

- **Implement User Agent Configuration for Transport Protocols**
  - Provide options in the user agent settings to configure and enforce the selection of desired secure transport protocols like TLS (Transport Layer Security) when dealing with HTTP communications.
 
- **Update Documentation Regarding Security Protocols**
  - Document the process and protocols used for securing HTTP messages in application guides, emphasizing the independence of HTTP from the transport layer, and how to leverage the chosen transport for enhanced security.

- **Review the Use of "http" vs "https" Schemes**
  - Regularly audit code and configuration to ensure "https" is used for all sensitive requests to prevent transmission of data in clear text.
## **Server Log Privacy**
The handling of server log information is crucial due to its confidential nature and potential to identify personal data about users. It's important to maintain compliance with laws and regulations when storing and analyzing log information.

**TODO**
- Implement anonymization techniques for server logs to protect personal data.
- Ensure the secure storage of HTTP log information and follow appropriate guidelines for its analysis.
- Purge personally identifiable information from logs, such as user identifiers, IP addresses, and user-provided query parameters, once they're no longer needed for operational purposes such as security, auditing, or fraud control.
- Monitor and comply with relevant laws and regulations concerning the privacy and handling of log data to protect user privacy effectively.


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

---
---
# Optional
## **Incomplete Request Message ** 
**TODO**
- Implement optional error response for incomplete request messages
  - `A server that receives an incomplete request message, usually due to a canceled request or a triggered timeout exception, MAY send an error response prior to closing the connection.`

## **Message Parsing Robustness**

**TODO**

- **Pre-Request Line Handling**: Implement logic to ignore at least one empty line (CRLF) received prior to the request line in order to accommodate older HTTP/1.0 user agents that might send an extra CRLF after a POST request.

- **Single LF Recognition**: Allow the message parser to recognize a single LF as a line terminator and ignore any preceding CR. This enhances robustness in handling messages from sources that might not adhere strictly to the standard CRLF requirement. 

- **Whitespace Flexibility**: Improve the parser to accept any form of whitespace as the separator between component elements in the request-line and status-line. This includes handling SP, HTAB, VT (%x0B), FF (%x0C), or bare CR as potential separators.

- **Robustness Exceptions**: Ensure that the server, while being lenient with robustness exceptions, still responds with a 400 (Bad Request) when it receives a sequence of octets that does not align with the HTTP message grammar and is not covered by the allowed robustness exceptions.

- **Security Considerations**: Review and properly handle lenient parsing practices to avoid security vulnerabilities when processing messages that might be interpreted differently by multiple recipients (see Section 9.5 for detailed recommendations).

**NOTE**: While building these functionalities, be mindful of the potential security implications of lenient parsing and always prioritize maintaining adherence to security best practices in HTTP message processing.