## Introduction

**TODO**
- Familiarize yourself with the purpose and high-level architecture of HTTP
- Review RFC7230 for detailed information on message syntax and routing: [RFC7230](https://www.rfc-editor.org/rfc/rfc7230)

**TODO**
- Understand the uniform interface provided by HTTP for interacting with resources: [Section 2](https://www.rfc-editor.org/rfc/rfc7231#section-2)

**TODO**
- Learn about HTTP request methods and their associated intentions: [Section 4](https://www.rfc-editor.org/rfc/rfc7231#section-4)

**TODO**
- Investigate the extension of semantics through request header fields: [Section 5](https://www.rfc-editor.org/rfc/rfc7231#section-5)

**TODO**
- Comprehend the meaning of status codes for machine-readable responses: [Section 6](https://www.rfc-editor.org/rfc/rfc7231#section-6)

**TODO**
- Explore the representation metadata describing the payload interpretation: [Section 3](https://www.rfc-editor.org/rfc/rfc7231#section-3)

**TODO**
- Examine the concept of content negotiation and selection algorithms: [Section 3.4](https://www.rfc-editor.org/rfc/rfc7231#section-3.4)
## **Conformance and Error Handling**

**TODO**
- Ensure interpretations align with RFC 2119 keyword definitions
- `Key words "MUST", "MUST NOT", "REQUIRED", "SHALL", "SHALL NOT", "SHOULD", "SHOULD NOT", "RECOMMENDED", "MAY", and "OPTIONAL" should be interpreted as described in RFC2119.`

**TODO**
- Establish error-handling protocol in line with Section 2.5 of RFC 7230
- https://www.rfc-editor.org/rfc/rfc7230#section-2.5

Ensure that all team members understand the conformance criteria and error handling procedures delineated in these sections to maintain compliance with HTTP standards.
# Optional
## **ABNF Syntax Notation**
**TODO**
- Familiarize with Augmented Backus-Naur Form (ABNF) specific to HTTP
- Reference: [RFC5234](https://www.rfc-editor.org/rfc/rfc5234)
- Include "#" operator for handling comma-separated lists and other relevant operators as mentioned in [Section 7 of RFC7230](https://www.rfc-editor.org/rfc/rfc7230#section-7)
- Appendix C and D in [RFC7231](https://www.rfc-editor.org/rfc/rfc7231#appendix-C) for additional context and expanded grammar.

**TODO**
- Utilize the terms "character", "character encoding
   scheme", "charset", and "protocol element" as defined in [RFC6365](https://www.rfc-editor.org/rfc/rfc6365) for consistency and accuracy in implementation decisions.
## **Resources**
**TODO**
- Implement parsing logic for target URIs when a client constructs an HTTP/1.1 request message.
- `Ensure the server can reconstruct an effective request URI for the target resource as per the specifications in Section 5.5 of RFC7230.`

**TODO**
- Ensure that resource identification is independent from request semantics.
- `Guarantee request semantics are defined by the request method and request-modifying header fields, as stated in Section 4 and Section 5 of RFC7231.` 

**TODO**
- Handle conflicts between method semantics and URIs.
- `In cases of conflict, prioritize the request method semantics over any URI-derived semantics, as highlighted in Section 4.2.1 of RFC7231.`

**References**
- [Section 2.7 of RFC7230](https://www.rfc-editor.org/rfc/rfc7230#section-2.7)
- [Section 5.3 of RFC7230](https://www.rfc-editor.org/rfc/rfc7230#section-5.3)
- [Section 5.5 of RFC7230](https://www.rfc-editor.org/rfc/rfc7230#section-5.5)
- [Section 4 of RFC7231](https://www.rfc-editor.org/rfc/rfc7231#section-4)
- [Section 5 of RFC7231](https://www.rfc-editor.org/rfc/rfc7231#section-5)
- [Section 4.2.1 of RFC7231](https://www.rfc-editor.org/rfc/rfc7231#section-4.2.1)
# Representations

## Understanding and Implementing HTTP Representations

**TODO**
- Implement functionality to handle multiple representations of a resource
- `An origin server might generate multiple representations that reflect the current state of a target resource. Implement algorithms to select the most applicable representation based on content negotiation.`

**TODO**
- Manage representation metadata and data stream
- `Ensure the representation includes both metadata and potentially unbounded data stream to denote the past, current, or desired state of a resource.`

**TODO**
- Implement content negotiation
- `Develop an algorithm that enables content negotiation for selecting the most appropriate resource representation for a given request.`

**TODO**
- Evaluate conditional requests using the selected representation
- `Use the selected representation for conditional requests evaluations as specified by RFC 7232.`

**TODO**
- Construct payloads for 200 (OK) and 304 (Not Modified) responses
- `Leverage the selected representation to construct response payloads appropriately for 200 and 304 status codes.`
## **Representation Metadata**
Representation header fields provide metadata about the representation. When a message includes a payload body, these header fields describe how to interpret the representation data enclosed in the payload body. In a response to a HEAD request, they describe the representation data that would have been enclosed in the payload body if the same request had been a GET.

**TODO**
- Implement handling for representation header fields which convey representation metadata including:
  - **Content-Type**: Ensure correct interpretation and handling of the media type of the representation.
    - Refer to [Section 3.1.1.5](https://www.rfc-editor.org/rfc/rfc7231#section-3.1.1.5) for specifics.
  - **Content-Encoding**: Implement logic to appropriately process the represented data encoding.
    - Refer to [Section 3.1.2.2](https://www.rfc-editor.org/rfc/rfc7231#section-3.1.2.2).
  - **Content-Language**: Determine and process the natural language(s) of the representation.
    - Refer to [Section 3.1.3.2](https://www.rfc-editor.org/rfc/rfc7231#section-3.1.3.2).
  - **Content-Location**: Handle the specific location where the data is available.
    - Refer to [Section 3.1.4.2](https://www.rfc-editor.org/rfc/rfc7231#section-3.1.4.2).

- Implement the logic to properly interpret these headers when a payload is present and when responding to HEAD requests, ensuring compliance with the directives specified in the relevant RFC sections.


## **Processing Representation Data**  
**TODO**
- Implement content coding support
   - It is necessary to correctly interpret the `Content-Encoding` header field in order to process representation data. The implementation should support commonly used content codings such as `gzip` and `deflate`, while other codings may require extensions or third-party libraries.
   
- Provide support for character encoding
   - Correctly interpret the `Content-Type` header field's optional `charset` parameter to understand the character encoding used. Common encodings like `UTF-8` and `ISO-8859-1` should be supported by default.

- Implement handling of composite media types
   - Manage media types that have multiple parts, such as `multipart/form-data`. Ensure that individual parts are parsed and processed efficiently.

- Verify and handle content-type and content-coding errors
   - Ensure that any unsupported media types or content-codings result in an appropriate error response, such as `415 Unsupported Media Type`.

- Maintain type and encoding preservation
   - When forwarding or caching representation data, preserve the original `Content-Type`, `Content-Encoding`, and `Content-Language` header fields to maintain proper handling by downstream receivers.
# Mandatory
## **Media Type**

**TODO**
- Parse and handle media types in `Content-Type` and `Accept` headers
- `media-type = type "/" subtype *( OWS ";" OWS parameter )`
- `type = token`
- `subtype = token`

**TODO**
- Ensure the correct handling of parameters following the media type
- `parameter = token "=" ( token / quoted-string )`

**TODO**
- Implement media type comparison logic to be case-insensitive 
- `The type, subtype, and parameter name tokens are case-insensitive.`

**TODO**
- Handle parameter values which may be case-sensitive depending on parameter name semantics
- `Parameter values might or might not be case-sensitive, depending on the semantics of the parameter name.`

**TODO**
- Treat quoted and unquoted parameter values equivalently 
- Example equivalences:
  - `text/html;charset=utf-8`
  - `text/html;charset=UTF-8`
  - `Text/HTML;Charset="utf-8"`
  - `text/html; charset="utf-8"`

**TODO**
- Disallow whitespace around the "=" character in media type parameters
- `media type parameters do not allow whitespace (even "bad" whitespace) around the "=" character.`

**Note:**
- Ensure that all Internet media types are registered with IANA according to defined procedures.
## **Charset**
**TODO**
- Implement case-insensitive token matching for charset names
  - `Charset names are represented as tokens and should be treated in a case-insensitive manner.`

**TODO**
- Validate and register charset names against the IANA "Character Sets" registry
  - `Ensure charset names are registered and validated according to the procedures defined in RFC2978.`
  - Reference: [IANA Character Sets registry](http://www.iana.org/assignments/character-sets)
# Canonicalization and Text Defaults

## **Text Media in HTTP**

**TODO**
- Implement support for various line breaks in `text` media types
  - Ensure the ability to parse line breaks in text media that may consist of CRLF, bare CR, or bare LF.
  - Note: This flexibility only applies within a representation assigned a "text" media type.

**TODO**
- Ensure text media is not charset-limited to octets 13 and 10 for CR and LF

**TODO**
- Verify representations are in canonical form before encoding with a content-coding

**References**
- MIME canonical form details: [RFC2045](https://www.rfc-editor.org/rfc/rfc2045)
- Constraints not applicable to HTTP: [RFC7231 Appendix A](https://www.rfc-editor.org/rfc/rfc7231#appendix-A)

**Note**
- While MIME constrains text subtypes to use CRLF, HTTP allows more flexible text line breaks for performance. 

### Additional Considerations:
- The flexibility for line breaks does not extend to "multipart" types or elements outside the payload body (e.g., header fields).
# Multipart Types

**TODO**
- Implement handling for multipart MIME types
  - Follow the common syntax as defined in [RFC2046 Section 5.1.1](https://www.rfc-editor.org/rfc/rfc2046#section-5.1.1) 
  - Ensure the inclusion of a boundary parameter as part of the media type value

**TODO**
- Generate CRLF exclusively for line breaks between body parts
  - `A sender MUST generate only CRLF to represent line breaks between body parts.`

**TODO**
- Understand that HTTP message framing does not use multipart boundaries to indicate message body length
  - `For example, the "multipart/form-data" type is often used for carrying form data in a request, as described in RFC2388, and the "multipart/byteranges" type is defined for use in some 206 (Partial Content) responses.`
## **Content-Type Header**

**TODO**
- Implement support for parsing and handling the `Content-Type` header field
  - `Content-Type = media-type`
  - Media types are defined in [Section 3.1.1.1](https://www.rfc-editor.org/rfc/rfc7231#section-3.1.1.1).
  - Example: `Content-Type: text/html; charset=ISO-8859-4`

**TODO**
- Ensure that messages containing a payload body include a `Content-Type` header field, unless the media type is unknown.
  - If the `Content-Type` header is absent, assume "application/octet-stream" or examine data to determine its type.

**TODO**
- Provide option to disable "content sniffing"
  - Implement a mechanism to disable automatic content-type interpretation based on payload when `Content-Type` is suspected incorrect.
  - **Reason**: Risk of security vulnerabilities like "privilege escalation" due to incorrect MIME type interpretation.

**TODO**
- Consider the implications for security and processing when the `Content-Type` does not match the media type of the payload.
  - `A receiver that considers "content sniffing" should carefully determine if overriding the specified type offers any benefit without security implications.`


# HTTP/1.1 RFC 7230 Compliance

## Mandatory
...

## **Encoding for Compression or Integrity**
**TODO**
- Implement `Transfer-Encoding` for data encoding purposes
- `Transfer-Encoding is primarily provided to enable efficient
   transmission and to improve message integrity.`

**TODO**
- Support and correctly order `Transfer-Encoding` options
- `If one or more encodings have been applied to a representation, the sender MUST apply them in the order that they were listed in the header field. A sender that applies multiple encodings MUST generate a Transfer-Encoding header field that lists the sequence of encodings applied.`

**TODO**
- Reject malformed `Transfer-Encoding` with a 400 status code
- `Recipients MUST reject messages where the length of the Transfer-Encoding header value is not clearly defined or deviates from specifications.`

**TODO**
- Handle `Chunked` Transfer-Encoding
- `The chunked transfer coding is useful when the content length is not known at the start of the message, allowing a server to stream data without knowing the final size in advance.`

**TODO**
- Implement logic for decoding and processing `Content-Encoding` upon receipt
- `If any content-coding is applied to a payload, the integrity of that content must be ensured during the transfer to guarantee proper receipt and processing by the recipient.`

**TODO**
- Implement robust error handling for unsupported or unexpected encodings
- `If a recipient receives a transfer-encoding that it does not understand or cannot process, it MUST respond with a 501 (Not Implemented) status code.`

...
# Mandatory
## **Content Codings**

**TODO** 
- Implement support for content-coding values
  - Designate recognized content-coding values (`compress`, `deflate`, `gzip`, `x-compress`, `x-gzip`)
  - Ensure content-coding values are processed in a case-insensitive manner
  - Refer to the HTTP Content Coding Registry: [Section 8.4](https://www.rfc-editor.org/rfc/rfc7231#section-8.4)
  
**TODO**
- Implement handling of Accept-Encoding header fields
  - Process and validate content coding tokens provided in the `Accept-Encoding` requests

**TODO**
- Implement handling of Content-Encoding header fields
  - Ensure these are processed according to the specific coding rules defined in:
    - [Section 4.2.1 (compress and x-compress)](https://www.rfc-editor.org/rfc/rfc7230#section-4.2.1)
    - [Section 4.2.2 (deflate)](https://www.rfc-editor.org/rfc/rfc7230#section-4.2.2)
    - [Section 4.2.3 (gzip and x-gzip)](https://www.rfc-editor.org/rfc/rfc7230#section-4.2.3)

**TODO**
- Ensure encoded representations are decoded at the final recipient
- Check and handle for identity retention and information loss during encoding/decoding process
# **Content-Encoding**

## **TODO**

- **Implement Parsing of Content-Encoding Header**  
  `The "Content-Encoding" header field indicates what content codings have been applied to the representation. It's crucial to parse this header to determine the necessary decoding mechanisms.`

- **Generate Correct Content-Encoding Header**  
  `If one or more encodings have been applied to a representation, the sender that applied the encodings MUST generate a Content-Encoding header field that lists the content codings in the order in which they were applied. Ensure your implementation follows this order.`

- **Decode Representations Based on Content-Encoding**  
  `Typically, the representation is only decoded just prior to rendering or analogous usage. Ensure your system supports decoding based on the content-codings indicated.`

- **Handle Inherent Encoding Carefully**  
  `If the media type includes an inherent encoding that matches a content-coding, do not list it in Content-Encoding unless applied again. Implement logic to discern between inherent and additional content-codings.`

- **Support 415 Status Code for Unsupported Encoding**  
  `Implement logic to respond with a status code of 415 (Unsupported Media Type) if a representation in the request message has a content coding that is not acceptable.`

- **Consider Compatibility with User Agents**  
  `Note that some user agents may handle representations differently depending on whether the coding is defined as part of Content-Type or Content-Encoding. Implement logic to address this nuance in representations as necessary.`

By implementing these steps, we ensure that our handling of the "Content-Encoding" header is robust and compliant with RFC 7230.
# **Connection Management**
## **Persistent Connections**
**TODO**
- Implement handling of persistent connections
- `HTTP/1.1 defines persistent connections by default. Each connection can be reused for several requests/responses. A connection is closed by default when the server or client sends a `Connection: close` header.`

**TODO**
- Support for connection timeout
- `Servers SHOULD have some sort of connection timeout to ensure that connections that are unexpectedly dropped or left idle do not stay open indefinitely.`

## **Upgrade Header**
**TODO**
- Support protocol switching via Upgrade header
- `The "Upgrade" header field is intended to provide a simple mechanism for transitioning from HTTP/1.1 to some other protocol on the same connection. When sent by a client, it indicates the desired protocol to switch to.`

## **Connection Header**
**TODO**
- Correct processing of the Connection header field
- `The Connection header field allows the sender to specify options that are requested for the connection and MUST NOT be forwarded by proxies. It can be used to control options such as connection persistence and connection-specific headers.`

# **General Syntax**
## **Field Parsing**
**TODO**
- Implement case insensitive field-name parsing
- `Although HTTP header names are usually capitalized, they are case-insensitive and should be parsed as such.`

**TODO**
- Account for obsolete line folding
- `In legacy HTTP implementations, header fields were allowed to span multiple lines via line folding. HTTP/1.1 has deprecated this mechanism. The CRLF sequence followed by at least one whitespace is a folding indication and should be treated as a single space in the folded line.`

## **Character Encoding**
**TODO**
- Handle character encoding properly
- `HTTP messages are typically encoded using the US-ASCII character set, but they may include message bodies encoded in other character sets as determined by the Content-Type of the entity.`

# **Security Considerations**
## **Message Integrity**
**TODO**
- Implement message integrity checks 
- `Implement mechanisms like checksums or signatures to ensure the integrity of HTTP messages, checking against corruption or tampering during transit.`

## **Confidentiality**
**TODO**
- Consider Transport Layer Security (TLS)
- `HTTP/1.1 itself does not provide a secure means of ensuring confidentiality. Consider using Transport Layer Security (TLS) for encrypting traffic to ensure confidentiality.`

These tasks are now ready for inclusion in your team’s markdown documentation based on your ongoing analysis of RFC 7230!
## **Language Tags**

**TODO**
- Implement Language Tag Parsing
- Ensure language tags conform to [RFC5646](https://www.rfc-editor.org/rfc/rfc5646) specifications, which dictate that language tags must be a sequence of one or more case-insensitive subtags, each separated by a hyphen ("-"). Whitespace is not allowed.

**TODO**
- Validate Language Tags in Headers
- Apply language tag parsing specifically to the `Accept-Language` and `Content-Language` headers, ensuring no whitespace within language tags and compliance with [RFC5646](https://www.rfc-editor.org/rfc/rfc5646#section-2.1).

**Examples:**
- Recognize and correctly handle language tags such as `fr`, `en-US`, `es-419`, `az-Arab`, `x-pig-latin`, `man-Nkoo-GN`. 

**References:**
- RFC 5646: [Tags for Identifying Languages](https://www.rfc-editor.org/rfc/rfc5646) 
- RFC 7231 Section 5.3.5: [Language-Range](https://www.rfc-editor.org/rfc/rfc7231#section-5.3.5)
## **HTTP Headers - Content-Language**

**TODO**
- Implement parsing and handling of `Content-Language` header
- `The "Content-Language" header field describes the natural language(s) of the intended audience for the representation.`
  
**TODO**
- Recognize and handle cases where multiple language tags are specified
- Language tags should be defined as `1#language-tag`

**TODO**
- Default assumption when `Content-Language` is not specified:
  - `If no Content-Language is specified, the default is that the content is intended for all language audiences. This might mean that the sender does not consider it to be specific to any natural language, or that the sender does not know for which language it is intended.`

**TODO**
- Differentiate content by intended audience
- `The primary purpose of Content-Language is to allow a user to identify and differentiate representations according to the users' own preferred language.`

**Examples:**
- Single language content for Danish audience:  
  `Content-Language: da`
  
- Multiple languages content for Maori and English audiences:
  `Content-Language: mi, en`

- Proper handling where multiple languages are present but intended for a single audience (e.g., a Beginner’s language primer):  
  `Content-Language` should only include the language of the intended audience, e.g., `en` for English literacy.

Note: Content-Language can be applied to any media type, not limited to textual documents.
## **Identification**

### **Connection Management**
**TODO**
- Implement logic to process `Connection` header field correctly
- `The "Connection" header field allows the sender to indicate desired control options for the current connection. The connection options are a comma-separated list of one or more connection-token values.`
- Ensure that `Connection` header field is used only in HTTP/1.1 versions and manage its tokens appropriately.
- https://www.rfc-editor.org/rfc/rfc7230#section-6.1

### **Host Header Field**
**TODO**
- Enforce the presence of `Host` header field in HTTP/1.1 requests
- `A client MUST send a Host header field in all HTTP/1.1 request messages on the Internet.`
- Implement logic to handle requests lacking `Host` header field with a 400 (Bad Request) status code.
- https://www.rfc-editor.org/rfc/rfc7230#section-5.4

### **Proxies**
**TODO**
- Recognize and appropriately handle the `Forwarded` HTTP extension defined in RFC 7239
- `A sender MAY generate a Forwarded header field with information about the possible routes a request has taken.`
- Ensure support for parsing multiple `Forwarded` headers into a coherent list.
- https://www.rfc-editor.org/rfc/rfc7239#section-5

### **Attributes**
**TODO**
- Parse and validate attributes in both requests and responses
- Ensure compliance with syntactic rules for defining attributes within header fields, as defined in the RFC.

### **Product Tokens**
**TODO**
- Validate the format of product tokens in `User-Agent` and `Server` fields
- Implement logic to ignore or reject malformed product tokens
- `A product token refers to software that acts as a client or server; it is comprised of a name and a version, which are used to identify the software in informational header fields, such as User-Agent and Server.` 
- https://www.rfc-editor.org/rfc/rfc7231#section-5.5.3

By adhering to these actionable steps, we ensure that our HTTP implementation effectively handles key aspects of identification, maintaining robust communication standards and compliance with RFC 7230 guidelines.
## **Identifying a Representation**

**TODO**
- Handle Content-Location header field for request messages
  - `For a request message: If the request has a Content-Location header field, then the sender asserts that the payload is a representation of the resource identified by the Content-Location field-value. However, such an assertion cannot be trusted unless it can be verified by other means.`

**TODO**
- Implement logic for identifying representations in response messages
  - `For response messages, apply these rules:`

    1. **GET/HEAD Method & Certain Status Codes**
       - If the response status code is 200 (OK), 204 (No Content), 206 (Partial Content), or 304 (Not Modified) and the request method is GET or HEAD, treat the payload as a representation of the resource identified by the effective request URI.

    2. **203 Status Code (Non-Authoritative Information)**
       - If the request method is GET or HEAD and the status code is 203, treat the payload as a potentially modified or enhanced representation of the target resource as provided by an intermediary.

    3. **Content-Location with Same URI as Effective Request URI**
       - If the response has a Content-Location header field that is a reference to the same URI as the effective request URI, treat the payload as a representation of the resource identified by the effective request URI.

    4. **Content-Location with Different URI**
       - If the response has a Content-Location header field that is a reference to a URI different from the effective request URI, treat the payload as a representation of the resource identified by the Content-Location field-value. Note that this assertion cannot be trusted unless verified by other means.

    5. **Unidentified Payloads**
       - If none of the above rules apply, consider the payload as unidentified.
## **Content-Location Header**

**TODO**
- Handle the `Content-Location` header for both requests and responses.
  - As a response header:
    - If a `Content-Location` is included in a 2xx response and refers (converted to absolute) to the same URI as the effective request URI:
      - Consider the payload as the current representation of that resource at the origination date.
    - If the `Content-Location` URI differs from the effective request URI:
      - Recognize the claim that this URI identifies a different resource corresponding to the representation.
      - For responses to GET/HEAD: Indicate content negotiation with `Content-Location` being a more specific identifier.
      - For 201 (Created) responses: A matching `Content-Location` and `Location` indicates the payload is a representation of the newly created resource.
      - Otherwise, imply the payload reports on the requested action's status and its report is retrievable at the `Content-Location` URI.
  
  - As a request header:
    - Treat `Content-Location` as transitory request context.
    - Do not save it as metadata verbatim but may use it for processing guidance or source link versioning.
    - Must not alter request semantics based on the `Content-Location`. 

**References**
- Ensure consistency with [RFC 7230, Section 5.5](https://www.rfc-editor.org/rfc/rfc7230#section-5.5) and [RFC 2557, Section 4](https://www.rfc-editor.org/rfc/rfc2557#section-4) regarding syntax and semantics. 
- Consider implications detailed in [RFC 7231, Section 4.3.1](https://www.rfc-editor.org/rfc/rfc7231#section-4.3.1) and [RFC 7231, Section 4.3.2](https://www.rfc-editor.org/rfc/rfc7231#section-4.3.2).
# **Representation Data**
## **Payload Body and Semantics**
**TODO**
- Parse representation data as either payload body or effective request URI based on message semantics.
  
**TODO**
- Implement interpretation of representation data format and encoding using associated metadata header fields.

## **Content-Type and Content-Encoding**
**TODO**
- Implement two-layer, ordered encoding model to determine representation data type.
- Use `Content-Type` header field to identify data format.
- Use `Content-Encoding` header field to identify data encoding.
  
**TODO**
- Apply `Content-Encoding` transformations before applying `Content-Type` transformation to representation data.
## **Payload Semantics**

**TODO**
- Implement handling of HTTP message payloads based on request method and status code
  - **Request Payloads:** 
    - PUT: Ensure payload represents the desired state of the target resource. See [RFC7231 Section 4.3.4](https://www.rfc-editor.org/rfc/rfc7231#section-4.3.4).
    - POST: Ensure payload contains information to be processed by the target resource. See [RFC7231 Section 4.3.3](https://www.rfc-editor.org/rfc/rfc7231#section-4.3.3).

  - **Response Payloads:**
    - GET: Ensure payload represents the current state of the target resource. See [RFC7231 Section 4.3.1](https://www.rfc-editor.org/rfc/rfc7231#section-4.3.1).
    - POST: Ensure payload represents either the processing result or the new state of the resource after processing.

- Handle error status codes and ensure payload communicates the error condition appropriately

**TODO**
- Implement parsing and handling for specific payload header fields as defined in RFC:
  - `Content-Length`: [RFC7230 Section 3.3.2](https://www.rfc-editor.org/rfc/rfc7230#section-3.3.2)
  - `Content-Range`: [RFC7233 Section 4.2](https://www.rfc-editor.org/rfc/rfc7233#section-4.2)
  - `Trailer`: [RFC7230 Section 4.4](https://www.rfc-editor.org/rfc/rfc7230#section-4.4)
  - `Transfer-Encoding`: [RFC7230 Section 3.3.1](https://www.rfc-editor.org/rfc/rfc7230#section-3.3.1)
## **Content Negotiation**

**TODO**
- Implement support for proactive content negotiation
  - Ensure the server can select representation based on user agent preferences. Use headers like `Accept`, `Accept-Language`, `Accept-Encoding` to determine the best representation.

**TODO**
- Implement support for reactive content negotiation
  - Provide a list of available representations for the user agent to choose from if multiple formats, languages, or encodings exist.

**TODO**
- Review and consider handling other content negotiation patterns if applicable
  - Conditional content, active content, and Transparent Content Negotiation can offer additional benefits:

  - Conditional content:
    - Explore capabilities to serve different parts of the representation based on user agent parameters.
    
  - Active content:
    - Enable scripts within the representation to make additional requests based on user agent features.

  - Transparent Content Negotiation:
    - Investigate intermediary solutions for content selection, using RFC2295 as a reference.

**TODO**
- Ensure resource consistency in responses
  - Maintain consistent representations of resources over time and across different negotiation dimensions for reliable user experiences.
Here's the markdown for actionable TODOs regarding Proactive Negotiation:

## **Proactive Negotiation**

**TODO**

- Implement handling for proactive content negotiation
  - Recognize and process explicit negotiation fields in requests: See [RFC 7231 Section 5.3](https://www.rfc-editor.org/rfc/rfc7231#section-5.3)
  - Consider implicit characteristics like client's network address or User-Agent for negotiation.

- Ensure to handle the potential inefficiencies and privacy risks associated with user agent capability descriptions in every request.
  - Be cognizant of user privacy and reduce unnecessary information sharing.
  - Optimize for efficiency given that only a small percentage of responses have multiple representations.

- Manage the complexity of proactive negotiation by ensuring its reliable implementation.
  - Recognize that user-agent preferences may not always be honored by the server. Thus, balancing response appropriateness and compliance with user-agent requests.

- Handle `Vary` header appropriately
  - Implement logic to include a `Vary` header field in responses subject to proactive negotiation to indicate request components influencing the selection.
  - [RFC 7231 Section 7.1.4](https://www.rfc-editor.org/rfc/rfc7231#section-7.1.4)

## **Reactive Negotiation**

**TODO**
- Implement support for reactive negotiation
  - `With reactive negotiation (a.k.a., agent-driven negotiation), selection of the best response representation is performed by the user agent after receiving an initial response from the origin server that contains a list of resources for alternative representations.`
  
**TODO**
- Handle responses with status code 300 (Multiple Choices) and 406 (Not Acceptable) for reactive negotiation
  - `A server might choose not to send an initial representation, other than the list of alternatives, and thereby indicate that reactive negotiation by the user agent is preferred.`
  - `For example, the alternatives listed in responses with the 300 (Multiple Choices) and 406 (Not Acceptable) status codes include information about the available representations so that the user or user agent can react by making a selection.`
  
**TODO**
- Document the advantages and disadvantages of reactive negotiation:
  - 📈 Advantages:
    - `Reactive negotiation is advantageous when the response would vary over commonly used dimensions (such as type, language, or encoding)`
    - `When the origin server is unable to determine a user agent's capabilities from examining the request.`
    - `When public caches are used to distribute server load and reduce network usage.`
  - 📉 Disadvantages:
    - `Reactive negotiation suffers from the disadvantages of transmitting a list of alternatives to the user agent, which degrades user-perceived latency if transmitted in the header section, and needing a second request to obtain an alternate representation.`

**TODO**
- Consider developing a mechanism for supporting automatic selection of alternative representations
  - `Furthermore, this specification does not define a mechanism for supporting automatic selection, though it does not prevent such a mechanism from being developed as an extension.`
## **Request Methods**

**TODO**
- Validate incoming request methods against implemented methods.
- `A server that receives a request method it does not recognize or that is not supported SHOULD respond with a 501 (Not Implemented) status code.`

**TODO**
- Add support for the following standard HTTP methods:
   - `GET`: Retrieve the resource at the request URI.
   - `POST`: Submit data to be processed to the identified resource.
   - `PUT`: Replace all current representations of the target resource.
   - `DELETE`: Remove all current representations of the target resource.
   - `HEAD`: Retrieve metadata without receiving the body.
   - `OPTIONS`: Describe the communication options for the target resource.
   - `PATCH`: Apply partial modifications to a resource.
   - `TRACE`: Perform a message loop-back test along the path to the target resource.

**TODO**
- Consider method safety and idempotency when implementing methods.
   - `GET`, `HEAD`, `OPTIONS`, and `TRACE` are defined as safe methods.
   - `PUT` and `DELETE` are idempotent methods.
## **Request Methods**
**TODO**
- Ensure implementations support the mandatory GET and HEAD methods. 
- Implement support for other optional methods (POST, PUT, DELETE, CONNECT, OPTIONS, TRACE) based on application needs.
- Verify implemented methods adhere to standardized semantics, applying the same approach to every resource.
- Examine existing HTTP method registrations in the "Hypertext Transfer Protocol (HTTP) Method Registry" maintained by IANA.
- Implement behavior to respond with a 501 (Not Implemented) status code if an unrecognized or unimplemented request method is received.
- Implement behavior to respond with a 405 (Method Not Allowed) status code if a known but not allowed request method is received for a target resource.

Link for reference:
- Explore the table of standardized methods above and implement necessary logic for each.
- Consult Section 8.1 for registry-defined methods: [Section 8.1](https://www.rfc-editor.org/rfc/rfc7231#section-8.1)
## **Common Method Properties**

**TODO** 
- Implement support for idempotent methods
  - `A request method is considered "idempotent" if the intended effect
    on the server of multiple identical requests with that method is the
    same as the effect for a single such request.`

**TODO** 
- Implement support for safe methods
  - `Request methods are considered "safe" if their defined semantics are
    essentially read-only; i.e., the client does not request, and does
    not expect, any state change on the origin server as a result of
    applying the request.`

**TODO** 
- Recognize and handle cacheable methods
  - `In general, a method is considered "cacheable" if responses to that method
    can be stored for reuse by a cache. GET, HEAD, and POST are methods already
    considered to be cacheable.`

**TODO** 
- Validate safe methods
  - `Origin server implementations SHOULD handle "safe" methods
    correctly by not causing side effects for any resource that was not
    explicitly taken as a target of the request.`
  
**TODO**
- Implement logic for conditional requests
  - `An HTTP/1.1 server that does not serve requests with "Expect: 100-continue"
    MUST still be able to respond appropriately to the actual request itself.` 

**TODO**
- Develop test cases for method safety and idempotency
  - Include scenarios for repeated and conditional requests to validate correct behavior.
___
## **Safe Methods**

**TODO**  
- **Ensure Appropriate Use of Safe Methods**  
  Verify that GET, HEAD, OPTIONS, and TRACE methods are implemented as read-only without causing state changes, to align with RFC 7231's definition of safe methods.

**TODO**  
- **Implement User Agent Awareness**  
  Develop a mechanism for the user agent to distinguish between safe and unsafe methods, providing users with warnings before initiating potentially unsafe actions.

**TODO**  
- **Implement Resource Owner Restrictions**  
  Ensure resource owners disable or disallow unsafe actions (such as 'delete' via query parameters) when accessed using safe request methods like GET, to prevent unintended side effects during automated tasks like web crawling or indexing. 

**Guideline Reference**  
- [RFC 7231 Section 4.2.1 - Safe Methods](https://www.rfc-editor.org/rfc/rfc7231#section-4.2.1)
## **Idempotent Methods**

**TODO**
- Implement support for idempotent methods by ensuring that multiple identical requests have the same intended effect as a single request.
- Recognize PUT, DELETE, and safe request methods as idempotent according to the specification.
- Allow for automatic repetition of idempotent requests in case of communication failures before the server's response can be read by the client.
- Implement mechanisms to ensure that repeated idempotent requests produce consistent intended effects, even if the original request succeeded.
## **Cacheable Methods**

**TODO**
- Implement caching logic for cacheable HTTP methods.
- `Request methods can be defined as "cacheable" to indicate that responses to them are allowed to be stored for future reuse. The specification defines GET, HEAD, and POST as cacheable methods. However, the majority of cache implementations tend to only support GET and HEAD.`

  - `GET`: Ensure caching is properly handled to store and retrieve resources efficiently.
  - `HEAD`: Implement logic to handle caching of metadata about resources without fetching the actual resource.
  - `POST`: Be aware of its cacheable definition, though in practice it may not be widely supported for caching – recognize specific scenarios where caching a POST response is appropriate in line with RFC7234.

- For detailed requirements, refer to RFC7234: [Hypertext Transfer Protocol (HTTP/1.1): Caching](https://www.rfc-editor.org/rfc/rfc7234).


## **Method Definitions**

**TODO**
- Implement robust parsing and handling logic for the different HTTP method types.
- `Each method has specific semantics that influence how the server processes the request and what kind of response is expected. Ensure adherence to the RFCs defining each method's behavior.`

**TODO**
- Validate and enforce allowed methods for your server.
- `The allowed methods should align with the server functionality and security requirements. Consider auditing supported methods and denying unsupported or unsafe methods with a 405 (Method Not Allowed) status.`

**TODO**
- Ensure idempotency and safety for applicable methods.
- `Methods like GET should be safe, i.e., they don't alter the state of the server. Methods like PUT and DELETE should be idempotent, meaning multiple requests yield the same effect as one request.`

**TODO**
- Implement logic for processing request headers and bodies associated with different methods.
- `Some methods mandate a request body, while others rely solely on headers. Ensure that request and response processing aligns with method-specific requirements as per RFC 7230.`

**TODO**
- Maintain extendability for adding new HTTP methods in the future.
- `Keep the implementation flexible enough to accommodate new methods, ensuring proper validation and functionality testing is in place.`

**TODO**
- Return appropriate status codes depending on method outcomes
- `For instance, use 405 when a method is not allowed, 501 for unsupported methods, and 400 or other applicable status codes for erroneous requests.`
# Request Methods
## **GET Method**

**TODO**
- Implement logic to handle GET requests
- `The GET method requests transfer of a current selected representation for the target resource. It is the primary mechanism for information retrieval.`

**TODO**
- Normalize URI mapping
- `An origin server might be configured to execute files with the request as input and send the result as the representation rather than transferring files directly.`

**TODO**
- Implement support for Range requests
- `A client can alter the semantics of GET to be a "range request", requesting transfer of only some part(s) of the selected representation by sending a Range header field in the request.`

**TODO**
- Validate the GET request payload handling
- `A payload within a GET request message has no defined semantics; sending a payload body on a GET request might cause some existing implementations to reject the request.`

**TODO**
- Ensure the response to GET requests is cacheable
- `The response to a GET request is cacheable; a cache MAY use it to satisfy subsequent GET and HEAD requests unless otherwise indicated by the Cache-Control header field.`
## **HEAD Method**
**TODO**
- Ensure server does not send a message body in response to a HEAD request
  - Implement logic to terminate the response at the end of the header section.

**TODO**
- Send the same header fields in response to a HEAD request as would be sent for a GET request, except payload header fields may be omitted
  - Review and refine header management to conditionally include or exclude payload header fields in HEAD responses.

**TODO**
- Handle cases where a payload is included in a HEAD request
  - Evaluate potential rejection of HEAD requests with a payload by existing implementations.

## **Caching for HEAD Requests**
**TODO**
- Implement caching logic for HEAD responses
  - Use the Cache-Control header field to determine caching behavior.
  - Consider effects on previously cached responses to GET requests, ensuring compliance with RFC 7234 Section 4.3.5.
## **POST Method Implementation**

**TODO**
- Implement POST Method Processor
  - Handle various semantic functions of POST such as:
    - Providing a block of data to a data-handling process (e.g., HTML form submissions)
    - Posting a message to a bulletin board, newsgroup, or similar
    - Creating new resources
    - Appending data to a resource's existing representations

**TODO**
- Implement Response Semantics for POST
  - Ensure the use of appropriate status codes for processing outcomes of POST requests:
    - 201 (Created) when resources are created, including a Location header for the new resource
    - Avoid using status codes 206, 304, and 416 in POST responses

**TODO**
- Handle POST Response Caching
  - Implement caching logic only when explicit freshness information is included
  - Enable caching results from POST for later GET retrievals by using a 200 (OK) response with a Content-Location header matching the POST request URI

**TODO**
- Handle Redirection for Existing Resources
  - Redirect the user agent to existing resource identifiers using a 303 (See Other) response when applicable
  - Include the existing resource's identifier in the Location header to facilitate caching and resource identification without extra requests
**PUT Method**

**TODO**
- Implement logic for the PUT method, ensuring that it creates or replaces the target resource with the state defined by the enclosed representation.
  - `A successful PUT of a given representation would suggest that a subsequent GET on that same target resource will result in an equivalent representation being sent in a 200 (OK) response.`
  
**TODO**
- Return a `201 (Created)` response when a PUT request successfully creates a resource representation, otherwise return `200 (OK)` or `204 (No Content)` if it modifies an existing resource representation.

**TODO**
- Ensure that unrecognized header fields in a PUT request are ignored by the server.
  - `An origin server SHOULD ignore unrecognized header fields received in a PUT request.`

**TODO**
- Verify the consistency of the PUT representation with server constraints and handle inconsistencies as follows:
  - Transform the representation to make it consistent.
  - Change the resource configuration.
  - Reject the request with a `409 (Conflict)` or `415 (Unsupported Media Type)` status code if consistency cannot be achieved.

**TODO**
- Prevent sending validator header fields such as ETag or Last-Modified in a successful PUT response unless the representation data is saved without transformation and the validator field reflects the new representation.

**TODO**
- Redirect with a `3xx (Redirection)` response when a PUT request cannot be applied to the target resource, guiding the client to another resource if applicable.
  
**TODO**
- Send a `400 (Bad Request)` response if a PUT request contains a `Content-Range` header, as this indicates partial content mistakenly used with PUT.
  - `An origin server that allows PUT on a given target resource MUST send a 400 (Bad Request) response to a PUT request that contains a Content-Range header field.`

**TODO**
- Implement logic to handle potential side effects of a PUT request, acknowledging that updates might affect other resources and link them accordingly.

**TODO**
- Ensure that responses to the PUT method are not cacheable, and invalidate any stored responses in caches if a successful PUT request passes through them.
**DELETE Method in HTTP**

- The DELETE method is used by clients to request the origin server to disassociate the target resource from current functionality.

**TODO**

- Implement support for the DELETE HTTP method.
- Ensure only resources with defined deletion mechanisms allow DELETE requests.

**Handling DELETE Responses**

- Send appropriate status codes:
  - `202 Accepted`: If the deletion will likely succeed but hasn't been enacted yet.
  - `204 No Content`: If the deletion has been enacted but no content is returned.
  - `200 OK`: If the deletion has been enacted and a status representation is included in the response.

**Cache Behavior**

- Ensure DELETE responses are non-cacheable.
- Implement logic to invalidate stored responses in caches for the effective request URI whenever a DELETE request is received. 
- Refer to [RFC7234 Section 4.4](https://www.rfc-editor.org/rfc/rfc7234#section-4.4) for cache invalidation details.

**DELETE Request Payload**

- Avoid sending a payload body in a DELETE request, as it has no defined semantics and may cause some implementations to reject the request.


# **CONNECT Method**

**Description**: The CONNECT method requests the recipient to establish a tunnel to the destination origin server identified by the request-target. It's intended primarily for use with proxy requests and, if successful, restricts its behavior to blind forwarding of packets, in both directions, until the tunnel is closed.

**Relevant Requirements:**

**TODO**
- Implement logic to handle the CONNECT request according to the RFC guidelines.

**Recipient Proxy Behavior**
- Establish a tunnel by directly connecting to the request-target or by forwarding the CONNECT request to the next inbound proxy.

**Establishing Connection**
- Respond with a 2xx (Successful) status code if the tunnel is established. No Transfer-Encoding or Content-Length headers should be sent in the response.
  - **Note**: Implement logic to switch to tunnel mode immediately after the blank line in the successful response's header section.

**TODO**
- If a client sends a CONNECT request with payload, evaluate how to handle or reject the request.

**Security and Restrictions**
- Proxies supporting CONNECT SHOULD restrict its usage to specific known ports or adhere to a configurable whitelist of safe request targets to mitigate risks.
  - **Note**: Avoid allowing CONNECT to well-known or reserved TCP ports not intended for web traffic (e.g., port 25 for SMTP).

**Connection Closure**
- A tunnel is closed when a tunnel intermediary detects either side has closed its connection. The intermediary MUST attempt to send outstanding data to the opposite side before closing both connections and discarding any remaining data.

**Proxy Authentication**
- Consider handling cases of proxy authentication to establish authority for tunnel creation.
  - Example:
    ```
    CONNECT server.example.com:80 HTTP/1.1
    Host: server.example.com:80
    Proxy-Authorization: basic aGVsbG86d29ybGQ=
    ```

**Caching**
- Responses to CONNECT requests are not cacheable and must be treated as such in your implementation.
# Request Methods

## **OPTIONS Method**

**TODO**

- Implement support for the OPTIONS method.
  - Determine the communication options available for the target resource. This should apply to either the origin server or an intervening intermediary.

- Handle OPTIONS request with an asterisk ("*") as the request-target.
  - Treat it as a request to evaluate the server's general capabilities without targeting a specific resource.
  - Use this as a "ping" or "no-op" method, mainly to test the server’s HTTP/1.1 conformance.

- Implement response generation for OPTIONS requests.
  - Include any header fields that might indicate optional features or extensions implemented by the server, such as the `Allow` header.
  - Ensure to send a `Content-Length` header with a value of "0" if no payload body is present.
 
- Handle Max-Forwards header field.
  - Support sending a Max-Forwards header field to target a specific recipient in the request chain.
  - Ensure a proxy does not generate a Max-Forwards header while forwarding unless it was received with that header.

- Support payloads in OPTIONS requests.
  - Ensure a valid `Content-Type` header field is included if the OPTIONS request contains a payload body.
  - Though this specification does not define a use for such payloads, future extensions might require them for detailed queries.

- Confirm that responses to OPTIONS requests are not cached.
## **TRACE Method**

**TODO**
- Implement TRACE method support
- `The TRACE method requests a remote, application-level loop-back of the request message. The final recipient of the request SHOULD reflect the message received, excluding sensitive fields, back to the client as the message body of a 200 (OK) response with a Content-Type of "message/http".`

**TODO**
- Ensure that TRACE requests do not contain a message body
- `A client MUST NOT send a message body in a TRACE request.`

**TODO**
- Ensure that responses to TRACE requests are not cacheable
- `Responses to the TRACE method are not cacheable.`

**TODO**
- Exclude sensitive data in TRACE responses
- `A client MUST NOT generate header fields in a TRACE request containing sensitive data that might be disclosed by the response. The final recipient of the request SHOULD exclude any request header fields that are likely to contain sensitive data when that recipient generates the response body.`

**TODO**
- Handle the `Max-Forwards` header field appropriately
- `The final recipient is either the origin server or the first server to receive a Max-Forwards value of zero (0) in the request.`

**TODO**
- Handle the `Via` header field in TRACE responses
- `The value of the Via header field is of particular interest, since it acts as a trace of the request chain.`

___
___
# Request Header Fields

Request header fields are used by clients to provide additional context about the request, modify the request's behavior, or supply credentials and preferences. Consider the following actions for handling these fields appropriately:

**TODO**

- Ensure request header fields are processed correctly to account for request context and preferred response formats.
  - `Clients use request header fields to supply information about the context of the request, make it conditional based on the target resource state, suggest preferred response formats, supply authentication credentials, or modify the request's expected processing.`

- Implement logic to handle conditional requests based on target resource state.
  - `Ensure implementation of conditional GET using If-Modified-Since, If-Unmodified-Since, If-Match, If-None-Match, and If-Range request header fields.`

- Parse `Accept` headers to determine preferred formats and set response accordingly.
  - `Clients use Accept headers to indicate preferred media types, character sets, or encodings. Implement parsing logic to ensure responses match these preferences where possible.`

- Handle `Authorization` header field for authentication purposes.
  - `Use the Authorization header to receive and validate credentials that are encoded as JSON Web Tokens, API keys, or basic authentication.`

- Implement logic to parse and handle other common request header fields such as `User-Agent`, `Referer`, and `Cache-Control`.
  - `Parse fields to log user agents, validate referrers, and implement cache controls.`

For more detailed requirements, check relevant sections of the RFC documentation.

---
## **Request Header Controls**

Controls are request header fields that direct specific handling of the request.

**TODO**
- Implement handling for each specified header control according to their respective RFC sections:

| Header Field Name | RFC Reference |
|-------------------|---------------|
| Cache-Control     | [Section 5.2 of RFC7234](https://www.rfc-editor.org/rfc/rfc7234#section-5.2) |
| Expect            | [Section 5.1.1 of RFC7231](https://www.rfc-editor.org/rfc/rfc7231#section-5.1.1) |
| Host              | [Section 5.4 of RFC7230](https://www.rfc-editor.org/rfc/rfc7230#section-5.4) |
| Max-Forwards      | [Section 5.1.2 of RFC7231](https://www.rfc-editor.org/rfc/rfc7231#section-5.1.2) |
| Pragma            | [Section 5.4 of RFC7234](https://www.rfc-editor.org/rfc/rfc7234#section-5.4) |
| Range             | [Section 3.1 of RFC7233](https://www.rfc-editor.org/rfc/rfc7233#section-3.1) |
| TE                | [Section 4.3 of RFC7230](https://www.rfc-editor.org/rfc/rfc7230#section-4.3) |

**TODO**
- Review sections from the RFCs to understand the expected behavior and implement logic that adheres to these specifications.
## **Expect Header Field**

The "Expect" header field indicates certain behaviors that need to be supported by the server to properly handle the request. Currently, the only expectation defined is "100-continue".

**TODO**
- **Validate Expect Header**
  - Ensure the "Expect" header field is case-insensitive and correctly identifies the value "100-continue".
  - Implement logic to handle the "Expect" header field effectively, following the guidelines.
  
**TODO**
- **Client Expectations**
  - **Messages Without Body**: Ensure clients do not generate a 100-continue expectation in requests without a message body.
  - **Send Expect Header**: Clients that wait for a 100 (Continue) response before sending the request body MUST send an Expect header field containing "100-continue".
  - **Handle 417 Status Code**: Implement logic for clients to handle a 417 (Expectation Failed) status code and repeat the request without the 100-continue expectation.
  - **Indeterminate Waiting**: Clients SHOULD NOT wait indefinitely for a 100 (Continue) response.

**TODO**
- **Server Requirements**
  - **HTTP/1.0 Requests**: Implement logic to ignore 100-continue expectations in HTTP/1.0 requests.
  - **100 (Continue) Response Omission**: Options to skip the 100 (Continue) response if the message body has already been received or is indicated not to exist.
  - **Final Response**: Servers sending a 100 (Continue) response MUST send a final status code after processing the complete message body.
  - **Early Final Responses**: Indicate in the early final response whether the connection will close or if the server will continue reading/discarding the request message.

**TODO**
- **Proxy Requirements**
  - Upon receiving a 100-continue expectation in requests, decide on immediate final status code response or forward to origin server.
  - For proxies that suspect the next server supports only HTTP/1.0, consider sending an immediate 100 (Continue) response to the client.

Note: The "Expect" header field's extension mechanism was intended for interim 100 (Continue) responses and must-understand indications but is not extensively utilized in current practices.
## **Max-Forwards Header**

**TODO**

- Implement logic to handle `Max-Forwards` header for TRACE and OPTIONS methods
  - Check if a `Max-Forwards` header is present; if not, assume no forwarding limit
  - If `Max-Forwards` is present and the value is zero (0):
    - Do not forward the request
    - Respond as the final recipient
  - If `Max-Forwards` is greater than zero:
    - Decrement the value by one (1)
    - Forward the request with the updated `Max-Forwards` value

- Ensure logic is only applied for TRACE and OPTIONS request methods
  - Ignore `Max-Forwards` for other request methods

- Validate that `Max-Forwards` contains a valid decimal integer

- Prevent infinite loops by addressing potential max forwarding scenarios

- Reference Section 4.3.7 (OPTIONS) and Section 4.3.8 (TRACE) for specific method details
  - [OPTIONS Request Method](https://www.rfc-editor.org/rfc/rfc7231#section-4.3.7)
  - [TRACE Request Method](https://www.rfc-editor.org/rfc/rfc7231#section-4.3.8)
## **Conditional Requests**
**TODO**
- Implement handling for conditional request header fields
- `HTTP conditional request header fields allow a client to place a precondition on the state of the target resource, so that the action corresponding to the method semantics will not be applied if the precondition evaluates to false.`
- `Preconditions evaluate whether the state of the target resource has changed since a given state known by the client.`

- **Header Fields to Handle:**
  - `If-Match`: Disallow the operation if the current resource state does not match any of the listed entity tags.
    - Reference: [RFC7232 Section 3.1](https://www.rfc-editor.org/rfc/rfc7232#section-3.1)
  - `If-None-Match`: Disallow the operation if the current resource state matches any of the listed entity tags; often used to ensure resources are only modified if they haven't changed.
    - Reference: [RFC7232 Section 3.2](https://www.rfc-editor.org/rfc/rfc7232#section-3.2)
  - `If-Modified-Since`: Perform the operation only if the resource has been modified since the specified date.
    - Reference: [RFC7232 Section 3.3](https://www.rfc-editor.org/rfc/rfc7232#section-3.3)
  - `If-Unmodified-Since`: Perform the operation only if the resource has not been modified since the specified date.
    - Reference: [RFC7232 Section 3.4](https://www.rfc-editor.org/rfc/rfc7232#section-3.4)
  - `If-Range`: Used with a Range header to request the range provided that the resource is still the same as an identified entity tag/date.
    - Reference: [RFC7233 Section 3.2](https://www.rfc-editor.org/rfc/rfc7233#section-3.2)

**TODO**
- Ensure precondition evaluation is aligned with method semantics and choice of conditional as per RFC7232 guidelines.
## **Content Negotiation**
Content negotiation allows clients to specify their preferences for the format and content of the response.

**TODO: Implement request header fields for proactive content negotiation**

- **Accept**
  - Reference: [Section 5.3.2](https://www.rfc-editor.org/rfc/rfc7231#section-5.3.2)
  - Implement parsing and handling of the `Accept` header to specify preferred response media types.

- **Accept-Charset**
  - Reference: [Section 5.3.3](https://www.rfc-editor.org/rfc/rfc7231#section-5.3.3)
  - Implement parsing and handling of the `Accept-Charset` header to specify preferred character sets for the response.

- **Accept-Encoding**
  - Reference: [Section 5.3.4](https://www.rfc-editor.org/rfc/rfc7231#section-5.3.4)
  - Implement parsing and handling of the `Accept-Encoding` header to specify acceptable content encodings for the response.

- **Accept-Language**
  - Reference: [Section 5.3.5](https://www.rfc-editor.org/rfc/rfc7231#section-5.3.5)
  - Implement parsing and handling of the `Accept-Language` header to specify preferred languages for the response.
## **Quality Values**
**TODO**
- Implement parsing logic for "q" parameter 
- Ensure compliance with the `qvalue` format: real number between 0 and 1
- Default `qvalue` to 1 if not present

**TODO**
- Validate `qvalue` provided by sender
- Reject `qvalue` entries with more than three digits after the decimal point

**TODO**
- Limit user configuration to no more than three decimal digits

**References**
- The quality values are used for proactive content negotiation
- Quality values (`qvalue`) are normalized and range from 0 (not acceptable) to 1 (most preferred)
- https://www.rfc-editor.org/rfc/rfc7231#section-5.3.1
## **Accept Header Field**

**TODO**
- Implement logic to parse the `Accept` header field, supporting media-ranges and quality parameters.
- `Accept = #( media-range [ accept-params ] )`

**TODO**
- Recognize and handle specific and range media types.
- Process specific media types with higher precedence over media-range wildcards (`*/*`, `type/*`).

**TODO**
- Implement evaluation and prioritization based on media type quality value (`q`).
- If no `Accept` header is present, assume that any media type is acceptable.

**TODO**
- Implement handling for cases where no available representation is acceptable based on `Accept` header.
- Respond with a `406 Not Acceptable` status code or proceed with default representation if negotiation is disregarded.

**TODO**
- Ensure compatibility with media extensions and handle cases where media types have parameters.
- Remember: `q` parameter acts as a separator between media type parameters and extensions.

**TODO**
- Provide configuration options for setting default quality values for media-ranges within user agents.
# Accept Charset

## **Accept-Charset Header**
**TODO**
- Implement Accept-Charset header parsing
- `Accept-Charset = 1#( ( charset / "*" ) [ weight ] )`

**TODO**
- Handle special value '*' in Accept-Charset
- `The special value "*", if present in the Accept-Charset field, matches every charset that is not mentioned elsewhere in the Accept-Charset field.`

**TODO**
- Determine default behavior for absence of Accept-Charset field
- `A request without any Accept-Charset header field implies that the user agent will accept any charset in response.`

**TODO**
- Implement logic to handle 406 (Not Acceptable) response
- `If an Accept-Charset header field is present in a request and none of the available representations for the response has a charset that is listed as acceptable, the origin server can either honor the header field, by sending a 406 (Not Acceptable) response, or disregard the header field by treating the resource as if it is not subject to content negotiation.`

**TODO**
- Consider security and privacy concerns (Section 9.7) when implementing Accept-Charset
- User agents should be mindful of detailed charset lists which could potentially identify individual users.

---
## **Accept-Encoding**

**TODO**
- Parse and handle `Accept-Encoding` header field
- `Accept-Encoding: compress, gzip`
- Evaluate the client's acceptable content-codings and select the appropriate encoding for the response.

**TODO**
- Implement `identity` token handling
- Recognize "identity" token as a synonym for "no encoding" when processing `Accept-Encoding`.

**TODO**
- Handle wildcards in `Accept-Encoding`
- Implement logic to handle asterisk `*` symbol in the `Accept-Encoding` field, which matches any content-coding not explicitly listed.

**TODO**
- Implement quality values (qvalues)
- Evaluate qvalues associated with each coding value to determine preference for content-coding: 
  - A qvalue of 0 means "not acceptable".
  - Prefer highest non-zero qvalue when multiple encodings are acceptable.

**TODO**
- Handle empty `Accept-Encoding` fields
- Treat an empty combined field-value as the user agent not wanting any content-coding.

**TODO**
- Implement fallback for non-acceptable content-codings
- If no content-coding meets the `Accept-Encoding` criteria, server should send a response without any content-coding.
# Optional 
## **Accept-Language Header Field**
- https://www.rfc-editor.org/rfc/rfc7231#section-5.3.5

**TODO**
- Implement functionality to parse and process the "Accept-Language" header
- `Accept-Language = 1#( language-range [ weight ] )`

**TODO**
- Support parsing and comprehension of language ranges and quality values
- `language-range` as defined in RFC4647 Section 2.1
- Quality value indicates user preference for the specific language: higher values signify higher preference

**TODO**
- Apply content negotiation based on "Accept-Language" when serving responses. If none of the available representations match the language preference:
  - Consider disregarding the "Accept-Language" header and treating the response as if it is not subject to content negotiation
  - Alternatively, send a 406 (Not Acceptable) response, though it is not encouraged to avoid preventing user access to content.

**TODO**
- Ensure user agents allow user control over the linguistic preferences sent in "Accept-Language":
  - If users cannot configure their language preferences, an "Accept-Language" header field should not be sent
  - Provide guidance to users on setting their language preferences, suggesting alternatives if specific dialects (e.g., "en-gb") are selected. 

**NOTE**
- When multiple language ranges have the same quality value, their order may be considered as descending priority, though this behavior is unreliable. Ensure that user agents uniquely assign quality values for consistency.

**TODO**
- Investigate and potentially implement various language matching schemes as described in RFC4647 Section 3, with emphasis on "Basic Filtering" for HTTP. 

- **Privacy Considerations**: Be aware that sending complete linguistic preferences might conflict with user privacy expectations. Implement safeguards accordingly. 
# Mandatory

## **Authentication Credentials**

**TODO**
- Implement logic to handle `Authorization` and `Proxy-Authorization` header fields according to [RFC7235](https://www.rfc-editor.org/rfc/rfc7235).
  - `Authorization`: Section 4.2 of [RFC7235](https://www.rfc-editor.org/rfc/rfc7235#section-4.2)
  - `Proxy-Authorization`: Section 4.4 of [RFC7235](https://www.rfc-editor.org/rfc/rfc7235#section-4.4)

**TODO**
- Identify and manage user authentication via the `Cookie` header field based on [RFC6265](https://www.rfc-editor.org/rfc/rfc6265).

## **Security Note**

**TODO**
- Evaluate security implications of custom mechanisms using `Cookie` for authentication and implement appropriate safeguards.
# Mandatory

## **Request Context**
**TODO**
- Implement parsers for request context header fields and extract relevant information
  - **From**: Identify the user or organization responsible for the request
    - Reference: [Section 5.5.1](https://www.rfc-editor.org/rfc/rfc7231#section-5.5.1)
  - **Referer**: Determine the address of the previous web page from which a link to the currently requested page was followed
    - Reference: [Section 5.5.2](https://www.rfc-editor.org/rfc/rfc7231#section-5.5.2)
  - **User-Agent**: Identify the client's user agent string, providing details about the software acting on behalf of the user
    - Reference: [Section 5.5.3](https://www.rfc-editor.org/rfc/rfc7231#section-5.5.3)
  
**TODO**
- Validate and handle potentially sensitive information in the request context headers to ensure user privacy and compliance with relevant policies and regulations.


```markdown
## **From Header Field**
- https://www.rfc-editor.org/rfc/rfc7231#section-5.5.1

**TODO**
- Allow users to configure "From" header field
- `A user agent SHOULD NOT send a From header field without explicit
   configuration by the user, since that might conflict with the user's
   privacy interests or their site's security policy.`

**TODO**
- Include "From" header field for robotic user agents
- `A robotic user agent SHOULD send a valid From header field so that
   the person responsible for running the robot can be contacted if
   problems occur on servers, such as if the robot is sending excessive,
   unwanted, or invalid requests.`

**NOTE**
- Do not use "From" field for access control or authentication
- `A server SHOULD NOT use the From header field for access control or
   authentication, since most recipients will assume that the field
   value is public information.`
```

## **Referer Header Field**

**TODO**
- Implement logic for generating and handling the Referer header field according to the RFC.
- Ensure the user agent does not include fragment and userinfo components of the URI when generating the Referer field value.
- If the referring source does not have a URI (e.g., user input), exclude the Referer field or send it with the value "about:blank".
- Ensure that the user agent does not send a Referer header field in an unsecured HTTP request if the referring page was received with a secure protocol.
- Be aware of the privacy implications: do not reveal personal or confidential information within a Referer URI.
- Consider security impacts: do not indiscriminately remove or modify Referer fields, especially those sharing the same scheme and host as the request target. 

**References:**
- [RFC3986: Uniform Resource Identifier (URI): Generic Syntax](https://www.rfc-editor.org/rfc/rfc3986)
- [RFC7231 Section 9.4: Security Considerations regarding Referer](https://www.rfc-editor.org/rfc/rfc7231#section-9.4)


Here is the actionable markdown for handling the "User-Agent" header field from RFC 7230:

## **User-Agent Header**
**TODO**
- Implement logic to send a `User-Agent` field in each request unless specifically configured not to do so.
- Ensure that the `User-Agent` field consists of one or more product identifiers, each optionally followed by comments, to identify the user agent software and its significant subproducts.
- `User-Agent = product *( RWS ( product / comment ) )`
  
**TODO**
- Limit generated product identifiers to only essential information for identifying the product.
- Ensure that the `product-version` field contains accurate version identifiers and does not include advertising or nonessential information.
- `product = token ["/" product-version]`
- `product-version = token`

**TODO**
- Avoid generating `User-Agent` field values with unnecessarily fine-grained details. Limit the addition of subproducts by third parties.
- Avoid using `product` tokens of other implementations to declare compatibility as this may lead to unwanted identification (fingerprinting).

**Example**  
- `User-Agent: CERN-LineMode/2.15 libwww/2.17b3`
  
**Note**
- If a user agent masquerades as a different user agent intentionally, recipients should assume the user desires responses tailored for that identified user agent.
## **Response Status Codes**
- The `status-code` is a three-digit integer that indicates the result of the attempt to understand and fulfill the request.

**TODO**

- Implement logic to interpret and handle various status codes:
  - **1xx (Informational):** The request was received, continuing process
  - **2xx (Successful):** The request was successfully received, understood, and accepted
  - **3xx (Redirection):** Further action needs to be taken to complete the request
  - **4xx (Client Error):** The request contains bad syntax or cannot be fulfilled
  - **5xx (Server Error):** The server failed to fulfill an apparently valid request

- Ensure the client recognizes the class of the status code (first digit) even if the specific code is unknown. For unrecognized status codes, treat them as the `x00` status code of that class, with the exception of caching restrictions.

- Example: If a client receives an unrecognized status code like `471`, it should be treated as `400 (Bad Request)`.

- Implement reason phrases for each status code to provide explanatory descriptions. 

---

This structure will ensure that all status codes are appropriately understood and managed, thus improving the robustness and user-friendliness of client-server interactions.
## **Status Codes**

**TODO**
- Implement support for various HTTP status codes ensuring adherence to their definitions.
- For each status code, implement the default reason phrases listed below, with the option to replace them with local equivalents as necessary.

**Actionable Items:**
- Status codes that are cacheable by default, ensure they can be reused by a cache with heuristic expiration unless otherwise specified by method definitions or explicit cache controls ([RFC7234](https://www.rfc-editor.org/rfc/rfc7234)).

### Status Code List with Reason Phrases:
- **1xx Informational:**
  - `100` - Continue [(Section 6.2.1)](https://www.rfc-editor.org/rfc/rfc7231#section-6.2.1)
  - `101` - Switching Protocols [(Section 6.2.2)](https://www.rfc-editor.org/rfc/rfc7231#section-6.2.2)
  
- **2xx Successful:**
  - `200` - OK [(Section 6.3.1)](https://www.rfc-editor.org/rfc/rfc7231#section-6.3.1)
  - `201` - Created [(Section 6.3.2)](https://www.rfc-editor.org/rfc/rfc7231#section-6.3.2)
  - `202` - Accepted [(Section 6.3.3)](https://www.rfc-editor.org/rfc/rfc7231#section-6.3.3)
  - `203` - Non-Authoritative Information [(Section 6.3.4)](https://www.rfc-editor.org/rfc/rfc7231#section-6.3.4)
  - `204` - No Content [(Section 6.3.5)](https://www.rfc-editor.org/rfc/rfc7231#section-6.3.5)
  - `205` - Reset Content [(Section 6.3.6)](https://www.rfc-editor.org/rfc/rfc7231#section-6.3.6)
  - `206` - Partial Content [(Section 4.1 of RFC7233)](https://www.rfc-editor.org/rfc/rfc7233#section-4.1)
  
- **3xx Redirection:**
  - `300` - Multiple Choices [(Section 6.4.1)](https://www.rfc-editor.org/rfc/rfc7231#section-6.4.1)
  - `301` - Moved Permanently [(Section 6.4.2)](https://www.rfc-editor.org/rfc/rfc7231#section-6.4.2)
  - `302` - Found [(Section 6.4.3)](https://www.rfc-editor.org/rfc/rfc7231#section-6.4.3)
  - `303` - See Other [(Section 6.4.4)](https://www.rfc-editor.org/rfc/rfc7231#section-6.4.4)
  - `304` - Not Modified [(Section 4.1 of RFC7232)](https://www.rfc-editor.org/rfc/rfc7232#section-4.1)
  - `305` - Use Proxy [(Section 6.4.5)](https://www.rfc-editor.org/rfc/rfc7231#section-6.4.5)
  - `307` - Temporary Redirect [(Section 6.4.7)](https://www.rfc-editor.org/rfc/rfc7231#section-6.4.7)
  
- **4xx Client Error:**
  - `400` - Bad Request [(Section 6.5.1)](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.1)
  - `401` - Unauthorized [(Section 3.1 of RFC7235)](https://www.rfc-editor.org/rfc/rfc7235#section-3.1)
  - `402` - Payment Required [(Section 6.5.2)](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.2)
  - `403` - Forbidden [(Section 6.5.3)](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.3)
  - `404` - Not Found [(Section 6.5.4)](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.4)
  - `405` - Method Not Allowed [(Section 6.5.5)](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.5)
  - `406` - Not Acceptable [(Section 6.5.6)](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.6)
  - `407` - Proxy Authentication Required [(Section 3.2 of RFC7235)](https://www.rfc-editor.org/rfc/rfc7235#section-3.2)
  - `408` - Request Timeout [(Section 6.5.7)](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.7)
  - `409` - Conflict [(Section 6.5.8)](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.8)
  - `410` - Gone [(Section 6.5.9)](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.9)
  - `411` - Length Required [(Section 6.5.10)](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.10)
  - `412` - Precondition Failed [(Section 4.2 of RFC7232)](https://www.rfc-editor.org/rfc/rfc7232#section-4.2)
  - `413` - Payload Too Large [(Section 6.5.11)](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.11)
  - `414` - URI Too Long [(Section 6.5.12)](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.12)
  - `415` - Unsupported Media Type [(Section 6.5.13)](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.13)
  - `416` - Range Not Satisfiable [(Section 4.4 of RFC7233)](https://www.rfc-editor.org/rfc/rfc7233#section-4.4)
  - `417` - Expectation Failed [(Section 6.5.14)](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.14)
  - `426` - Upgrade Required [(Section 6.5.15)](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.15)
  
- **5xx Server Error:**
  - `500` - Internal Server Error [(Section 6.6.1)](https://www.rfc-editor.org/rfc/rfc7231#section-6.6.1)
  - `501` - Not Implemented [(Section 6.6.2)](https://www.rfc-editor.org/rfc/rfc7231#section-6.6.2)
  - `502` - Bad Gateway [(Section 6.6.3)](https://www.rfc-editor.org/rfc/rfc7231#section-6.6.3)
  - `503` - Service Unavailable [(Section 6.6.4)](https://www.rfc-editor.org/rfc/rfc7231#section-6.6.4)
  - `504` - Gateway Timeout [(Section 6.6.5)](https://www.rfc-editor.org/rfc/rfc7231#section-6.6.5)
  - `505` - HTTP Version Not Supported [(Section 6.6.6)](https://www.rfc-editor.org/rfc/rfc7231#section-6.6.6)
  
**Note**: This list may not be exhaustive and does not include extension status codes from other specifications. For a comprehensive list, refer to the [IANA status code registry](https://www.rfc-editor.org/rfc/rfc7231#section-8.2).
# 1xx (Informational) Status Codes

## Handling 1xx Responses

**TODO**
- **Server Logic**: Ensure that the server does not send 1xx responses to an HTTP/1.0 client.
- `A server MUST NOT send a 1xx response to an HTTP/1.0 client.`

**TODO**
- **Client Logic**: Implement parsing for one or more 1xx responses before a final response.
- `A client MUST be able to parse one or more 1xx responses received prior to a final response, even if the client does not expect one.`

**TODO**
- **Client Logic**: Allow user agents to ignore unexpected 1xx responses.
- `A user agent MAY ignore unexpected 1xx responses.`

**TODO**
- **Proxy Logic**: Ensure that proxies forward 1xx responses unless they requested the generation of the 1xx response.
- `A proxy MUST forward 1xx responses unless the proxy itself requested the generation of the 1xx response.`

## Implementation Notes

- 1xx responses are terminated by the first empty line after the status-line, signaling the end of the header section.
- Implement proper handling of 100 (Continue) responses for proxies adding "Expect: 100-continue" fields in forwarded requests.
## **100 Continue**

**TODO**
- Implement handling of 100 (Continue) status code.
- `When a request includes an Expect header with the 100-continue expectation, ensure the server sends a 100 (Continue) status code, signalling the client to proceed with the request payload.`

**TODO**
- Discard 100 (Continue) responses on the client-side if the Expect header is not present.
  - Client: Process and discard the 100 response when the Expect header with a 100-continue expectation is not included.
  
**TODO**
- Ensure proper sending of final response post-request receipt
  - Server: Send the appropriate final response after fully receiving and processing the entire request.
# **Status Codes**

## **101 Switching Protocols**
**TODO**
- Implement logic to handle the 101 (Switching Protocols) status code
- Ensure the server understands and is willing to comply with the client's request via the Upgrade header field.
- `The server MUST generate an Upgrade header field in the response that indicates which protocol(s) will be switched to immediately after the empty line that terminates the 101 response.`

- Evaluate scenarios where protocol switching is advantageous, such as:
  - Switching to a newer version of HTTP
  - Switching to a real-time, synchronous protocol for resources requiring such features

- Revisit the logic to ensure protocol switch only occurs when beneficial.
Here is a markdown for the 2xx status codes based on the RFC7231 guidelines:

## **2xx Successful Status Codes**
- The 2xx (Successful) class of status code indicates that the client's request was successfully received, understood, and accepted.

**TODO**
- Implement response handling logic for each of the following 2xx status codes:

  - **200 OK**: The request has succeeded. The information returned with the response is dependent on the method used in the request.
  
  - **201 Created**: The request has been fulfilled and has resulted in one or more new resources being created. The newly created resources can be referenced by the URI(s) returned in the response's Location header field.

  - **202 Accepted**: The request has been accepted for processing, but the processing has not been completed. The request might or might not eventually be acted upon.

  - **203 Non-Authoritative Information**: The request was successful but the enclosed payload has been modified from that of the origin server's 200 OK response by a transforming proxy.

  - **204 No Content**: The server has successfully processed the request, but is not returning any content. This code is often used as a response to a successful DELETE request.

  - **205 Reset Content**: The server has fulfilled the request and desires that the user agent reset the "document view" that caused the request to be sent.

  - **206 Partial Content**: The server is delivering only part of the resource due to a range header sent by the client. Used in the context of partial GET requests.

- Implement logic to follow different flow when a URI is returned with the Location header.

- Ensure that validation and error-checking mechanisms are put in place to appropriately provide debug information or handle unexpected behavior in response to all 2xx status responses.
# Mandatory

## **200 OK Status Code**
**TODO**
- Implement logic to handle 200 OK status code response based on the request method.
- `The 200 (OK) status code indicates that the request has succeeded. The payload sent in a 200 response depends on the request method.`
  - **GET**: Respond with a representation of the target resource.
  - **HEAD**: Respond with the same representation as GET, but without the representation data.
  - **POST**: Respond with a representation of the status of, or results obtained from, the action.
  - **PUT, DELETE**: Respond with a representation of the status of the action.
  - **OPTIONS**: Respond with a representation of the communications options.
  - **TRACE**: Respond with a representation of the request message as received by the end server.

- Ensure that, aside from responses to CONNECT, a 200 response always has a payload, though an origin server MAY generate a payload body of zero length.
- Use 204 (No Content) if no payload is desired.

- `A 200 response is cacheable by default unless otherwise indicated by the method definition or explicit cache controls.`

### Special Handling for `CONNECT`
**TODO**
- Ensure no payload is allowed for responses to CONNECT as the successful result is a tunnel, which begins immediately after the 200 response header section.
**TODO**
- Implement handling for 201 (Created) status code
- When responding with a 201 status:
  - Include a Location header field to identify the primary resource created, if applicable.
  - If no Location field is present, indicate the created resource by the effective request URI.
  - Ensure the response payload describes and links to the created resource(s).
- Reference [Section 7.2 of RFC 7231](https://www.rfc-editor.org/rfc/rfc7231#section-7.2) for handling and including validator header fields such as ETag and Last-Modified.
## **HTTP Status Code 202 (Accepted)**
**TODO**
- Implement handling of 202 (Accepted) status code
- `The 202 (Accepted) status code indicates that the request has been
   accepted for processing, but the processing has not been completed.
   The request might or might not eventually be acted upon, as it might
   be disallowed when processing actually takes place. There is no facility in HTTP for re-sending a status code from an asynchronous operation.`
- **Description**: Ensure that the representation sent with a 202 response describes the current status of the request and points to (or embeds) a status monitor that can provide an estimate of when the request will be fulfilled.
- Consider applications where server processing is deferred, ensuring the user agent does not have to maintain a connection until completion.
## **Status Codes**
**TODO**
- Implement handling for 203 Non-Authoritative Information status code
- `The 203 status code indicates that the response payload has been modified by a proxy.`
- Notify recipients when a transformation has been applied, impacting potential future decisions about the content.
- Ensure the 203 response is cacheable by default, unless overridden by method definitions or explicit cache controls.
- Consider providing a mechanism to identify transformations similar to Warning code 214.
## Status Code: 204 No Content

**TODO**
- Implement logic to handle 204 No Content status code
  - Ensure that the server properly fulfills a request with no additional content in the response body.
  - Consider response headers referring to the target resource and its representation after the requested action.
  
- Implement handling of metadata such as ETag in the response of 204 status code
  - For scenarios like a successful PUT request, set the ETag field-value to represent the entity-tag of the updated resource.
  
- Ensure that responses with a 204 status code do not contain a message body
  - Terminate a 204 response with the first empty line after the header fields.
  
- Ensure that 204 responses are cacheable by default unless specified otherwise by method definition or explicit cache controls. 
  - Refer to cache controls as specified in Section 4.2.2 of RFC 7234.
## **HTTP Response: 205 Reset Content**

**TODO**
- Implement handling for the 205 (Reset Content) status code
  - Ensure the server fulfills requests and instructs the user agent to reset the document view to its original state.
  - No additional content should be provided in a 205 response.

**Implementation Options for 205 Response:**
- Add a `Content-Length` header field with a value of 0 to indicate a zero-length body.
- Include a `Transfer-Encoding` header field with a value of `chunked`, with a message body consisting of a single zero-length chunk.
- Close the connection immediately after sending the blank line that terminates the header section.
# Redirection (3xx) Status Codes

The 3xx (Redirection) class of status codes indicates that further action is needed to complete the request. This section outlines the appropriate behaviors when handling redirection status codes.

**TODO**

- **Implement Redirection Logic**
  - Redirect automatically using the `Location` header field when provided.
  - Exercise caution when automatically redirecting requests with methods not known to be safe (e.g., POST).

- **Handling Different Types of Redirects**
  - 301 (Moved Permanently), 302 (Found), 307 (Temporary Redirect): These status codes indicate that a resource might be available at a different URI. Ensure the client handles reapplication of the method correctly.
  - 300 (Multiple Choices): Provide users with choices of resources, capable of representing the original request target.
  - 303 (See Other): Redirect clients to a different resource using the URI in the `Location` field to represent an indirect response.
  - 304 (Not Modified): Indicate that the client can use a cached version of the requested resource.

- **Cyclic Redirections Detection**
  - Detect and intervene in cases of cyclical redirections to prevent "infinite" loops. Implement logic to recognize repeated redirection patterns and limit the number of allowed redirections to avoid endless loops.

- **Method Handling**
  - Handle 301 and 302 status codes by rewriting the method to GET as per prevailing practice, especially when the original request method was POST. Ensure compliance with RFC specifications when handling redirection with these status codes.

This implementation will ensure that the HTTP client adheres to traditional redirection practices and prevents the risks involved with automatic request redirection in unsafe operations.
## **Status Codes**
### **300 Multiple Choices**

**TODO**
- Implement handling for 300 (Multiple Choices) status code.
- When a 300 response is generated, include a list of representations with metadata and their URI references in the response payload.
- If there is a preferred choice, generate a `Location` header field with a URI reference of the preferred choice.
- Ensure that the 300 response is cacheable by default unless otherwise indicated by cache controls or method definition.

**Example Implementation of Automatic Selection:**
- For request methods other than HEAD, the user agent may automatically select a choice if it understands the media type provided in the response.
- Consider using a commonly accepted hypertext format for representation if automatic selection is viable.

**Considerations**
- Note that the original proposal for using URI and Alternates in a 300 response was not implemented due to syntax disagreement. Instead, consider using `Link` header fields with "alternate" relationships as per [RFC5988](https://www.rfc-editor.org/rfc/rfc5988).

### Reference Links
- Reactive negotiation: [RFC7231 Section 3.4](https://www.rfc-editor.org/rfc/rfc7231#section-3.4)
- Cache controls: [RFC7234 Section 4.2.2](https://www.rfc-editor.org/rfc/rfc7234#section-4.2.2)
## **301 Moved Permanently**

**TODO**
- Handle `301 Moved Permanently` status code
  - Ensure that the response includes a `Location` header field with the new permanent URI.
  - Implement automatic client-side redirection using the `Location` header field value, where applicable.
  - Recognize that a user-agent MAY change the request method from POST to GET unless a 307 is used instead.
  - Ensure that a 301 response is cacheable by default; incorporate relevant cache control mechanisms.
## **302 Found Status Handling**

**TODO** 
- Implement logic to handle 302 Found status code

  - **Description**: The 302 (Found) status code indicates that the target resource resides temporarily under a different URI.
  
  - **Client-side Recommendation**: Clients should continue using the original URI for future requests as the redirection might change.
  
  - **Server Responsibility**: 
    - **Generate** a `Location` header field in the response to provide a URI for the temporary redirection.
    - **Response Payload**: Include a brief hypertext note with a hyperlink to the new URI(s).

  - **User Agent Behavior**:
    - A user agent MAY change the request method from POST to GET for the redirected request.
    - If maintaining the original request method is desired, consider using the 307 (Temporary Redirect) status code instead.
   
- **Example** of handling logic in server code:
  
  - If 302 status is set, ensure there's a `Location` header in the HTTP response.
  - For POST requests, confirm whether a method switch to GET is permissible or if redirection via 307 should be enforced.

- **Considerations**: Implement logic in both client and server handlers to accommodate user agent behaviors described for 302 status codes.

___
# Status Codes

## **303 See Other**

**TODO**
- Implement handling for 303 (See Other) status code.
- Redirect the user agent to a different resource as indicated by the URI in the Location header field.
- Ensure handling allows for GET or HEAD retrieval requests targeting the URI provided in the Location header field.

**TODO**
- Implement behavior for client-side handling of 303 responses to POST requests.
- Ensure redirection to a resource that can be independently identified, bookmarked, and cached.

**TODO**
- Implement appropriate representation for 303 responses, especially for GET requests.
- Include a short hypertext note with a hyperlink to the same URI referenced in the Location header field, except for responses to HEAD requests.
**Status Codes**

**TODO**
- Avoid using the 305 (Use Proxy) status code as it has been deprecated
- `The 305 (Use Proxy) status code was defined in a previous version of this specification and is now deprecated.`

**TODO**
- Document deprecated status codes for reference
- `Appendix B of RFC 7230 lists deprecated status codes. Ensure to properly address or handle them during implementation to prevent unintended behavior.`
# Status Codes

## **306 (Unused)**
- **Note**: The 306 status code was defined in a previous version of this specification, is no longer used, and the code is reserved. 

**TODO**
- Ensure system does not generate 306 status code as it is obsolete. 
- Document that 306 status code is reserved for potential future use.
# Redirection Handling

## **307 Temporary Redirect**

**TODO**
- Implement handling for the 307 status code
  - `The 307 (Temporary Redirect) status code indicates that the target resource resides temporarily under a different URI, and the user agent MUST NOT change the request method if it performs an automatic redirection to that URI.`
  - Ensure the `Location` header field is correctly generated in the response for redirection.

**Notes**
- The main difference between 307 and 302 status codes is that 307 does not allow changing the request method from POST to GET during redirection.
- This status code is similar to 302 (Found) but should preserve the method and request body.

**Further Reading**
- For details on the 308 (Permanent Redirect), see [RFC7238](https://www.rfc-editor.org/rfc/rfc7238).
## **Client Error 4xx**

### **TODO**
- Implement logic to handle 4xx status codes appropriately.
- `The 4xx (Client Error) class of status code indicates that the client seems to have erred. Except when responding to a HEAD request, the server SHOULD send a representation containing an explanation of the error situation, and whether it is a temporary or permanent condition. These status codes are applicable to any request method. User agents SHOULD display any included representation to the user.`
- Ensure the server sends an explanatory representation for 4xx responses (except for HEAD requests).
- Logic to distinguish between temporary (e.g., 404 Not Found) and permanent client errors (e.g., 410 Gone).
- Ensure that representations are included with 4xx error responses and are visible to the user.
- Review relevant sections for specific 4xx status codes from [RFC7231 Section 6.5](https://www.rfc-editor.org/rfc/rfc7231#section-6.5) and implement their specific requirements.

## **HTTP Status Codes: 400 Bad Request**
**TODO**
- Implement logic to identify and handle client errors leading to 400 Bad Request
- `Ensure that requests are correctly parsed and validated. Report a 400 Bad Request status code if issues like malformed request syntax, invalid request message framing, or deceptive request routing are detected.`
- Update existing error-handling mechanisms to comply with the RFC guidelines for 400 Bad Request scenarios

- Examples of situations requiring a 400 Bad Request response:
  - Request contains invalid syntax elements
  - Errors in framing or constructing the request message
  - Any detected attempts of request forgery or deceptive routing

- Confirm logging mechanisms are in place for tracking occurrences of this status code to help diagnose client-supplied errors systematically.

- Client feedback:
  - Provide informative error messages to clients on receiving a 400 Bad Request, where feasible, to aid troubleshooting and correction of the error.
## **Status Codes**
**TODO**
- Monitor developments on the 402 Payment Required status code
- `The 402 (Payment Required) status code is reserved for future use. Handle appropriately if standardized or utilized in the future implementations.`


## **403 Forbidden Status Code** 

**TODO**
- Implement handling of 403 Forbidden status code
- `The 403 (Forbidden) status code indicates that the server understood the request but refuses to authorize it. A server that wishes to make public why the request has been forbidden can describe that reason in the response payload (if any).`
- `If authentication credentials were provided in the request, the server considers them insufficient to grant access. The client SHOULD NOT automatically repeat the request with the same credentials. The client MAY repeat the request with new or different credentials.`
- `An origin server that wishes to "hide" the current existence of a forbidden target resource MAY instead respond with a status code of 404 (Not Found).` 

**TODO**
- Design response payload structure to convey reasons for request refusal when using 403 status code
- Document cases when it might be appropriate to respond with 404 instead of 403
## **Status Code: 404 Not Found**

**TODO:**
- Implement 404 (Not Found) status code handling.
- Ensure that the server returns a 404 status code when it does not find a current representation for the target resource or is not willing to disclose its existence.
- Consider the 410 (Gone) status code if known that the lack of representation is likely permanent.

**Cache Handling:**
- By default, a 404 response should be cacheable unless stated otherwise by method definition or explicit cache control directives.
- Reference Section 4.2.2 of [RFC7234](https://www.rfc-editor.org/rfc/rfc7234#section-4.2.2) for detailed cache control mechanisms related to the 404 status code.
## **Status Codes**

**TODO**
- Handle 405 Method Not Allowed
- `The 405 (Method Not Allowed) status code indicates that the method received in the request-line is known by the origin server, but not supported by the target resource. Implement logic to generate an Allow header field in a 405 response containing a list of the target resource's currently supported methods.`
- Recognize that a 405 response is cacheable by default unless otherwise specified by method definition or explicit cache controls.
- Reference: https://www.rfc-editor.org/rfc/rfc7234#section-4.2.2
## **Status Codes**
**TODO**
- Implement handling for 406 (Not Acceptable) status code
- Ensure the server responds with a 406 status code when a requested resource does not have a representation acceptable to the client based on proactive negotiation header fields received in the request.

**TODO**
- Generate a payload for 406 responses containing:
  - A list of available representation characteristics
  - Corresponding resource identifiers for the user or user agent to choose the most appropriate representation

**Note**
- The user agent may automatically select the most appropriate choice from the provided list, but no standard for automatic selection is defined. Refer to [RFC 7231 Section 6.4.1](https://www.rfc-editor.org/rfc/rfc7231#section-6.4.1) for additional context.
## **Status Codes**

**TODO**
- Handle 408 (Request Timeout) status code
- Ensure that the server sends the "close" connection option in the response when a 408 status code is returned.
- Allow the client to repeat the request on a new connection if they have an outstanding request in transit.
# Status Codes
## **409 Conflict**
**TODO**
- Handle 409 Conflict status code
- `The 409 (Conflict) status code indicates that the request could not
   be completed due to a conflict with the current state of the target
   resource.  This code is used in situations where the user might be
   able to resolve the conflict and resubmit the request.`
- The server SHOULD generate a payload to help the user recognize the conflict
- Example: Common in response to a `PUT` request if changes conflict with earlier requests
- Payload should provide information for merging differences based on revision history
## **Status Codes**

**TODO**

- Implement handling for **410 (Gone)** status code
   - Description: The 410 status code indicates that the resource is permanently gone from the origin server.
   - Use this status code to assist web maintenance by notifying clients that a resource is intentionally unavailable.
   - If unsure about the permanence of the resource's unavailability, use the 404 (Not Found) status code instead.
   - Ensure the 410 response is cacheable by default, unless specified otherwise by explicit cache controls.
- Reference: [Section 6.5.9 of RFC 7231](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.9)
## **Status Codes**
**TODO**
- Handle the 411 (Length Required) status code
- `The 411 (Length Required) status code indicates that the server refuses to accept the request without a defined Content-Length.`
- `The client MAY repeat the request if it adds a valid Content-Length header field containing the length of the message body in the request message.`

- Implement logic to send a 411 status code response when a request is received without a Content-Length header field where it is required, as per [RFC7230 Section 3.3.2](https://www.rfc-editor.org/rfc/rfc7230#section-3.3.2).

## **Request Payload Limits**
**TODO**
- Implement logic for rejecting requests with payloads too large
- `The 413 (Payload Too Large) status code indicates that the server is
   refusing to process a request because the request payload is larger
   than the server is willing or able to process.`
- Consider server capability and configuration for payload size limits
- Implement logic to close connections when rejecting with 413
- `The server MAY close the connection to prevent the client from continuing the request.`

**TODO**
- Optionally handle the `Retry-After` header field for temporary conditions
- `If the condition is temporary, the server SHOULD generate a
   Retry-After header field to indicate that it is temporary and after
   what time the client MAY try again.`

Add these tasks to your implementation roadmap to ensure compliance with RFC 7231, Section 6.5.11.
# Additional TODOs

## **Status Codes**
**TODO**
- Implement handling for 414 (URI Too Long) status code.
- The server should return a 414 status code when the request-target is longer than the server is willing to interpret.
- Ensure the 414 response is cacheable by default, unless otherwise specified by method definition or explicit cache controls.
- Be aware of potential causes such as improperly converted POST requests, redirection loops, or attempted security exploits. 

**TODO**
- Test scenarios where clients improperly switch POST to GET with long query strings, hit recursive redirects, or when simulating attacks with overly long URLs. 

Add these TODOs to your markdown and share with the team to ensure comprehensive handling of URI length issues and appropriate status code responses according to RFC 7230 specifications.
## **Unsupported Media Type (415 Status Code)**
**TODO**
- Implement proper handling for the 415 status code
- `Ensure that the server refuses requests with payloads in a format not supported by the method on the target resource. Consider both the Content-Type and Content-Encoding headers as well as direct inspection of the payload data for determining unsupported formats. Return a 415 (Unsupported Media Type) status code in such cases.`
## **Expectation Handling**
**TODO**
- Handle `Expect` header field
- `The 417 (Expectation Failed) status code indicates that the
  expectation given in the request's Expect header field
  (RFC 7231 Section 5.1.1) could not be met by at least one of the inbound servers.`
- Reference: https://www.rfc-editor.org/rfc/rfc7231#section-5.1.1

**TODO**
- Respond with 417 status code if the `Expect` header field cannot be fulfilled
## **426 Upgrade Required Status Code**
**TODO**
- Implement logic for handling 426 status code
- `When sending a 426 (Upgrade Required) response, ensure that the server includes an Upgrade header field to specify the required protocol(s) that the client needs to upgrade to.`

**Example Response:**
```http
HTTP/1.1 426 Upgrade Required
Upgrade: HTTP/3.0
Connection: Upgrade
Content-Length: 53
Content-Type: text/plain

This service requires use of the HTTP/3.0 protocol.
```

**TODO**
- Define a mechanism to determine when an upgrade is necessary and ensure appropriate handling and response.
## **5xx (Server Error) Status Codes**
- `The 5xx (Server Error) class of status code indicates that the server is aware that it has erred or is incapable of performing the requested method. Except when responding to a HEAD request, the server SHOULD send a representation containing an explanation of the error situation, and whether it is a temporary or permanent condition. A user agent SHOULD display any included representation to the user. These response codes are applicable to any request method.`

**TODO**
- Implement logic to handle 5xx status codes: When returning a 5xx status code, include a representation explaining the error situation.
- Ensure representations provide information on whether the error condition is temporary or permanent.
- Special handling for HEAD requests: No representation should be sent with 5xx status codes for HEAD requests.
- Display server error representations to the user agent, if applicable.

**TODO**
- Ensure consistent application of 5xx status codes across different request methods.
# **Error Handling**

## **500 Internal Server Error**

**TODO**

- Implement logic to trigger a `500 Internal Server Error` response when the server encounters an unexpected condition.
- Ensure that any unhandled exceptions or unexpected conditions are logged appropriately for debugging and follow-up investigations.
- Implement a user-friendly error message to accompany the 500 status code to clearly communicate to users that an unexpected server error has occurred.
- `The 500 (Internal Server Error) status code indicates that the server
  encountered an unexpected condition that prevented it from fulfilling
  the request.`

**Implementation Suggestions**

- Create a centralized error-handling mechanism to process exceptions and trigger appropriate HTTP status codes and responses.
- Regularly monitor server logs for occurrences of the 500 status code to identify common causes and implement preventive measures.

**Resources**

- [RFC 7231 Section 6.6.1: 500 Internal Server Error](https://www.rfc-editor.org/rfc/rfc7231#section-6.6.1)

___

Continue this format to extract relevant actions for handling additional status codes, following the style used above.
# Mandatory

## **501 Not Implemented Status Code**

**TODO**
- Implement handling for 501 Not Implemented status code
  - `The server must return 501 status code if it does not recognize the request method or is incapable of supporting it for any resource.`
  - `Implement cacheability for 501 response by default unless otherwise specified by method definition or explicit cache controls.`

___
___

# HTTP Method Handling

**TODO**
- Recognize unsupported HTTP methods
  - `On encountering an unsupported or unrecognized HTTP method, respond with a 501 (Not Implemented) status code to indicate that the server does not support this functionality.`
## **Status Codes: 502 Bad Gateway**
**TODO**
- Implement handling for the 502 (Bad Gateway) status code
- `A server acting as a gateway or proxy that receives an invalid response from an upstream server should respond with a 502 status code. This informs the client that the problem is not with their request, but with the server that was contacted to answer the request.`

---
**TODO**
- Implement 503 (Service Unavailable) Status Code
- `The 503 (Service Unavailable) status code indicates that the server
   is currently unable to handle the request due to a temporary overload
   or scheduled maintenance, which will likely be alleviated after some
   delay.`

**TODO**
- Optionally include "Retry-After" header
- `The server MAY send a Retry-After header field to suggest an appropriate amount of time for the client to wait before retrying the request.`

**TODO**
- Choose appropriate server behavior when overloaded
- Note that servers might choose not to use the 503 status code and instead refuse the connection entirely.
## **Status Codes**

**TODO**
- Handle 504 Gateway Timeout
- `The 504 (Gateway Timeout) status code indicates that the server, while acting as a gateway or proxy, did not receive a timely response from an upstream server it needed to access in order to complete the request.`

### Instructions:
- Implement logic to handle a 504 status code in scenarios where the server is operating as a gateway or proxy.
- Ensure appropriate logging mechanisms are in place to diagnose the cause of the timeout. 
- Consider implementing retry logic to handle temporary network issues and minimize disruptions.  

---

**Additional Notes:**

- Remember to document your status code implementation strategy for easy reference and maintenance by the team.

---

By adhering to this approach, we ensure the reliability and transparency of our gateway and proxy communications, even in response to upstream server timeouts.
**TODO**
- Handle 505 (HTTP Version Not Supported) Status Code
- `The server should generate a response that explains why the HTTP version used in the request is not supported and list what other protocols are supported by that server.`
- https://www.rfc-editor.org/rfc/rfc7230#section-2.6
## **Response Header Fields**

**TODO**
- Process server and resource metadata from response headers
- `The response header fields provide additional information beyond what is present in the status-line, including details about the server, further access instructions for the target resource, or information about related resources.`

**TODO**
- Refine handling of response header fields based on request method and response status code
- `Each response header field can have its semantics modified based on the request method and/or response status code, making it important to contextualize header data accordingly.`

**TODO**
- Ensure proper handling and validation of response headers
- `Implement logic to confirm compliance with defined response header field meanings as per the RFC guidelines, and ensure that the handling of these headers aligns with both the method that was requested and the status code returned.`
## **Response Header Control Data**
Control data provided by response header fields can direct caching behavior, signal supplementary status information, or guide further client actions. The header fields include:

- **Age**: Defined in [RFC7234 Section 5.1](https://www.rfc-editor.org/rfc/rfc7234#section-5.1)
- **Cache-Control**: Defined in [RFC7234 Section 5.2](https://www.rfc-editor.org/rfc/rfc7234#section-5.2)
- **Expires**: Defined in [RFC7234 Section 5.3](https://www.rfc-editor.org/rfc/rfc7234#section-5.3)
- **Date**: Defined in [RFC7231 Section 7.1.1.2](https://www.rfc-editor.org/rfc/rfc7231#section-7.1.1.2)
- **Location**: Defined in [RFC7231 Section 7.1.2](https://www.rfc-editor.org/rfc/rfc7231#section-7.1.2)
- **Retry-After**: Defined in [RFC7231 Section 7.1.3](https://www.rfc-editor.org/rfc/rfc7231#section-7.1.3)
- **Vary**: Defined in [RFC7231 Section 7.1.4](https://www.rfc-editor.org/rfc/rfc7231#section-7.1.4)
- **Warning**: Defined in [RFC7234 Section 5.5](https://www.rfc-editor.org/rfc/rfc7234#section-5.5)

**TODO**
- Implement handlers for each control data header field as per the sections indicated.
## **HTTP Dates**
**TODO**
- Implement logic to generate a `Date` header field in response messages.
- `A received message that does not have a `Date` header field lacks an origination timestamp. In such cases, a recipient that caches or forwards the message downstream MUST append a corresponding `Date` header field with the current timestamp.`
- `Ensure all sent response messages, regardless of their status code, appropriately include the `Date` header field for proper message origination tracking.`
# Mandatory

## **Date/Time Handling**
**TODO**
- Implement logic to parse HTTP dates in all three formats
  - **IMF-fixdate**: Preferred format (e.g., Sun, 06 Nov 1994 08:49:37 GMT)
  - **RFC 850 format**: Obsolete format (e.g., Sunday, 06-Nov-94 08:49:37 GMT)
  - **ANSI C asctime() format**: Obsolete format (e.g., Sun Nov  6 08:49:37 1994)

- Ensure parsing accepts all formats and handles case sensitivity
  - `HTTP-date is case-sensitive. A sender MUST NOT generate additional whitespace in an HTTP-date beyond that specifically included as SP in the grammar.`

- Generate HTTP dates in **IMF-fixdate** format for HTTP headers
  - `When a sender generates a header field that contains one or more timestamps defined as HTTP-date, the sender MUST generate those timestamps in the IMF-fixdate format.`

- Interpret RFC 850 format two-digit years correctly
  - `Recipients of a timestamp value in rfc850-date format, which uses a two-digit year, MUST interpret a timestamp that appears to be more than 50 years in the future as representing the most recent year in the past that had the same last two digits.`

- Utilize network protocols like NTP to synchronize clock to UTC if generating HTTP-date values

- **Advice**: Remain flexible in parsing dates from non-HTTP sources and adhere to field-specific restrictions when parsing timestamps

___
___
# **Date Header Field**

## **TODO**

### **Date Header Field in Response**

- Implement logic to include the `Date` header field in server responses.

  - **Condition 1:** The server has a clock capable of providing reasonable UTC time.
    - **Action:** Send `Date` header in all responses except when:
      - Information response (1xx) or server error response (5xx) class, where the inclusion is optional.

  - **Condition 2:** The server lacks a reliable clock.
    - **Action:** Do not send the `Date` header field in responses.

### **Date Header Field in Request**

- Decide on cases where the client should send a `Date` header field in requests.

  - **Usage:** If application's logic requires sending the `Date`, ensure it's when such information benefits server-side processing (e.g., clock adjustments).

### **Handling Missing Date Header in Responses**

- Logic for recipients to manage messages missing a `Date` header.
  
  - **Have a Clock:** Record the reception time.
    - **Action:** Append a `Date` header field with the recorded time if caching or forwarding the message downstream.

## **General Notes**

- Ensure that the `Date` value adheres to the HTTP-date format, as specified in [RFC 7231 Section 7.1.1.1](https://www.rfc-editor.org/rfc/rfc7231#section-7.1.1.1).

- Example `Date` Header:
  ```http
  Date: Tue, 15 Nov 1994 08:12:31 GMT
  ```

## **References**
- [RFC5322 Section 3.6.1](https://www.rfc-editor.org/rfc/rfc5322#section-3.6.1) for origination date.
- [RFC7231 Section 7.1.1.1](https://www.rfc-editor.org/rfc/rfc7231#section-7.1.1.1) for HTTP-date specifications.
## **Location Header Field**
**TODO**
- Implement logic to correctly handle "Location" header field as per response type
- `For 201 (Created) responses, ensure that the Location value refers to the primary resource created by the request. For 3xx (Redirection) responses, verify that the Location value refers to the preferred target resource for redirecting the request.`

**TODO**
- Ensure proper redirection handling when Location value lacks a fragment
- `If the Location value in a 3xx (Redirection) response does not have a fragment component, the user agent MUST process the redirection as if the value inherits the fragment component of the URI reference used to generate the request target.`

**TODO**
- Implement logic to deal with invalid URI references in Location fields
- `Some recipients attempt to recover from Location fields that are not valid URI references. Though the specification does not mandate such processing, it allows it for the sake of robustness.`

**TODO**
- Distinguish between Location and Content-Location header fields
- `Understand that the Content-Location differs from Location as it refers to the most specific resource corresponding to the enclosed representation. It is possible for the response to contain both header fields.`
## **Retry-After Header**

**TODO**
- Implement logic to parse and handle the `Retry-After` header field appropriately.

  - The `Retry-After` header can specify the delay using either an HTTP-date or a number of seconds (delay-seconds).

  - When a server sends a `503 (Service Unavailable)` response with the `Retry-After` header, ensure that the client waits the indicated duration before retrying.

  - For any `3xx (Redirection)` response with the `Retry-After` header, enforce that the client waits for at least the specified time before issuing the redirected request.

  - If using HTTP-date, implement date parsing functionality.
  
  - If using delay-seconds, ensure it is parsed correctly as a non-negative decimal integer representing time in seconds.

- Consider the following examples:

  - `Retry-After: Fri, 31 Dec 1999 23:59:59 GMT` (An HTTP-date format)
  - `Retry-After: 120` (Indicates a 2-minute delay in seconds)
## **Vary Header Field**
**TODO**
- Implement support for parsing and interpreting the "Vary" header field.
- Recognize a single asterisk (*) or a list of header field names in the Vary field value.
- `Vary = "*" / 1#field-name`

**TODO**
- Ensure "Vary" header logic prevents the generation of a Vary field with a "*" value by proxies.
- `A proxy MUST NOT generate a Vary field with a "*" value.`

**TODO**
- Enhance cache validation using "Vary"
    - Implement logic to ensure that cached responses are only used for requests with matching values in the selecting header fields.
    - `Vary expands the cache key required to match a new request to the stored cache entry.`

**TODO**
- Encourage content negotiation awareness with "Vary"
    - Implement functionality to adjust response representations when header fields vary.
    - `This informs user agents that different representations might be sent in a subsequent request if different parameters are provided in the listed header fields.`

**TODO**
- Decide when not to send "Vary"
    - Assess whether the selection variance across requests can be avoided or is unnecessary for caching.
    - Consider using Cache-Control directives instead, if appropriate.
- `There is no need to send the Authorization field name in Vary because reuse across users is constrained by the field definition.`
## **Validator Header Fields**

**TODO**
- Implement handling for validator header fields like `ETag` and `Last-Modified`
  - `ETag`: The 'ETag' header field is used to communicate the entity-tag of a resource and it should be used in conditional requests to prevent overwriting changes accidentally.
  - `Last-Modified`: This header communicates the date and time at which the origin server believes the resource was last modified.
  - Reference: [RFC7232 Section 2.2 (Last-Modified)](https://www.rfc-editor.org/rfc/rfc7232#section-2.2) and [RFC7232 Section 2.3 (ETag)](https://www.rfc-editor.org/rfc/rfc7232#section-2.3)

**TODO**
- Ensure validator header fields are correctly interpreted to support cache validation and conditional requests
  
**TODO**
- Review the `If-Match`, `If-None-Match`, `If-Modified-Since`, and `If-Unmodified-Since` headers 
  - Ensure server behavior aligns with [RFC7232](https://www.rfc-editor.org/rfc/rfc7232) for handling requests using these headers

**TODO**
- If applicable, log scenarios where validator fields effectively prevent "lost update" issues for auditing purposes
## **Authentication Challenges**

**TODO** 
- Implement support for WWW-Authenticate and Proxy-Authenticate headers
- `Authentication challenges indicate what mechanisms are available for the client to provide authentication credentials in future requests.`

### References
- **WWW-Authenticate**: For more details, refer to [Section 4.1 of RFC 7235](https://www.rfc-editor.org/rfc/rfc7235#section-4.1)
- **Proxy-Authenticate**: For more details, refer to [Section 4.3 of RFC 7235](https://www.rfc-editor.org/rfc/rfc7235#section-4.3)
## **Response Header Fields**

**TODO**
- Implement support for the `Accept-Ranges` Header Field
  - Reference: [Section 2.3 of RFC7233](https://www.rfc-editor.org/rfc/rfc7233#section-2.3)
  - Purpose: Indicates the range units accepted by the server.

**TODO**
- Implement support for the `Allow` Header Field
  - Reference: [Section 7.4.1 of RFC7231](https://www.rfc-editor.org/rfc/rfc7231#section-7.4.1)
  - Purpose: Lists the set of methods supported by the target resource.

**TODO**
- Implement support for the `Server` Header Field
  - Reference: [Section 7.4.2 of RFC7231](https://www.rfc-editor.org/rfc/rfc7231#section-7.4.2)
  - Purpose: Contains information about the software used by the origin server fulfilling the request.
## **Allow Header**
**TODO**
- Implement Allow header functionality 
- `The "Allow" header field lists the set of methods advertised as supported by the target resource.`
- Ensure the server generates an Allow field in a 405 (Method Not Allowed) response
  - `An origin server MUST generate an Allow field in a 405 (Method Not Allowed) response and MAY do so in any other response.`
- Handle empty Allow field appropriately
  - `An empty Allow field value indicates that the resource allows no methods, which might occur in a 405 response if the resource has been temporarily disabled by configuration.`
- Ensure proxies do not modify the Allow header field
  - `A proxy MUST NOT modify the Allow header field -- it does not need to understand all of the indicated methods in order to handle them according to the generic message handling rules.`
## **Server Header Field**
- `The "Server" header field contains information about the software used by the origin server to handle the request.`

**TODO**
- Implement logic to generate the "Server" header field
- `An origin server MAY generate a Server field in its responses.`

**TODO**
- Ensure that the Server field consists of product identifiers with optional versioning
- `The Server field-value consists of one or more product identifiers, each followed by zero or more comments.`

**TODO**
- Do not include overly detailed information in the Server field 
- Limit the addition of subproducts by third parties
- `An origin server SHOULD NOT generate a Server field containing needlessly fine-grained detail.`

**TODO**
- Implement ordering for product identifiers based on significance
- `By convention, the product identifiers are listed in decreasing order of their significance for identifying the origin server software.`


# Mandatory
## **IANA Considerations**
**TODO**
- Register relevant parameters
- `When developing new protocols or extending existing ones, ensure that you register any relevant parameters with IANA. This could include new HTTP status codes, header fields, or URI schemes.`

- Verify existing registrations
- `Periodically verify that your implementation's current protocols and extensions are properly registered with IANA to ensure compliance with Internet standards.`

- Follow best practices for new registrations
- `When registering new parameters with IANA, follow the guidelines and best practices outlined in RFC 7230 and related documents to ensure smooth integration and interoperability with existing systems.`

**Reference Material**
- [IANA HTTP Parameters](https://www.iana.org/assignments/http-parameters/http-parameters.xhtml)

By handling IANA considerations thoughtfully, you mitigate the risks of conflicts in parameter assignments and ensure robust and interoperable protocol implementations.
## **HTTP Method Registry**
**TODO**
- Integrate HTTP Method Registry into the system
- `The "Hypertext Transfer Protocol (HTTP) Method Registry" defines the
   namespace for the request method token. The method registry has been created and is now maintained at <http://www.iana.org/assignments/http-methods>.`

**TODO**
- Ensure standard HTTP methods are supported, following the registry

**TODO**
- Implement proper validation for method tokens, adhering to standards defined in the registry
- `A sender MUST NOT use method tokens that are not registered or recognized.`

**TODO**
- Log usage of unrecognized or unregistered HTTP methods
- `A server that receives an unknown method SHOULD log the incident and MAY respond with a 501 (Not Implemented) status code.`


## **HTTP Method Registration**

**TODO** 
- Ensure HTTP method registrations contain the following fields:
  - **Method Name**: Refer to [Section 4](https://www.rfc-editor.org/rfc/rfc7231#section-4)
  - **Safe**: Indicate "yes" or "no" as per [Section 4.2.1](https://www.rfc-editor.org/rfc/rfc7231#section-4.2.1)
  - **Idempotent**: Indicate "yes" or "no" as per [Section 4.2.2](https://www.rfc-editor.org/rfc/rfc7231#section-4.2.2)
  - **Pointer to specification text**: Provide a reference or link to the relevant specification.

**TODO** 
- Review values added to the namespace in compliance with the IETF Review process as outlined in [RFC5226, Section 4.1](https://www.rfc-editor.org/rfc/rfc5226#section-4.1).
# Considerations for New HTTP Methods

## **Standardization**
**TODO**
- Ensure new methods are generic and applicable to any resource, not linked to a specific media type or application.

## **Message Parsing**
**TODO**
- Maintain message parsing independence from method semantics. New methods must not change the parsing algorithm, nor prohibit message bodies on request or response, except allowing Content-Length: 0 for zero-length bodies.

## **Safety, Idempotency, and Caching**
**TODO**
- Define if the new method is:
  - Safe ([Section 4.2.1](https://www.rfc-editor.org/rfc/rfc7231#section-4.2.1))
  - Idempotent ([Section 4.2.2](https://www.rfc-editor.org/rfc/rfc7231#section-4.2.2))
  - Cacheable ([Section 4.2.3](https://www.rfc-editor.org/rfc/rfc7231#section-4.2.3))
  
**TODO**
- If cacheable, document how caches store responses and reuse them for subsequent requests.

## **Payload and Semantics**
**TODO**
- Clarify payload body semantics.
- Address refinements to header fields or status code semantics.

## **Conditional Requests**
**TODO**
- Describe if the method can be conditional, and define server behavior when conditions are false ([Section 5.2](https://www.rfc-editor.org/rfc/rfc7231#section-5.2)).

## **Partial Responses**
**TODO**
- Document potential use for partial response semantics ([RFC7233](https://www.rfc-editor.org/rfc/rfc7233)).

## **Naming**
**TODO**
- Avoid using method names starting with "M-" to prevent misinterpretation with [RFC2774](https://www.rfc-editor.org/rfc/rfc2774) semantics.

___

# Mandatory

## **HTTP Methods**
**TODO**
- Implement method registration validation logic
- Register the HTTP methods in your server application using the guidelines from the "HTTP Method Registry".

### Method Characteristics
- CONNECT: `Safe = no, Idempotent = no` [Reference](https://www.rfc-editor.org/rfc/rfc7231#section-4.3.6)
- DELETE: `Safe = no, Idempotent = yes` [Reference](https://www.rfc-editor.org/rfc/rfc7231#section-4.3.5)
- GET: `Safe = yes, Idempotent = yes` [Reference](https://www.rfc-editor.org/rfc/rfc7231#section-4.3.1)
- HEAD: `Safe = yes, Idempotent = yes` [Reference](https://www.rfc-editor.org/rfc/rfc7231#section-4.3.2)
- OPTIONS: `Safe = yes, Idempotent = yes` [Reference](https://www.rfc-editor.org/rfc/rfc7231#section-4.3.7)
- POST: `Safe = no, Idempotent = no` [Reference](https://www.rfc-editor.org/rfc/rfc7231#section-4.3.3)
- PUT: `Safe = no, Idempotent = yes` [Reference](https://www.rfc-editor.org/rfc/rfc7231#section-4.3.4)
- TRACE: `Safe = yes, Idempotent = yes` [Reference](https://www.rfc-editor.org/rfc/rfc7231#section-4.3.8)

**TODO**
- Implement server logic to respect `Safe` and `Idempotent` properties of HTTP methods.
# Status Code Registry

**TODO**
- Reference the HTTP Status Code Registry 
  - `The "Hypertext Transfer Protocol (HTTP) Status Code Registry" defines the namespace for the response status-code token as described in [RFC7231 Section 6](https://www.rfc-editor.org/rfc/rfc7231#section-6). This registry is maintained at [IANA HTTP Status Codes](http://www.iana.org/assignments/http-status-codes).`
  
**TODO**
- Review and potentially update existing status code handling logic
  - `This section updates the registration procedure for HTTP Status Codes previously defined in Section 7.1 of [RFC2817](https://www.rfc-editor.org/rfc/rfc2817#section-7.1). Review this document and ensure compliance with the updated registration process.`

## **HTTP Status Code Registration**
**TODO**
- Ensure registration includes all required fields:
  - **Status Code**: 3 digits
  - **Short Description**: Brief description of the status code
  - **Pointer to Specification Text**: Reference to the related specification
- `Values to be added to the HTTP status code namespace require IETF Review.`

- Familiarize with [RFC5226 Section 4.1](https://www.rfc-editor.org/rfc/rfc5226#section-4.1) for procedural details on IETF Review. 

**TODO**
- Implement a “status code registry” function to streamline addition and management of new status codes, ensuring compliance with IETF procedures.
## **New Status Codes**

**TODO**
- Considerations for Proposing New Status Codes
  - Ensure new status codes are generic and not specific to a particular application or resource type.
  - Register new status codes in a non-application-specific document.
  - Ensure new status codes fall under one of the existing categories as per [RFC7231 Section 6](https://www.rfc-editor.org/rfc/rfc7231#section-6).
  - Allow existing parsers to process the response message without disallowing a payload, though a zero-length payload body can be mandated.

**Proposals**
- Avoid allocating specific numbers to new status codes in early drafts; use placeholders like "4NN" or "3N0" .. "3N9" instead.
- Clearly explain request conditions leading to the new status code, along with any dependencies on or interactions with response header fields.

**Cache Considerations**
- Specify the cacheability of the new status code:
  - An explicit mention if the status code can be cached without explicit freshness information.
  - Refer to [RFC7234](https://www.rfc-editor.org/rfc/rfc7234) for more on cache behavior.

**Resource Association**
- Define whether a payload associated with the new status code implies an association with an identified resource, referencing [RFC7231 Section 3.1.4.1](https://www.rfc-editor.org/rfc/rfc7231#section-3.1.4.1) for guidance.
## **Status Codes Registration**

**TODO**
- Update the status code registry in your application to include the following entries:

| Value | Description                   | Reference      |
|-------|-------------------------------|----------------|
| 100   | Continue                      | [Section 6.2.1](https://www.rfc-editor.org/rfc/rfc7231#section-6.2.1)  |
| 101   | Switching Protocols           | [Section 6.2.2](https://www.rfc-editor.org/rfc/rfc7231#section-6.2.2)  |
| 200   | OK                            | [Section 6.3.1](https://www.rfc-editor.org/rfc/rfc7231#section-6.3.1)  |
| 201   | Created                       | [Section 6.3.2](https://www.rfc-editor.org/rfc/rfc7231#section-6.3.2)  |
| 202   | Accepted                      | [Section 6.3.3](https://www.rfc-editor.org/rfc/rfc7231#section-6.3.3)  |
| 203   | Non-Authoritative Information | [Section 6.3.4](https://www.rfc-editor.org/rfc/rfc7231#section-6.3.4)  |
| 204   | No Content                    | [Section 6.3.5](https://www.rfc-editor.org/rfc/rfc7231#section-6.3.5)  |
| 205   | Reset Content                 | [Section 6.3.6](https://www.rfc-editor.org/rfc/rfc7231#section-6.3.6)  |
| 300   | Multiple Choices              | [Section 6.4.1](https://www.rfc-editor.org/rfc/rfc7231#section-6.4.1)  |
| 301   | Moved Permanently             | [Section 6.4.2](https://www.rfc-editor.org/rfc/rfc7231#section-6.4.2)  |
| 302   | Found                         | [Section 6.4.3](https://www.rfc-editor.org/rfc/rfc7231#section-6.4.3)  |
| 303   | See Other                     | [Section 6.4.4](https://www.rfc-editor.org/rfc/rfc7231#section-6.4.4)  |
| 305   | Use Proxy                     | [Section 6.4.5](https://www.rfc-editor.org/rfc/rfc7231#section-6.4.5)  |
| 306   | (Unused)                      | [Section 6.4.6](https://www.rfc-editor.org/rfc/rfc7231#section-6.4.6)  |
| 307   | Temporary Redirect            | [Section 6.4.7](https://www.rfc-editor.org/rfc/rfc7231#section-6.4.7)  |
| 400   | Bad Request                   | [Section 6.5.1](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.1)  |
| 402   | Payment Required              | [Section 6.5.2](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.2)  |
| 403   | Forbidden                     | [Section 6.5.3](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.3)  |
| 404   | Not Found                     | [Section 6.5.4](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.4)  |
| 405   | Method Not Allowed            | [Section 6.5.5](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.5)  |
| 406   | Not Acceptable                | [Section 6.5.6](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.6)  |
| 408   | Request Timeout               | [Section 6.5.7](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.7)  |
| 409   | Conflict                      | [Section 6.5.8](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.8)  |
| 410   | Gone                          | [Section 6.5.9](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.9)  |
| 411   | Length Required               | [Section 6.5.10](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.10) |
| 413   | Payload Too Large             | [Section 6.5.11](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.11) |
| 414   | URI Too Long                  | [Section 6.5.12](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.12) |
| 415   | Unsupported Media Type        | [Section 6.5.13](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.13) |
| 417   | Expectation Failed            | [Section 6.5.14](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.14) |
| 426   | Upgrade Required              | [Section 6.5.15](https://www.rfc-editor.org/rfc/rfc7231#section-6.5.15) |
| 500   | Internal Server Error         | [Section 6.6.1](https://www.rfc-editor.org/rfc/rfc7231#section-6.6.1)  |
| 501   | Not Implemented               | [Section 6.6.2](https://www.rfc-editor.org/rfc/rfc7231#section-6.6.2)  |
| 502   | Bad Gateway                   | [Section 6.6.3](https://www.rfc-editor.org/rfc/rfc7231#section-6.6.3)  |
| 503   | Service Unavailable           | [Section 6.6.4](https://www.rfc-editor.org/rfc/rfc7231#section-6.6.4)  |
| 504   | Gateway Timeout               | [Section 6.6.5](https://www.rfc-editor.org/rfc/rfc7231#section-6.6.5)  |
| 505   | HTTP Version Not Supported    | [Section 6.6.6](https://www.rfc-editor.org/rfc/rfc7231#section-6.6.6)  |
## **Header Field Registry**

**TODO**
- Register any new custom HTTP header fields 
- Utilize the "Message Headers" registry located at [http://www.iana.org/assignments/message-headers](http://www.iana.org/assignments/message-headers) in accordance with [BCP90](https://www.rfc-editor.org/rfc/rfc7231#ref-BCP90).
- Ensure compliance with registration procedures for any custom or proprietary header fields intended for interoperability to avoid conflicts. 

___
## **New Header Fields Considerations**

### **General Guidelines**
**TODO**
- Define new header field names without the "X-" prefix unless for proprietary use.
- Utilize ABNF syntax ([RFC5234](https://www.rfc-editor.org/rfc/rfc5234)) for defining new header field values.
- Use encoding like [RFC5987](https://www.rfc-editor.org/rfc/rfc5987) for fields requiring a larger character range.

### **Field Syntax**
**TODO** 
- Ensure leading and trailing whitespaces are removed upon parsing.
- Use quoted-string syntax for field values where leading or trailing whitespace is significant ([RFC7230 Section 3.2.6](https://www.rfc-editor.org/rfc/rfc7230#section-3.2.6)).
- Protect components that may contain commas with double-quotes using the quoted-string ABNF production.

### **Considerations for Defining New Header Fields**
**TODO**
- Determine if the field allows a single value or a list (comma-delimited).
- Specify conditions under which the field can be used (requests, responses, particular methods, etc.).
- Identify if the field is to be stored by origin servers upon a PUT request.
- Consider if the field semantics are refined by context (request methods, status codes).
- Decide if the field-name should be included in the Connection header field ([RFC7230 Section 6.1](https://www.rfc-editor.org/rfc/rfc7230#section-6.1)).

**TODO**
- Clarify conditions permitting intermediaries to insert, delete, or modify the field's value.
- Assess if it's appropriate to list the field-name in a Vary response header ([RFC7231 Section 7.1.4](https://www.rfc-editor.org/rfc/rfc7231#section-7.1.4)).
- Evaluate if the field is useful or permissible in trailers ([RFC7230 Section 4.1](https://www.rfc-editor.org/rfc/rfc7230#section-4.1)).
- Determine if the field should be maintained across redirects.
- Analyze any additional security considerations (e.g., disclosure of privacy-related data).
## **Message Headers Registration**

**TODO** 
- Register the following message headers as per their updated status:

  - `Accept`: 
    - Protocol: `http`
    - Status: `standard`
    - Reference: [Section 5.3.2](https://www.rfc-editor.org/rfc/rfc7231#section-5.3.2)
  
  - `Accept-Charset`: 
    - Protocol: `http`
    - Status: `standard`
    - Reference: [Section 5.3.3](https://www.rfc-editor.org/rfc/rfc7231#section-5.3.3)
  
  - `Accept-Encoding`: 
    - Protocol: `http`
    - Status: `standard`
    - Reference: [Section 5.3.4](https://www.rfc-editor.org/rfc/rfc7231#section-5.3.4)
  
  - `Accept-Language`: 
    - Protocol: `http`
    - Status: `standard`
    - Reference: [Section 5.3.5](https://www.rfc-editor.org/rfc/rfc7231#section-5.3.5)
  
  - `Allow`: 
    - Protocol: `http`
    - Status: `standard`
    - Reference: [Section 7.4.1](https://www.rfc-editor.org/rfc/rfc7231#section-7.4.1)

  - `Content-Encoding`:
    - Protocol: `http`
    - Status: `standard`
    - Reference: [Section 3.1.2.2](https://www.rfc-editor.org/rfc/rfc7231#section-3.1.2.2)

  - `Content-Language`:
    - Protocol: `http`
    - Status: `standard`
    - Reference: [Section 3.1.3.2](https://www.rfc-editor.org/rfc/rfc7231#section-3.1.3.2)

  - `Content-Location`:
    - Protocol: `http`
    - Status: `standard`
    - Reference: [Section 3.1.4.2](https://www.rfc-editor.org/rfc/rfc7231#section-3.1.4.2)

  - `Content-Type`:
    - Protocol: `http`
    - Status: `standard`
    - Reference: [Section 3.1.1.5](https://www.rfc-editor.org/rfc/rfc7231#section-3.1.1.5)

  - `Date`:
    - Protocol: `http`
    - Status: `standard`
    - Reference: [Section 7.1.1.2](https://www.rfc-editor.org/rfc/rfc7231#section-7.1.1.2)

  - `Expect`:
    - Protocol: `http`
    - Status: `standard`
    - Reference: [Section 5.1.1](https://www.rfc-editor.org/rfc/rfc7231#section-5.1.1)

  - `From`:
    - Protocol: `http`
    - Status: `standard`
    - Reference: [Section 5.5.1](https://www.rfc-editor.org/rfc/rfc7231#section-5.5.1)

  - `Location`:
    - Protocol: `http`
    - Status: `standard`
    - Reference: [Section 7.1.2](https://www.rfc-editor.org/rfc/rfc7231#section-7.1.2)

  - `Max-Forwards`:
    - Protocol: `http`
    - Status: `standard`
    - Reference: [Section 5.1.2](https://www.rfc-editor.org/rfc/rfc7231#section-5.1.2)

  - `MIME-Version`:
    - Protocol: `http`
    - Status: `standard`
    - Reference: [Appendix A.1](https://www.rfc-editor.org/rfc/rfc7231#appendix-A.1)

  - `Referer`:
    - Protocol: `http`
    - Status: `standard`
    - Reference: [Section 5.5.2](https://www.rfc-editor.org/rfc/rfc7231#section-5.5.2)

  - `Retry-After`:
    - Protocol: `http`
    - Status: `standard`
    - Reference: [Section 7.1.3](https://www.rfc-editor.org/rfc/rfc7231#section-7.1.3)

  - `Server`:
    - Protocol: `http`
    - Status: `standard`
    - Reference: [Section 7.4.2](https://www.rfc-editor.org/rfc/rfc7231#section-7.4.2)

  - `User-Agent`:
    - Protocol: `http`
    - Status: `standard`
    - Reference: [Section 5.5.3](https://www.rfc-editor.org/rfc/rfc7231#section-5.5.3)

  - `Vary`:
    - Protocol: `http`
    - Status: `standard`
    - Reference: [Section 7.1.4](https://www.rfc-editor.org/rfc/rfc7231#section-7.1.4)

- Ensure the change controller for the above registrations is set to: "IETF (iesg@ietf.org) - Internet Engineering Task Force".
# HTTP Content Coding
**TODO**
- Familiarize with "HTTP Content Coding Registry"
- **Action**: Review the namespace for content coding names
- **Reference**: https://www.rfc-editor.org/rfc/rfc7230#section-4.2
- **Resource**: Find the registry at http://www.iana.org/assignments/http-parameters

**TODO**
- Implement support for recognizing and appropriately handling content codings based on the registry.
- Ensure consistency with the content coding registry for interoperability with other HTTP implementations.
# **Content Coding Registration**

## **Content Coding Procedure**

**TODO**

- Ensure all content coding registrations include the necessary fields:
  - **Name**
  - **Description**
  - **Pointer to specification text**

- Verify that content coding names do not overlap with transfer coding names unless they represent the same encoding transformation as defined in the specification.

**TODO**

- Implement a mechanism to ensure values added to the content coding namespace require IETF Review and conform to the defined purpose of content coding.
  
References:
- Content coding and transfer coding distinction, refer to [RFC7230 Section 4](https://www.rfc-editor.org/rfc/rfc7230#section-4) for more details.
- Review procedures outlined in [RFC5226 Section 4.1](https://www.rfc-editor.org/rfc/rfc5226#section-4.1).
## **Content Codings**
**TODO**
- Implement content encoding registry
- `The "HTTP Content Coding Registry" needs to be updated to include the new registrations.`
- Ensure to handle "identity" as a synonym for "no encoding" in Accept-Encoding.
- Reference: [RFC 7231, Section 5.3.4](https://www.rfc-editor.org/rfc/rfc7231#section-5.3.4)
## Security Considerations

**TODO**

- Review known security concerns relevant to HTTP semantics and address considerations related to message syntax, parsing, and routing.
  - Reference Section 9 of [RFC7230](https://www.rfc-editor.org/rfc/rfc7230#section-9) for detailed insights.
  
**TODO**

- Ensure secure implementation on the server-side applications behind the HTTP interface.
  
**TODO**

- Enhance the secure processing of payloads received via HTTP in user agents.

**TODO**

- Follow best practices for the secure use of the Internet when implementing HTTP.

**TODO**

- Regularly consult with organizations like [OWASP](https://www.rfc-editor.org/rfc/rfc7231#ref-OWASP) for up-to-date research and guidelines on web application security.


# Security Considerations
## **Attacks Based on File and Path Names**
**TODO**
- Implement checks to prevent directory traversal attacks
  - Ensure the server does not resolve `..` as a valid path component for accessing resources above the intended directory
  - Reject or sanitize any incoming request URIs containing special names that might be misinterpreted by the file system

**TODO**
- Introduce validation for invalid or unexpected characters
  - Inspect request URIs and file names for invalid or unexpected characters that may be interpreted differently by the local file system
  - Implement comprehensive validation logic to defend against file path manipulation

**TODO**
- Analyze behavior of the local storage system regarding character normalization
  - Investigate how the local system handles decomposed characters and case-normalized names
  - Implement logic to enforce strict character encoding and normalization rules to prevent inconsistencies leading to security vulnerabilities

**TODO**
- Implement logging and monitoring for unusual file system access patterns
  - Integrate monitoring tools to alert on suspicious access attempts, particularly attempts to read or write to unexpected system paths or devices

**TODO**
- Conduct regular security audits on request URI processing
  - Regularly test mappings from request URIs to local file resources to ensure no inadvertent exposure of sensitive files or directories
# Security Considerations: Prevention of Injection Attacks 

## **Command, Code, or Query Injection Attacks**

**TODO** 
- Review and enhance security measures to prevent injection attacks
  - Ensure data received in HTTP requests (method, request-target, header fields, or body) is validated and sanitized before usage.

**TODO**
- Avoid directly using request data within instructions or commands
  - Compare incoming parameters against fixed strings and take actions based on those comparisons.
  
**TODO**
- Filter or encode data before processing 
  - Implement data filtering and encoding to prevent untrusted data from being interpreted as commands or scripts.

**TODO**
- Implement protections for stored request data
  - Ensure any logged or stored request data is handled securely and is not vulnerable to injection attacks.
  - Use appropriate encoding when storing request data to avoid execution of embedded scripts.

---

Each of these action points will contribute to preventing vulnerabilities such as SQL injection, ensuring the security of HTTP request handling processes.
# Privacy Considerations

## **Disclosure of Personal Information**

Clients handle significant amounts of personal information, which includes data provided by the user, such as names, locations, mail addresses, passwords, encryption keys, and browsing activities like history and bookmarks.

**TODO**
- Assess and implement strategies to prevent unintentional disclosure of personal information within our HTTP communication.
- Review data handling and storage processes to ensure that sensitive information is secure and not included in requests or exposed through HTTP headers inadvertently.
- Implement mechanisms to anonymize or minimize personal data exposure when making requests or logging server activities.

**TODO**
- Review existing security practices to enhance data privacy and integrity, focusing on encrypting sensitive information and managing access controls effectively.
## Disclosure of Sensitive Information in URIs

**TODO**
- Avoid sensitive data in URIs
- `Ensure that sensitive, personally identifiable, or risky information is never included in URIs, as URIs are typically shared or stored in ways that can be accessed by third parties, such as logs and bookmarks.`

**TODO**
- Utilize POST-based form submission for sensitive data
- `Authors of services should avoid using GET-based forms for submitting sensitive data. Instead, use POST-based form submission to prevent exposure of sensitive data in URIs.`

**TODO**
- Limit exposure via Referer header
- `Review and adhere to the limitations on the Referer header field as outlined in [Section 5.5.2 of RFC 7231](https://www.rfc-editor.org/rfc/rfc7231#section-5.5.2) to minimize the potential for revealing information about the user's browsing history or personal data.`
## **Fragment Identifiers and Redirects**

**TODO**
- Be aware of the potential disclosure of fragment identifiers when handling redirects.
- Ensure that user agents and any scripts or extensions are aware of the visibility of fragment identifiers.
- `Implement checks to ensure that when a redirect occurs and the original request's fragment identifier is inherited by a new reference in the Location header, it does not unintentionally disclose information across sites.`
- `If the original site uses personal information in fragment identifiers, ensure that redirects to other sites include a fragment component (even if empty) to block inheritance of potentially sensitive data.`
- Reference: [RFC 7231 Section 7.1.2](https://www.rfc-editor.org/rfc/rfc7231#section-7.1.2)
## **Header Information Disclosure**

**TODO**
- Evaluate and implement strategies to minimize User-Agent, Via, and Server header fields exposure to reduce potential security risks.
- Implement proxy precautions to replace sensitive machine names with pseudonyms in the Via header field, ensuring hosts behind a firewall are not inadvertently disclosed.
- Review current handling and logging of these headers to ensure compliance with best practices regarding information disclosure.

- Reference:
  - User-Agent: [RFC7231, Section 5.5.3](https://www.rfc-editor.org/rfc/rfc7231#section-5.5.3)
  - Via: [RFC7230, Section 5.7.1](https://www.rfc-editor.org/rfc/rfc7230#section-5.7.1)
  - Server: [RFC7231, Section 7.4.2](https://www.rfc-editor.org/rfc/rfc7231#section-7.4.2)
# Browser Fingerprinting

## **Privacy Concerns and Mitigation**
**TODO** 
- Evaluate header fields for fingerprinting potential:
  - Analyze headers such as `User-Agent`, `Accept`, `Accept-Charset`, `Accept-Encoding`, and `Accept-Language` for details that could contribute to fingerprinting.

**TODO**
- Implement configuration options to minimize fingerprinting risk:
  - Limit or anonymize details in headers that may aid identification.
  - Allow users to adjust the level of detail in proactive negotiation headers.

**TODO**
- Consider user consent for disclosure of personal information:
  - Use interaction methods to seek permission before sending potentially sensitive information, like `Accept-Language`.

**TODO**
- Implement Privacy Measures:
  - Propose user interfaces or experiences that inform users about privacy implications linked to detailed header information.
  - Consider working with proxy solutions to filter or anonymize headers where feasible.

**TODO**
- Develop a mechanism to maintain a whitelist of sites for selective headers:
  - Implement a system where `Accept-Language` and similar headers are only sent to whitelisted domains to maintain user privacy. 

---
Consider integrating these TODOs into privacy-related feature development to enhance user trust and mitigate fingerprinting risks. Implement these tasks as features in the user interface to ensure transparency and user control over shared information.
