#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "Familia_Bravo";
const char* password = "0151273042B";
const int relayPin = 26;

AsyncWebServer server(80);

// HTML estático con botones directos (sin procesadores complejos)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial; text-align: center; background-color: #f4f4f9; padding-top: 50px; }
    .card { background: white; padding: 30px; border-radius: 15px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); display: inline-block; }
    .btn { display: block; width: 220px; padding: 20px; margin: 15px auto; font-size: 1.2rem; border-radius: 10px; color: white; text-decoration: none; font-weight: bold; }
    .btn-on { background-color: #2ecc71; }
    .btn-off { background-color: #e74c3c; }
  </style>
</head>
<body>
  <div class="card">
    <h1>🌱 Control de Válvula</h1>
    <a href="/on" class="btn btn-on">ABRIR RIEGO</a>
    <a href="/off" class="btn btn-off">CERRAR RIEGO</a>
  </div>
</body>
</html>)rawliteral";

void setup(){
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Apagado inicial

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(1000); Serial.print("."); }
  
  Serial.println("\nConectado!");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(relayPin, LOW); // Enciende
    request->redirect("/");
  });

  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(relayPin, HIGH); // Apaga
    request->redirect("/");
  });

  server.begin();
  Serial.println("Servidor en puerto 80");
}

void loop(){}