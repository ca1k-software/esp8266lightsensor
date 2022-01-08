/*------------------------------------------------------------------------------
  07/01/2018
  Author: Makerbro
  Platforms: ESP8266
  Language: C++/Arduino
  File: webserver_html.ino
  ------------------------------------------------------------------------------
  Description: 
  Code for YouTube video demonstrating how to use HTML weppages in a web 
  server's response.
  https://youtu.be/VNgFbQAVboA

  Do you like my videos? You can support the channel:
  https://patreon.com/acrobotic
  https://paypal.me/acrobotic
  ------------------------------------------------------------------------------
  Please consider buying products from ACROBOTIC to help fund future
  Open-Source projects like this! We'll always put our best effort in every
  project, and release all our design files and code for you to use. 

  https://acrobotic.com/
  https://amazon.com/acrobotic
  ------------------------------------------------------------------------------
  License:
  Please see attached LICENSE.txt file for details.
------------------------------------------------------------------------------*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFiMulti.h>
#define SENSOR_PIN A0

ESP8266WebServer server;
ESP8266WiFiMulti WiFiMulti;

float sensorValue;

char ind[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta http-equiv="refresh" content="1"></meta>

)=====";
  
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("SLJ", "edcbaabcde");
  Serial.begin(115200);
  while(WiFi.status()!=WL_CONNECTED)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED_BUILTIN, LOW);
  
  server.on("/",[](){
  sensorValue = analogRead(SENSOR_PIN) - 974;
  if(sensorValue < 0){sensorValue = sensorValue * 0;}
  String hex = String(int(0xFF - (sensorValue * 4)), HEX);
  String sv2 = "<title>" + hex + hex + hex + "</title></head><body style='background-color:#" + hex + hex + hex + ";'></body></html>";
  char sv3[strlen(ind) + sv2.length()];
  strcpy(sv3,ind);
  strcat(sv3,sv2.c_str());
  server.send_P(200,"text/html",sv3);
  });
  server.begin();
}

void loop()
{
  server.handleClient();
}
