#include <ESP8266WiFi.h>            
#include <ESP8266WebServer.h>
#define pinRelay 0

char wifiSSID[]="bme5757";
char password[]="eelab5757";
int state;

ESP8266WebServer server(80);

void setup()
{
  pinMode(pinRelay,OUTPUT);
  digitalWrite(pinRelay,HIGH);
  state=1;
  
  WiFi.begin(wifiSSID,password);
  while(WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
  }
  server.on("/light",turnOn);
  server.begin();

  Serial.begin(9600);
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());

}

void loop()
{
  server.handleClient();
}

void turnOn()
{
    if(server.arg("on")=="1")
    {
      digitalWrite(pinRelay,1);
      server.send(200,"text/plain","1");
      state=1;
    }
    else if(server.arg("on")=="0")
    {
      digitalWrite(pinRelay,0);
      server.send(200,"text/plain","0");
      state=0;
    }
    else if(server.arg("on")=="2")
    {
      server.send(200,"text/plain",(String)state);
    }
}

