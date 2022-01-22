#include <SPI.h>
#include <RH_RF95.h>
#include "DHT.h"

#define DHTPIN A0     // what pin we're connected to
#define DHTTYPE DHT11  // DHT type
#define RF95_FREQ 434.0
DHT dht(DHTPIN, DHTTYPE);
int hum;  //Stores humidity value
int temp; //Stores temperature value

RH_RF95 rf95;

void setup() 
{
  pinMode(13,OUTPUT);
  Serial.begin(9600);
  dht.begin();
  if (!rf95.init())
    Serial.println("init failed");
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }

  rf95.setTxPower(18); //Transmission power of the Lora Module
}

void loop()
{  digitalWrite(13,HIGH);
   delay(1000);
   digitalWrite(13,LOW);
   delay(1000);
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  String humidity = String(hum); //int to String
  String temperature = String(temp);
  String data = temperature + humidity;
  Serial.print(data);
  char d[5];
  data.toCharArray(d, 5); //String to char array
  Serial.println("Sending to rf95_server");
  rf95.send(d, sizeof(d));
  rf95.waitPacketSent();
  delay(4000);
}
