#!/usr/bin/env python3
import os
import sys

# CGI-Header, um HTML auszugeben
print("Content-Type: text/html\n")

# Lese den Body nur bei POST
if os.environ.get('REQUEST_METHOD') == 'POST':
    try:
        content_length = int(os.environ.get('CONTENT_LENGTH', 0))
        post_data = sys.stdin.read(content_length)
        print(f"<html><body><h1>POST request body: {post_data}</h1></body></html>")
    except Exception as e:
        print(f"<html><body><h1>Error: {e}</h1></body></html>")
else:
    print("<html><body><h1>No POST data received</h1></body></html>")
