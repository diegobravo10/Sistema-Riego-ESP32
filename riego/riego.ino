#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "Familia_Bravo";
const char* password = "0151273042B";
const int relayPin = 26;
const int sensorPin = 34;

unsigned long tiempoApagado = 0;
bool timerActivo = false;
bool modoAutomatico = false;
int umbralHumedad = 30; 

AsyncWebServer server(80);

void setup(){
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  
  // Si antes en HIGH estaba abierto, ahora iniciamos en LOW
  digitalWrite(relayPin, LOW); 

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

  // --- API STATUS ---
  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
    int raw = analogRead(sensorPin);
    int hum = map(raw, 4095, 1500, 0, 100);
    if(hum < 0) hum = 0; if(hum > 100) hum = 100;

    // Mantenemos el envío del estado del pin tal cual
    bool valveState = (digitalRead(relayPin) == HIGH); 
    
    String json = "{\"humidity\":" + String(hum) + 
              ",\"valve\":" + String(valveState ? "true" : "false") + 
              ",\"timerActive\":" + String(timerActivo ? "true" : "false") +
              ",\"autoMode\":" + String(modoAutomatico ? "true" : "false") + "}";
    request->send(200, "application/json", json);
  });

  // --- MANUAL ---
  server.on("/manual", HTTP_GET, [](AsyncWebServerRequest *request){
    if(request->hasParam("state")){
      String state = request->getParam("state")->value();
      timerActivo = false; 

      digitalWrite(relayPin, (state == "ON") ? HIGH : LOW);
      request->send(200, "application/json", "{\"status\":\"ok\"}");
    }
  });

  // --- TIMER ---
  server.on("/startTimer", HTTP_GET, [](AsyncWebServerRequest *request){
    if(request->hasParam("seg")){
      int s = request->getParam("seg")->value().toInt();
      timerActivo = true;
      tiempoApagado = millis() + (s * 1000);

      digitalWrite(relayPin, HIGH); 
      request->send(200, "application/json", "{\"status\":\"timer_started\"}");
    }
  });

  server.on("/setAuto", HTTP_GET, [](AsyncWebServerRequest *request){
  if(request->hasParam("active")){
    String val = request->getParam("active")->value();
    modoAutomatico = (val == "true");
    request->send(200, "application/json", "{\"status\":\"ok\"}");
  }
});
  Serial.println("");
  Serial.println("¡Conectado exitosamente!");
  Serial.print("IP asignada por el router: ");
  Serial.println(WiFi.localIP()); // Esta es la IP que pondrás en tu túnel de Cloudflare
  Serial.print("Señal (RSSI): ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");

  server.begin();
}

void loop(){
  // Timer
  if(timerActivo && millis() >= tiempoApagado){
    digitalWrite(relayPin, LOW);
    timerActivo = false;
  }

  if(modoAutomatico && !timerActivo){
    int raw = analogRead(sensorPin);
    int hum = map(raw, 3260, 1230, 0, 100);
    if(hum < 0) hum = 0; if(hum > 100) hum = 100;

    if(hum < umbralHumedad){
      digitalWrite(relayPin, HIGH); // Abre válvula
    } else {
      digitalWrite(relayPin, LOW);  // Cierra válvula
    }
  }
}