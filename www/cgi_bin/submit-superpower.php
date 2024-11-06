#!/usr/bin/php-cgi
<?php
// Set the correct content type for HTTP response
header("Content-Type: text/html");

// Simple HTML output
echo "<html>";
echo "<head><title>PHP CGI Script</title></head>";
echo "<body>";
echo "<h1>Hello, World!</h1>";
echo "<p>This is a simple PHP script running as a CGI program.</p>";
echo "</body>";
echo "</html>";
?>
