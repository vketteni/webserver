#!/usr/bin/env python3
import cgi
import os
import sys

# CGI-Header, um HTML auszugeben
print("Content-Type: text/html\n")
print("<html><body>")
print("<h2>Hello from Python CGI!</h2>")
print("<p>This is a CGI script response with debug information.</p>")

# Debugging: Umgebungsvariablen anzeigen
print("<h3>Environment Variables</h3>")
print("<pre>")
for key, value in sorted(os.environ.items()):
    print(f"{key}: {value}")
print("</pre>")

# Debugging: Anfrage-Methode anzeigen
print(f"<h3>Request Method: {os.environ.get('REQUEST_METHOD')}</h3>")

# Debugging: GET und POST Parameter anzeigen
form = cgi.FieldStorage()

print("<h3>GET and POST Parameters</h3>")
if form:
    print("<pre>")
    for key in form.keys():
        print(f"{key}: {form.getvalue(key)}")
    print("</pre>")
else:
    print("<p>No GET or POST parameters received.</p>")

# Debugging: POST-Daten anzeigen, falls vorhanden
if os.environ.get('REQUEST_METHOD') == 'POST':
    try:
        content_length = int(os.environ.get('CONTENT_LENGTH', 0))
        if content_length > 0:
            post_data = sys.stdin.read(content_length)
            print("<h3>POST Data</h3>")
            print("<pre>")
            print(post_data)
            print("</pre>")
        else:
            print("<p>No POST data received.</p>")
    except Exception as e:
        print(f"<p>Error reading POST data: {e}</p>")

print("</body></html>")
