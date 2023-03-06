#ifndef ESP_01_H_
#define ESP_01_H_

//headers
#include "Arduino.h"

//Macros
/*
	TODO:
		*Init serial connection//init()
		*WiFi connect with acknowledge//WiFiConnect
		*WiFi disconnect//WiFiDisconnect()
		*TCP client connection//TCPconnect()
		*Send data through TCP//TCPsend()		
		*TCP ping//TCPping
		*Close TCP connection//TCPclose
*/
#define ESP_YIELD_MS 10
#define DEFAULT_BAUD 115200

#define NO_RX	0
#define ERR_RX	1
#define OK_RX	2
#define WIFI_CONN	3

class Esp01
{	
public:		
	explicit Esp01(Stream& stream) : stream(stream){};
	bool init();
	bool WiFiConnect(char* ssid, char* pass);
	bool WiFiDisconnect();
	bool TCPconnect(char* host, uint16_t port);
	bool TCPsend(char* data, uint16_t len);
	uint32_t TCPping(char* host);
	bool TCPclose();

private:
	Stream& stream;
	uint8_t listening(uint32_t timeout_ms);
	bool testSerial(uint32_t timeout_ms);
};

#endif
