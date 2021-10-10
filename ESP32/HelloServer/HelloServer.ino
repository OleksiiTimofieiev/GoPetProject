#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <ArduinoJson.h>

#define RXD2 16 //RXX2 pin
#define TXD2 17 //TX2 pin

HardwareSerial serial2(2);

const char* ssid = "otimofie_plus";
const char* password = "K4rmU78B";

String L ;
String T ;
String H ;
String P ;

WebServer server(80);

const int led = 13;

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp32!");
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  Serial.begin(115200);
  serial2.begin(115200, SERIAL_8N1, TXD2, RXD2);
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.on("/data", handleDataRequest);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void handleDataRequest(){
      DynamicJsonDocument doc(512);
 
      doc["ip"] = WiFi.localIP().toString();
      doc["gw"] = WiFi.gatewayIP().toString();
      doc["nm"] = WiFi.subnetMask().toString();

      
      doc["L"] = L;
      doc["T"] = T;
      doc["H"] = H;
      doc["P"] = P;
 
      Serial.print(F("Stream..."));
      String buf;
      serializeJson(doc, buf);
      server.send(200, F("application/json"), buf);
      Serial.print(F("done."));
}
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
void parse(String input_string) {
//  Serial.println(input_string);
  String L_1 = getValue(input_string, ';', 0);
  if (L_1 != "") L = L_1;
  String T_1 = getValue(input_string, ';', 1);
  if (T_1 != "") T = T_1;
  String H_1 = getValue(input_string, ';', 2);
  if (H_1 != "") H = H_1;
  String P_1 = getValue(input_string, ';', 3);
  if (P_1 != "") P = P_1;
}

void loop(void) {
  server.handleClient();
  parse(serial2.readString());
  delay(1000);//allow the cpu to switch to other tasks
}
