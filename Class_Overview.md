# C++ Class Structure for HTTP Server Project

This project is about writing your own HTTP server. The following C++ classes are designed to meet the project’s requirements.

## 1. Server Class (Vincent)
**Purpose**: Manages the main logic for accepting connections, reading requests, and sending responses.

### Key Responsibilities:
- Listening on multiple ports.
- Handling incoming client connections using `poll()` or its equivalent.
- Managing non-blocking I/O operations.
- Parsing the configuration file to set up the server's behavior.

### Methods:
- `start()`: Initializes the server and begins listening.
- `handleClient(int client_fd)`: Handles requests and responses.
- `stop()`: Stops the server.

---

## 2. ClientConnection Class (Vincent)
**Purpose**: Manages individual client connections.

### Key Responsibilities:
- Reading requests (GET, POST, DELETE).
- Writing responses based on the HTTP status code.
- Managing timeouts and non-blocking I/O for each client.

### Methods:
- `processRequest()`: Reads and parses HTTP requests.
- `processResponse()`: Sends the appropriate HTTP response.
- `processUpload()`: Manages file uploads if the client uses POST.
- `disconnect()`: Cleans up when the client disconnects.

---

## 3. Request Class (Vincent)
**Purpose**: Parses and represents an incoming HTTP request.

### Key Responsibilities:
- Parsing HTTP request headers.
- Storing request data (e.g., method, URL, headers, and body).

### Methods:
- `parse()`: Parses raw data into the HTTP components.
- `getMethod()`: Returns the HTTP method (GET, POST, DELETE).
- `getURL()`: Extracts the requested URL.

---

## 4. Response Class (Vincent)
**Purpose**: Constructs and formats the HTTP response.

### Key Responsibilities:
- Generating HTTP headers and content.
- Handling status codes and error pages.

### Methods:
- `setstatus_code(int code)`: Sets the response status code.
- `addHeader(const std::string& name, const std::string& value)`: Adds HTTP headers.
- `setBody(const std::string& body)`: Sets the response body.

---

## 5. ConfigParser Class
**Purpose**: Parses the server configuration file.

### Key Responsibilities:
- Reading configuration details (e.g., ports, error pages, body size limits).

### Methods:
- `parse(const std::string& file_path)`: Reads and parses the configuration file.
- `getPort()`: Retrieves the configured port number.
- `getHost()`: Retrieves the host address.

---

## 6. CGIHandler Class
**Purpose**: Executes CGI scripts for dynamic content (e.g., PHP, Python).

### Key Responsibilities:
- Handling CGI execution using `fork()` for POST requests.
- Passing environment variables and request body to CGI.

### Methods:
- `executeCGI(const std::string& scriptPath)`: Executes the CGI script.

---

## 7. FileManager Class
**Purpose**: Handles file-related operations such as serving static content and managing file uploads.

### Key Responsibilities:
- Opening and reading files.
- Managing file uploads for POST requests.

### Methods:
- `readFile(const std::string& file_path)`: Reads the contents of a file.
- `writeFile(const std::string& file_path, const std::string& data)`: Writes data to a file.

---

## 8. Logger Class
**Purpose**: Logs server events (e.g., incoming requests, errors).

### Key Responsibilities:
- Logging connections, errors, and general information.

### Methods:
- `logInfo(const std::string& message)`: Logs informational messages.
- `logError(const std::string& message)`: Logs error messages.
