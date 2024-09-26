# Webserver Project - Allowed Functions and Their Use Cases

This document outlines the allowed functions for the project and their respective use cases, categorized by the roles they will play in building the web server using C++98.

## Process Control

- **`execve`**: Executes a program, typically used for CGI handling (e.g., running PHP or Python scripts).
- **`dup`, `dup2`**: Duplicates file descriptors, useful for redirecting input/output, especially when working with CGI scripts.
- **`pipe`**: Creates a unidirectional communication channel, useful for inter-process communication (IPC), such as between the server and a CGI process.
- **`fork`**: Creates a new process, which will be needed to run CGI scripts separately from the server.
- **`waitpid`**: Waits for a child process (e.g., a CGI script) to complete.
- **`kill`**: Sends signals to processes, useful for handling shutdowns or killing child processes.
- **`signal`**: Used to set up signal handlers (e.g., handling `SIGINT` for graceful server shutdown).

## Socket and Network Programming

- **`socket`**: Creates a socket for network communication. This is the entry point for client-server communication.
- **`bind`**: Associates a socket with a specific IP address and port. This is used to bind the server to a listening address.
- **`listen`**: Makes the socket listen for incoming client connections.
- **`accept`**: Accepts an incoming client connection and creates a new socket for communication with that client.
- **`connect`**: Used by clients to connect to a server socket (not typically used on the server side).
- **`send`, `recv`**: Sends and receives data over a socket. These are used to handle HTTP request and response data.
- **`socketpair`**: Creates a pair of connected sockets, useful for IPC (inter-process communication), especially for CGI handling.
- **`getaddrinfo`, `freeaddrinfo`**: Translates hostnames to IP addresses and manages memory for address info. This simplifies the setup of network communication.
- **`htons`, `htonl`, `ntohs`, `ntohl`**: Converts values between host and network byte order, required for network communication.
- **`getsockname`**: Retrieves the local address and port of a socket.
- **`getprotobyname`**: Retrieves protocol information (e.g., TCP) for use in socket creation.

## I/O Multiplexing (Non-blocking I/O)

- **`select`**: Monitors multiple file descriptors (sockets) for I/O readiness (reading/writing). Used for non-blocking I/O operations to handle multiple clients.
- **`poll`, `epoll`**: Similar to `select()`, but more efficient for a large number of file descriptors. These can be used as alternatives to `select()` for better performance.
  - **`epoll_create`, `epoll_ctl`, `epoll_wait`**: Functions for managing `epoll` instances and controlling file descriptors monitored for I/O events.
- **`kqueue`, `kevent`**: BSD-specific equivalents of `epoll`, used for managing I/O events, relevant if developing on macOS.

## File and Directory Handling

- **`open`, `close`**: Opens and closes file descriptors. Important for serving static files and handling file uploads.
- **`read`, `write`**: Reads from and writes to file descriptors. Used to process HTTP requests and responses.
- **`opendir`, `readdir`, `closedir`**: Functions for directory traversal, useful for listing directory contents (e.g., if directory listing is enabled on the server).
- **`stat`, `access`**: Retrieves file metadata (e.g., file size, permissions) and checks for file existence, useful for serving files or verifying permissions during uploads.

## Error Handling

- **`strerror`, `gai_strerror`**: Converts error numbers into human-readable strings, useful for debugging and logging.
- **`errno`**: A global variable that stores error codes set by system calls, useful for diagnosing failed operations.

## File Descriptor Control

- **`fcntl`**: Manipulates file descriptor flags, mainly used to set file descriptors to non-blocking mode.
  - Allowed flags: `F_SETFL`, `O_NONBLOCK`, `FD_CLOEXEC`.

## Miscellaneous

- **`chdir`**: Changes the current working directory, useful when running CGI scripts or serving files from different directories.
- **`setsockopt`**: Configures socket options (e.g., enabling address reuse, setting buffer sizes).
- **`access`**: Checks the permissions of a file, useful for verifying read/write access before serving files or processing uploads.

---

## Key Use Cases in the Project

### 1. **Socket Setup and Connection**
   - Use `socket()`, `bind()`, `listen()`, and `accept()` to set up the server socket and manage incoming client connections.
   - Utilize `select()`, `poll()`, or `epoll()` to handle multiple clients concurrently without blocking the server.

### 2. **Handling HTTP Requests and Responses**
   - Use `recv()` to receive HTTP requests and `send()` to send HTTP responses.
   - File-related functions (`open()`, `read()`, `write()`) and directory functions (`opendir()`, `readdir()`) will be used for serving static files or handling file uploads.

### 3. **Process Control for CGI**
   - Use `fork()` to create a separate process for CGI execution.
   - Use `execve()` to execute CGI scripts, and `pipe()` or `dup2()` to manage input/output between the server and the CGI process.

### 4. **Configuration and Testing**
   - Network functions like `getaddrinfo()` and `freeaddrinfo()` will be used to resolve hostnames and manage addresses.
   - Functions like `stat()` and `access()` will verify file existence and permissions before serving files.

### 5. **Graceful Server Management**
   - Use `signal()` and `kill()` for process termination and handling server shutdowns.
   - Handle child processes efficiently and ensure smooth shutdown during interrupts.

---

This summary outlines the functions you'll need to build a resilient, non-blocking web server capable of handling HTTP requests and CGI scripts, while ensuring compliance with system-level constraints.
