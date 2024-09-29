### High-Level Overview of `poll()` and Why We Use It

In network programming, servers often need to handle multiple clients at once, especially in the case of web servers. However, dealing with multiple connections can be tricky because you don't want to block a single client while waiting for it to send or receive data, nor do you want to spawn a new thread for every client (which could be resource-intensive).

`poll()` is a system call used for multiplexing I/O, meaning it allows the server to handle multiple I/O (input/output) streams, such as socket connections, without blocking the server for each connection. This is known as **non-blocking I/O** and is an efficient way to scale servers that need to handle many connections simultaneously.

#### Why Use `poll()`?
- **Scalability**: `poll()` allows the server to handle hundreds or thousands of client connections simultaneously. Unlike `select()`, which has a limit on the number of file descriptors it can monitor, `poll()` can handle more connections efficiently.
- **Non-blocking I/O**: Instead of waiting for a connection to send data (which would block the process), `poll()` checks if data is ready to be read or written on any monitored connections. The server can then handle only the "ready" connections.
- **Efficient resource usage**: By avoiding blocking on individual connections, the server can perform other tasks or handle multiple clients at once without consuming significant resources, such as creating threads or processes for every connection.

### Technical Low-Level Details of `poll()`

#### How `poll()` Works
`poll()` is designed to monitor multiple file descriptors (which could be network sockets, files, pipes, etc.) to see if any of them are ready for I/O operations like reading or writing. Instead of focusing on one connection at a time, `poll()` lets you watch multiple connections and only respond when something is ready.

#### Steps:
1. **Create a list of file descriptors (sockets) to monitor**: The server creates a list of file descriptors it wants to watch. These file descriptors could represent incoming client connections or listening sockets waiting for new connections.
  
2. **Call `poll()`**: The server passes this list of file descriptors to `poll()`, along with the type of events it wants to monitor (e.g., readable, writable, errors). `poll()` will block or wait for the specified timeout, or return immediately if any file descriptors are ready.
  
3. **`poll()` returns a list of "ready" descriptors**: Once one or more file descriptors become ready (i.e., a client has sent data or is ready to receive data), `poll()` returns the list of file descriptors that are ready for the next I/O operation. If none of the file descriptors are ready, `poll()` can optionally block until one becomes available or timeout.

4. **Handle the ready file descriptors**: The server then processes only the connections that are ready (such as reading incoming data from a client or sending a response). Other connections that are not ready remain unhandled until they become ready in future `poll()` calls.

5. **Repeat**: The server continues the cycle, calling `poll()` repeatedly to monitor connections, accept new ones, and process data.

#### Key Concepts in the `poll()` Example:
- **File descriptors**: These represent the network sockets (the server's listening socket or a connected client socket) that `poll()` will monitor.
- **Events**: The `events` field in the `pollfd` structure specifies what we are interested in (e.g., `POLLIN` for incoming data or connections).
- **Non-blocking mode**: The sockets are set to non-blocking mode using `fcntl()`, ensuring that `read()` and `accept()` do not block the server when no data is available or when no new connection arrives.

#### `pollfd` Structure:
Each element in the `poll()` array is a `struct pollfd`, which looks like this:

```cpp
struct pollfd {
    int fd;        // The file descriptor to monitor
    short events;  // Requested events (e.g., POLLIN, POLLOUT)
    short revents; // Returned events (what actually happened)
};
```

### What Happens Internally in `poll()`?

The `poll()` system call interacts with the kernel to determine if any of the file descriptors are ready for the requested I/O operations. It checks all file descriptors in the array, and if any are ready, it modifies the `revents` field of the corresponding `pollfd` structure.

If none of the requested events have occurred, `poll()` can block (waiting for the specified timeout) or return immediately if the timeout is set to `0`.

### Comparison with Other I/O Multiplexing:

- **`select()`**: An older system call that also allows monitoring multiple file descriptors. However, `select()` has limitations, such as a hard limit on the number of file descriptors it can monitor (often 1024). `poll()` doesn't have this limitation.
- **`epoll()`**: A more modern alternative (Linux-only) that is more efficient than `poll()` for managing a large number of connections because it doesn't require passing the entire file descriptor list each time.

### Key Advantages of `poll()`:

- Supports more file descriptors than `select()`.
- Non-blocking I/O ensures the server can handle multiple clients concurrently without blocking.
- Efficient way to multiplex I/O across multiple clients.

### Limitations:

- For very large-scale systems (e.g., tens of thousands of connections), `epoll()` (on Linux) or `kqueue()` (on BSD-based systems) might offer better performance because they scale better with high numbers of file descriptors.

### Conclusion:

`poll()` is widely used in multi-client servers because it enables non-blocking, multiplexed I/O. It allows servers to monitor multiple connections at once and handle only the connections that are ready, optimizing resource usage and improving scalability without the complexity of multi-threading.
