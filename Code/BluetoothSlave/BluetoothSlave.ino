#include <DHT.h>
#include <DHT_U.h>
#include <EasyTransfer.h> 

#define DHTPIN 11 
#define DHTTYPE DHT22 

struct SEND_DATA_STRUCTURE
{
  float temperature ;
  float humidity ;
};

struct ACKNOWLEDGE
{
  boolean received = false;
};

int counter = 0;
SEND_DATA_STRUCTURE data;
ACKNOWLEDGE acknowledge;
DHT dht(DHTPIN, DHTTYPE);
EasyTransfer ETin, ETout; //We need two EasyTransfer object, one for the data we send, and one for the data we receive.

void setup() {
  Serial.begin(9600); //Baudrate of the Bluetooth modules
  dht.begin();
  ETout.begin(details(data), &Serial);
  ETin.begin(details(acknowledge), &Serial);
}

void loop() {
  if(ETin.receiveData())
  {
    if(acknowledge.received == true)
    {
       digitalWrite(13,HIGH);
       delay(100);
       digitalWrite(13,LOW);
    }
  }
  counter ++;
  if(counter == 8)//Send data once every two seconds
  {
   data.temperature = dht.readTemperature();
   data.humidity = dht.readHumidity();
   ETout.sendData();
   counter = 0;
  }
  delay(250);
  acknowledge.received = false;
}
