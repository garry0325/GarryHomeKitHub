#include <Adafruit_Sensor.h>
#include<Wire.h>
#include<BH1750.h>
#include <DHT_U.h>
#include <SoftwareSerial.h>
#include <JeeLib.h>

#define pinDHT 8
#define typeDHT 11
#define bluetoothPinTXD 5
#define bluetoothPinRXD 6
#define airPin 0
#define refreshFrequency 240000

DHT dhtSensor(pinDHT,typeDHT);

BH1750 lightMeter;

SoftwareSerial bt(bluetoothPinTXD,bluetoothPinRXD);

float temperature;
float humidity;
float battery;


ISR(WDT_vect)
{
  Sleepy::watchdogEvent();
}

void setup() {
  // put your setup code here, to run once:
  pinMode(airPin,INPUT);
  Wire.begin();
  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
  bt.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
   temperature=dhtSensor.readTemperature();
  humidity=dhtSensor.readHumidity();
  uint16_t lux=lightMeter.readLightLevel();
  
  bt.print((String)temperature+","+(String)humidity+","+(String)lux);
  //delay(refreshFrequency);
  Sleepy::loseSomeTime(refreshFrequency);
  
}
