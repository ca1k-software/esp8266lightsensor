
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFiMulti.h>
#define SENSOR_PIN A0

ESP8266WebServer server;
ESP8266WiFiMulti WiFiMulti;

float sensorValue;

//head and refresher of html page
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
  /*d1 mini pro seems to have problems with the standard
   * arduino library, so I recommend using the
   * esp8266wifimulti library, namely addAP for
   * establishing connections*/
  WiFiMulti.addAP("xxx", "xxx");
  Serial.begin(115200);

  //LED will show once connection is established
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
  sensorValue = analogRead(SENSOR_PIN) - 974; //keeping a range of 0-50 for simplicity
  if(sensorValue < 0){sensorValue = sensorValue * 0;} //prevents negative values
  String hex = String(int(0xFF - (sensorValue * 4)), HEX); //value to be encoded for bgcolor
  //the rest of the html, such as title and bgcolor
  String sv2 = "<title>" + hex + hex + hex + "</title></head><body style='background-color:#" + hex + hex + hex + ";'></body></html>";
  char sv3[strlen(ind) + sv2.length()];
  //combine ind and sv2 so the html page is complete
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
