#include <Adafruit_Sensor.h>
#include<Wire.h>
#include<BH1750.h>
#include <DHT_U.h>
#include <SoftwareSerial.h>
#include <JeeLib.h>

#define pinDHT 8
#define typeDHT 22
#define bluetoothPinTXD 5
#define bluetoothPinRXD 6
#define airPin 0
#define refreshFrequency 60000

DHT dhtSensor(pinDHT,typeDHT);

BH1750 lightMeter;

SoftwareSerial bt(bluetoothPinTXD,bluetoothPinRXD);

float temperature;
float humidity;
float battery;
float pm25;
int air;
int pm25RAW;

ISR(WDT_vect)
{
  Sleepy::watchdogEvent();
}

void setup() {
  pinMode(airPin,INPUT);
  Wire.begin();
  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
  bt.begin(9600);
}

void loop() {
  /*
  while(true)
  {
    pm25RAW=Serial.read();
    if(0<pm25RAW && pm25RAW<=50)
      {
        pm25=pm25RAW*5/1024.0*700;
        Serial.print("         ");
        Serial.print(pm25);
        break;
      }
      //Serial.println(data);
  }
  */
  temperature=dhtSensor.readTemperature();
  humidity=dhtSensor.readHumidity();
  uint16_t lux=lightMeter.readLightLevel();
  air=analogRead(airPin);

  
  bt.print((String)temperature+","+(String)humidity+","+(String)lux+","+(int)air);
  //delay(refreshFrequency);
  Sleepy::loseSomeTime(refreshFrequency);
}

