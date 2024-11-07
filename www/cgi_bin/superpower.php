<?php
// Header für HTML-Ausgabe setzen
header("Content-Type: text/html");

// HTML-Antwort aufbauen
echo "<html><body>";
echo "<h2>Hello from PHP CGI!</h2>";
echo "<p>This is a CGI script response with debug information.</p>";

// Debugging: Anfrage-Methode anzeigen
echo "<h3>Request Method: " . htmlspecialchars($_SERVER['REQUEST_METHOD']) . "</h3>";

// Debugging: Umgebungsvariablen anzeigen
echo "<h3>Environment Variables</h3>";
echo "<pre>";
foreach ($_SERVER as $key => $value) {
    echo htmlspecialchars("$key: $value") . "\n";
}
echo "</pre>";

// Debugging: GET und POST Parameter anzeigen
echo "<h3>GET and POST Parameters</h3>";
if (!empty($_GET) || !empty($_POST)) {
    echo "<pre>";
    // GET-Parameter anzeigen
    foreach ($_GET as $key => $value) {
        echo "GET $key: " . htmlspecialchars($value) . "\n";
    }
    // POST-Parameter anzeigen
    foreach ($_POST as $key => $value) {
        echo "POST $key: " . htmlspecialchars($value) . "\n";
    }
    echo "</pre>";
} else {
    echo "<p>No GET or POST parameters received.</p>";
}

// Debugging: POST-Daten anzeigen, falls vorhanden
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $contentLength = isset($_SERVER['CONTENT_LENGTH']) ? (int)$_SERVER['CONTENT_LENGTH'] : 0;
    if ($contentLength > 0) {
        // POST-Daten direkt aus dem Input-Stream lesen
        $postData = file_get_contents("php://input");
        echo "<h3>POST Data</h3>";
        echo "<pre>" . htmlspecialchars($postData) . "</pre>";
    } else {
        echo "<p>No POST data received.</p>";
    }
}

echo "</body></html>";
?>
