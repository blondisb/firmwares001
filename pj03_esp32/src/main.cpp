#include <HTTPClient.h>
#include <WiFi.h>

// Configura tu WiFi
const char* ssid = "ETHERNET";
const char* password = "P4NSH1T41";

//  Webhook de Discord
String webhook_url = "https://discord.com/api/webhooks/1406138198886846514/yBsq7lmrRcaHAjgDFvbj9hjESgkK3y6tLnAf_y4WDcwXnZzMLavBRbPJbmb863PHqMwx"; 

void setup() {
  Serial.begin(115200);

  // Conexión WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n Conectado a WiFi");

  // Enviar mensaje al webhook
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(webhook_url);  
    http.addHeader("Content-Type", "application/json");

    // JSON con el mensaje
    String message = "{\"content\":\"Hola desde mi ESP32 \"}";

    int httpResponseCode = http.POST(message);

    if (httpResponseCode > 0) {
      Serial.println("Mensaje enviado a Discord");
      Serial.println("Código respuesta: " + String(httpResponseCode));
      Serial.println("Respuesta: " + http.getString());
    } else {
      Serial.println("Error al enviar: " + String(httpResponseCode));
    }

    http.end();
  }
}

void loop() {
  // No hacemos nada en el loop
}