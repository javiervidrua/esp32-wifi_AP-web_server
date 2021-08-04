/*---------------------------------------------------------------------------*/
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include "ESPAsyncWebServer.h" // https://randomnerdtutorials.com/esp32-web-server-spiffs-spi-flash-file-system/
#include "SPIFFS.h"

/*---------------------------------------------------------------------------*/
char ssid [25] = "mac_";
byte mac[6];
char macString [25] = "";
IPAddress ip;

//WiFiServer server(80);

String modeActual = "0";
AsyncWebServer server(80);
// Replaces the placeholder in the HTML with actual working mode state
// and sets the color of the buttons based on the actual workind mode state
String processor(const String& var){
  Serial.println(var);
  if(var == "MODE"){
    Serial.printf("The actual mode is: %s", modeActual);
    return modeActual;
  }
  if(var == "B0"){
    if(modeActual == "0"){
      return String("#4caf50");
    }else{
      return String("#011e41;");
    }
  }
  if(var == "B1"){
    if(modeActual == "1"){
      return String("#4caf50");
    }else{
      return String("#011e41;");
    }
  }
  if(var == "B2"){
    if(modeActual == "2"){
      return String("#4caf50");
    }else{
      return String("#011e41;");
    }
  }
  if(var == "B3"){
    if(modeActual == "3"){
      return String("#4caf50");
    }else{
      return String("#011e41;");
    }
  }
  return String("0");
}

/*---------------------------------------------------------------------------*/
void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("Setup");

  Serial.println("Updating the SSID with the MAC of the communications chip");
  WiFi.macAddress(mac);
  sprintf(macString, "%02x:%02x:%02x:%02x:%02x:%02x", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]); // https://www.arduino.cc/en/Reference/WiFiMACAddress
  sprintf(ssid, "%s%s", ssid, macString);
  Serial.printf("New SSID: %s\n", ssid);

  WiFi.softAP(ssid);
  delay(100);
  Serial.println("The WiFi AP has been started");

  ip = WiFi.softAPIP();
  Serial.print("IP address: ");
  Serial.println(ip);

  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Request for /");
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Request for style.css");
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Route to set modeActual to 0
  server.on("/0", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Request for /0");
    modeActual = "0";
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to set modeActual to 1
  server.on("/1", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Request for /1");
    modeActual = "1";
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Route to set modeActual to 2
  server.on("/2", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Request for /2");
    modeActual = "2";
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Route to set modeActual to 3
  server.on("/3", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Request for /3");
    modeActual = "3";
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Start server
  server.begin();
  Serial.println("The async HTTP server has been started");
}

void loop() {
  //Serial.println("Loop");
}
