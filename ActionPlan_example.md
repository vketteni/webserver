# Webserver Project - Action Plan

This project involves building an HTTP server in C++98. Below is the action plan for the team of three, with task distribution, deadlines, and milestones to guide us through the development process.

## Team Roles and Responsibilities

- **Team Lead (TL)**: Oversees the project, manages integration, and ensures timely submission.
- **Backend Developer (BD)**: Focuses on the core server functionality, request handling, and HTTP protocol compliance.
- **Configuration & Testing Specialist (CTS)**: Manages configuration files, non-functional requirements, and testing.

---

## Stage 1: Research and Planning (Day 1–3)

### TL
- Review the subject guidelines and ensure an understanding of the mandatory parts.
- Ensure the team reads relevant RFCs (e.g., 2616 for HTTP/1.1).
  
### BD
- Study HTTP request/response cycles and `select()`, `poll()`, `epoll()` for non-blocking I/O.
- Prepare a high-level architecture for managing connections and processing requests.

### CTS
- Research configurations in servers like NGINX, focusing on port management, error pages, and client request limits.
- Set up environments (e.g., NGINX) for future comparison and benchmarking.

---

## Stage 2: Setup and Initial Implementation (Day 4–10)

### TL
- Set up the repository with proper Git branches (e.g., GitFlow).
- Create a Makefile with basic project compilation (`all`, `clean`, `fclean`, `re`).
- Outline code skeletons with placeholders for the team.

### BD
- Implement socket setup and bind to multiple ports.
- Build a basic HTTP request handler for GET requests and serve static HTML pages.
- Ensure the server remains non-blocking using `poll()` or equivalent.

### CTS
- Develop a basic configuration file parser to set up host, port, and server name.
- Create a sample configuration file inspired by NGINX.

---

## Stage 3: Core Development (Day 11–18)

### TL
- Integrate configurations into the server startup.
- Coordinate between BD and CTS to ensure the components work together smoothly.

### BD
- Expand the request handler to support POST and DELETE methods.
- Implement file uploads with compliance to size limits.
- Add error handling for HTTP status codes (400, 404, 500, etc.).

### CTS
- Implement advanced configuration options: route handling, redirection, directory listings, etc.
- Write test scripts (e.g., in Python) to simulate client requests and compare behavior to NGINX.

---

## Stage 4: Stress Testing and Optimization (Day 19–25)

### TL
- Monitor system behavior during stress tests (handling multiple clients).
- Coordinate with CTS for final adjustments based on test results.

### BD
- Optimize the non-blocking I/O to reduce response times and improve memory management.
- Fix concurrency-related bugs and ensure server resilience.

### CTS
- Run extensive stress tests to check the server's response under high load.
- Document performance comparisons against NGINX.
- Implement corner case testing for malformed requests and client disconnections.

---

## Stage 5: Finalization and Bonus Features (Day 26–30)

### TL
- Ensure all mandatory requirements are complete (GET, POST, DELETE, file uploads).
- Prepare for submission, cleaning up the repository and verifying file names.

### BD
- Implement optional features:
  - Support for CGI (e.g., PHP).
  - Session and cookie management.

### CTS
- Update configuration files to showcase all features.
- Ensure the server works with a browser and passes evaluation tests.

---

## Stage 6: Submission and Peer-Evaluation

### TL
- Submit the project repository, ensuring proper file names and organization.
- Prepare for peer-evaluation and ensure the team can defend the project.

---

## Milestones

1. **Day 3**: Research and architecture finalized.
2. **Day 10**: Core socket setup and basic GET request handling complete.
3. **Day 18**: POST, DELETE, file uploads, and non-blocking I/O implemented.
4. **Day 25**: Stress testing completed; server remains stable.
5. **Day 30**: Final submission and optional bonus features completed.

---

This plan ensures that tasks are distributed efficiently, with continuous progress, testing, and optimization at every stage. Let's keep communication open and make sure we stick to the timeline!
