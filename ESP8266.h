

#include "Arduino.h"
#include <SoftwareSerial.h>

// RX buffer on the ESP is 256     byte
// SoftwareSerial RX buffer is 64  byte

class ESP8266
{
	public:
	// Constructor
	//Input: the SoftwareSerial chosed, the publish topic 
		ESP8266               (SoftwareSerial *, String);
	// Establish a connection with the esp to a chosed topic
	// Input: wireless username and password, topic and message
	// topic to publish as a test
	// using "0" disables the test publish
		void   Init           (String, String, String, String);
	// 	Publsh message
		String PublishMsg     (String);
	// Publish a long message 
	// > 64 byte
		String PublishLongMsg (String);
	// Publish a message into JSON format
	// Input: ID of the thing, ID of the container, put/remove
		String PublishJSON    (String, String, String);
	// for Topic Subscription
	// Input: name of the topic to be subscribed
		String Subscribe 	  (String);
	// Receive a message from subscribed topic
	// Output: returns a msg string
		String ReceiveMsg	  (void);
	private:
		SoftwareSerial *_stream;
		String _Publishtopic;
		String ESP8266_ReadResponceIUntilConnected(SoftwareSerial *_stream);
		String ESP8266_ReadResponce(SoftwareSerial *_stream);
};