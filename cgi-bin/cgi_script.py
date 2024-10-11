#!/usr/bin/env python3
import cgi

print("Content-Type: text/html\n")  # CGI header to indicate HTML output

form = cgi.FieldStorage()  # Get form data

print("<html><body>")
print("<h2>Hello from Python CGI!</h2>")
print("<p>This is a CGI script response.</p>")
print("</body></html>")

