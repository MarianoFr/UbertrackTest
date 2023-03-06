#include "esp_01.h"

/*
 * Init Esp01 module with AT commands
*/
bool Esp01::init()
{
  stream.print("Init esp_01\n");
	if (!testSerial(100))//test AT comms
    return false;
  stream.print("AT+RST\n");//reset module
  if (listening(100) != OK_RX)
    return false;
  stream.print("ATE0\n");//turn echo off 
  if (listening(100) != OK_RX)
    return false;
  stream.print("AT+CWMODE=1\n");// station mode
  if (listening(100) != OK_RX)
    return false;
  stream.print("AT+CIPMUX=0\n");//single connections
  if (listening(100) != OK_RX)
    return false;
  return true;
}

/*
 * Test connection with AT message and waiting confirmation
 * input: time out in mmilliseconds
*/
bool Esp01::testSerial(uint32_t timeout_ms)
{
	uint32_t start   = millis();
  for (uint32_t start = millis(); millis() - start < timeout_ms;)
  {     
    stream.print("AT\n");
    stream.flush();
    delay(ESP_YIELD_MS);
    if (listening(500) == OK_RX) 
    {
      return true;
    }
  }
  return false;
}

/*
 * Wait for Esp01 confirmation messages
 * input: time out in mmilliseconds
*/
uint8_t Esp01::listening(uint32_t timeout_ms)
{
  String rx_buff;
  String response_ok = "OK\n";
  String response_error = "ERROR\n";
  String response_connected = "WIFI CONNECTED\n";
  String send_data = ">";
  String send_ok = "SEND OK\n";
  uint8_t index = 0;
  uint32_t start = millis();
  do 
  {
    delay(ESP_YIELD_MS);
    while (stream.available() > 0)
    {
      int8_t c = stream.read();
      if (c <= 0) 
        continue;
      rx_buff += static_cast<char>(c);
      if (rx_buff.endsWith(response_ok))
      {
        return OK_RX;          
      }
      else if (rx_buff.endsWith(response_error))
      {
        return ERR_RX;          
      }
      else if (rx_buff.endsWith(response_connected))
      {
        return WIFI_CONN;
      }
      else if (rx_buff.endsWith(send_data))
      {
        return OK_RX;
      }
      else if (rx_buff.endsWith(send_ok))
      {
        return OK_RX;
      }
    }
  } while (millis() - start < timeout_ms);
  Serial.println("Timed Out");
  return NO_RX;
}

/*
 * Connect to wifi
 *  Inputs char* ssid: network name
 *         char* pass: network password 
*/
bool Esp01::WiFiConnect(char* ssid, char* pass)
{
  char cmd[255];
  sprintf(cmd,"AT+CWJAP=\"%s\",\"%s\"\n",ssid,pass);
  if(!stream.available())
  {
    stream.print(cmd);
    if(listening(100) == WIFI_CONN)
      return true;
    else
      return false;
  }
  else
    return false;
}

/*
 * Disconnect from wifi network
*/
bool Esp01::WiFiDisconnect()
{
  if(!stream.available())
  {
    stream.print("AT+CWQAP");
    if(listening(100) == OK_RX)
      return true;
    else
      return false;
  }
  else
    return false;
}

/*
* Start TCP connection
* Inputs char* host: url to connect to
*        uint16_t port number
*/
bool Esp01::TCPconnect(char* host, uint16_t port)
{
  char cmd[255];
  sprintf(cmd,"AT+CIPSTART=\"TCP\",\"%s\",%u\n",host,port);
  if(!stream.available())
  {
    stream.print(cmd);
    if(listening(100) == OK_RX)
      return true;
    else
      return false;
  }
  else
    return false;
}

/*
* Send TCP package
* Inputs char* data: data to send
*        uint16_t len: data length
*/
bool Esp01::TCPsend(char* data, uint16_t len)
{
  char cmd[255];
  sprintf(cmd,"AT+CIPSEND=%u\n",len);
  if(!stream.available())
  {
    stream.print(cmd);
    if(listening(100) == OK_RX)
    {
      stream.print(data);
      if(listening(100) == OK_RX)
        return true;
      else
        return false;
    }
    else
      return false;
  }
  else
    return false;
}

/*
* Ping an IP host
* Inputs char* host: url to ping
*/
uint32_t Esp01::TCPping(char* host)
{
  String rx_buff;
  char cmd[255];
  sprintf(cmd,"AT+PING=\"%s\"\n",host);
  if(!stream.available())
  {
    stream.print(cmd);
    delay(ESP_YIELD_MS);
    uint32_t start = millis();
    while (stream.available() > 0 || millis()-start<100)
    {
      char c = stream.read();      
      if (c <= 0) 
        continue;
      if(c==':')
      {      
        delay(ESP_YIELD_MS);
        while (stream.available() > 0 && c!='\n')
        {
          //Serial.println(c);
          c = stream.read();
          rx_buff += static_cast<char>(c);
        }
        return rx_buff.toInt();
      }
    }
    Serial.println("Ping timeout");          
  }
  return 0;
}

/*
* Close TCP connection
*/
bool Esp01::TCPclose()
{
  stream.print("AT+CIPCLOSE\n");
  if(listening(100) == OK_RX)
    return true;
  else
    return false;
}