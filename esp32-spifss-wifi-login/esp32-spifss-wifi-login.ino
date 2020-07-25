#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPI.h>
#include <SPIFFS.h>

void setup() {

  Serial.begin(115200);
  
  if(!SPIFFS.begin()){
          Serial.println("An Error has occurred while mounting SPIFFS");
  }

  start_access_point();
  start_web_server();
}


void loop()
{
  
}

const char* ap_ssid = "ESP32";
const char* ap_pwd = "12345678";

void start_access_point(){
  WiFi.softAP(ap_ssid, ap_pwd);
  IPAddress myIP = WiFi.softAPIP();  
  Serial.println("Access point started");
  Serial.print("AP IP address: ");
  Serial.println(myIP);
}

AsyncWebServer server(80);

const char* PARAM_SSID = "ssid";
const char* PARAM_PWD = "pwd";

void start_web_server(){
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/index.html", "text/html");
  });

  server.on("/post", HTTP_POST, [] (AsyncWebServerRequest *request) {
      String message = "";
      String ssid = "";
      String pwd = "";

      if (request->hasParam(PARAM_SSID, true) && request->hasParam(PARAM_PWD, true)) {
          ssid =  request->getParam(PARAM_SSID, true)->value();
          pwd = request->getParam(PARAM_PWD, true)->value();
          message = "Connecting to Wifi station";
      } else {
          message = "No SSID or Password!";
      }

      Serial.println(message);
      
      request->send(200, "text/plain", message);
      
      if (ssid != "" && pwd !=""){
        connectToWifi(ssid, pwd);
      }
  });

  server.onNotFound([] (AsyncWebServerRequest *request){
    request->send(404, "text/plain", "Not found");
  });

  server.begin();
}



void connectToWifi(String _ssid, String _password){
    
    WiFi.begin(_ssid.c_str(), _password.c_str());
    
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
    } else{
        Serial.println("");
        Serial.println("WiFi router connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
    }

}
