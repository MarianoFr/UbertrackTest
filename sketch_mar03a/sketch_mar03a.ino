#include <esp_01.h>
#include <SoftwareSerial.h>
#define SW_RX  11
#define SW_TX  12

SoftwareSerial SerialAT(SW_RX, SW_TX);
Esp01 esp01 (SerialAT);

void setup()
{
  char mensaje[] = "Rammstein!!";
  Serial.begin(9600);
  bool success = esp01.init();
  if(success)
    Serial.println("esp initialized");
  success = esp01.WiFiConnect("ssid","pass");
  if(success)
    Serial.println("Wifi connected");
  success = esp01.TCPconnect("www.google.com",8883);
  if(success)
    Serial.println("TCP connected");
  success = esp01.TCPsend(mensaje, (sizeof(mensaje)/sizeof(char))-1);
  Serial.println();
  if(success)
    Serial.println("Data sent");
  uint32_t ping = esp01.TCPping("www.google.com");
  if(ping==0)
    Serial.println("timeout");
  Serial.println(ping);
  success = esp01.TCPclose();
  if(success)
    Serial.println("Tcp closed");
}

void loop()
{
//  Serial.println("Rammstein");
//  delay(500);
}
