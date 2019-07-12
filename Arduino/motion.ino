#include <ESP8266WiFi.h>            
#include <ESP8266WebServer.h>
#define pinRelay 5
#define pinMotion A0
#define onDuration 180//Seconds 2X
#define occupancyDuration 60

char wifiSSID[]="bme5757";
char password[]="eelab5757";

int motion=0;
int state;
int occupancy;
int timer=0;

ESP8266WebServer server(80);

void setup() {
  // put your setup code here, to run once:
  pinMode(pinRelay,OUTPUT);
  pinMode(pinMotion,INPUT);
  WiFi.begin(wifiSSID,password);

  while(WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
  }
  server.on("/light",turnOn);
  server.on("/occupancy",occupy);
  server.begin();
  state=0;
  occupancy=0;
  timer=onDuration;

  Serial.begin(9600);
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());

  
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  
  motion=analogRead(pinMotion);
  
  if(state==0 && motion>512)
  {
    state=1;
    occupancy=1;
    timer=onDuration;
    digitalWrite(pinRelay,HIGH);
  }
  else if(state==1 && motion>512)
  {
    state=1;
    occupancy=1;
    timer=onDuration;
  }
  else if(state==1 && motion<=512)
  {
    timer=timer-1;
  }
  if(timer==0)
  {
    state=0;
    digitalWrite(pinRelay,LOW);
  }
  if(timer==onDuration-occupancyDuration)
  {
    occupancy=0;
  }

  delay(500);
}

void turnOn()
{
  if(server.arg("on")=="1")
  {
    digitalWrite(pinRelay,HIGH);
    server.send(200,"text/plain","1");
    state=1;
    timer=onDuration;
  }
  else if(server.arg("on")=="0")
  {
    digitalWrite(pinRelay,LOW);
    server.send(200,"text/plain","0");
    state=0;
  }
  else if(server.arg("on")=="2")
  {
    server.send(200,"text/plain",(String)state);
  }
}

void occupy()
{
  server.send(200,"text/plain",(String)occupancy);
}

