// ESP8266 Wireless Library
// Supports NodeMCU Firmware Software
// Arduino Hardware "Tested on the Arduino UNO Platform"
// By Mo'meN M. Ali, 17 of September, 2015
// IBM Cor.

#include "Arduino.h"
#include "ESP8266.h"

// Constructor
//Input: the SoftwareSerial chosed, the publish topic
ESP8266::ESP8266 (SoftwareSerial *stream, String Publishtopic)
{
	_stream= stream;
	_Publishtopic= Publishtopic;
}

// Establish a connection with the esp to a chosed topic
// Input: wireless username and password, topic and message
// topic to publish as a test
// using "0" disables the test publish
void ESP8266::Init (String UserName, String Passwd, String TestTopic, String PubMsg)
{
	String connectionCommand= "0", connectionChecker= "0";
	
	_stream->begin (9600);
// Assuming entering valid strings
	do
	{
		Serial.println ("Connecting...");
	//  Initalize the ESP8266
		_stream->println("wifi.sta.config(\""+UserName+"\",\""+Passwd+"\")");
		ESP8266_ReadResponce (_stream); 
		_stream->println("wifi.sta.connect()");
		ESP8266_ReadResponce (_stream); 
		_stream->println("m = mqtt.Client(\"clientid\", 120, \"uwgmsjio\", \"PmlV_LIgm_0G\")");
		ESP8266_ReadResponce (_stream);
		connectionCommand= "m:connect(\"m11.cloudmqtt.com\", 10415, 0, function(conn) print(\"connected@\") end)";
		_stream->println(connectionCommand);
		delay (10);
		//Serial.println ( ESP8266_ReadResponceIUntilConnected (_stream) ); 
		//Serial.println ( ESP8266_ReadResponce (_stream) );
		 connectionChecker= ESP8266_ReadResponceIUntilConnected (_stream);
		 //Serial.println ( ESP8266_ReadResponceIUntilConnected (_stream) );
		 //Serial.println ( ESP8266_ReadResponce (_stream) );
		 connectionChecker+= ESP8266_ReadResponce (_stream);
		 connectionChecker+= ESP8266_ReadResponce (_stream);
	} while ( (connectionChecker < connectionCommand) ||
			  (connectionChecker == connectionCommand) );
	delay (10);
	String test;
	if (TestTopic != "0") // Entering 0 means no subscription
	{
	// Dummy publish for testing
		connectionCommand= "m:publish(\""+TestTopic+"\",\""+PubMsg+"\", 0, 0, function(conn) print(\"sent\") end)";
		_stream->println(connectionCommand);
		connectionChecker= ESP8266_ReadResponce (_stream); 
	}


}

// 	Publsh message
String ESP8266::PublishMsg (String message)
{
	String command= "m:publish(\""+_Publishtopic+"\",\""+message+"\", 0, 0, function(conn) print(\"sent\") end)";
	_stream->println(command); 
	delay (10);
	return (ESP8266_ReadResponce (_stream));	
}

// Publish a long message 
// > 64 byte
String ESP8266::PublishLongMsg (String msg)
{
	_stream->print ("m:publish(\""+_Publishtopic+"\",");
	_stream->print (msg);
	_stream->println (", 0, 0, function(conn) print(\"sent\") end)");
	return (ESP8266_ReadResponce(_stream));
}

// Publish a message into JSON format
// Input: ID of the thing, ID of the container, put/remove
String ESP8266::PublishJSON (String thingID, String containerID, String action)
{
	_stream->print ("m:publish(\""+_Publishtopic+"\",");
	_stream->print ("\"{\\\"thing_id\\\" : "+thingID+" ,\\\"container_id\\\" : "+containerID+" , \\\"action\\\" : \\\""+action+"\\\"}\"");
	_stream->println (", 0, 0, function(conn) print(\"sent\") end)");
	return (ESP8266_ReadResponce(_stream));
	
}

// for Topic Subscription
// Input: name of the topic to be subscribed
String ESP8266::Subscribe (String topic)
{
// Subscribe
	String command= "m:subscribe(\""+topic+"\",0, function(conn) print(\"subscribe success\") end)";
	_stream->println (command);
	return ( ESP8266_ReadResponce (_stream) );
}

// Receive a message from subscribed topic
// Output: returns a msg string
String ESP8266::ReceiveMsg (void)
{
  // receive from Server
    String mqttRecvCmd, mqttRecvMsg;
    mqttRecvCmd= "m:on(\"message\", function(conn, topic, data) print(topic .. \":\" ) if data ~= nil then print(data) end end)";
    _stream->println (mqttRecvCmd);
	mqttRecvMsg= ESP8266_ReadResponce (_stream);
    if (mqttRecvMsg.length() > mqttRecvCmd.length())
		return (mqttRecvMsg);
	//else
		//return "0";
}

int interval= 10000;

// Reads the returned responce from the ESP8266 after sending a command
// Input: none
// Output: returns pointer-to-char (The string responce)
String ESP8266::ESP8266_ReadResponce(SoftwareSerial *_stream)
{
     String msg;
     unsigned long currentMillis = millis();
     unsigned long previousMillis = millis();
     while ( !_stream->available () )  
	 {
            currentMillis = millis();
            if (currentMillis - previousMillis >= interval) 
			{
				previousMillis = currentMillis;    // Max time required by the mqtt to reply else there is no reply
				return "Error";
            }
     }

     msg= _stream->readString ();
     return msg;
}


String ESP8266::ESP8266_ReadResponceIUntilConnected(SoftwareSerial *_stream)
{
     unsigned long currentMillis = millis();
     unsigned long previousMillis = millis();
     
     while ( !_stream->available () )  
	 {
            currentMillis = millis();
            if (currentMillis - previousMillis >= interval) 
			{
				previousMillis = currentMillis;    // Max time required by the mqtt to reply else there is no reply
				return "Error";
            }
     }
     
     return ( _stream->readStringUntil ('@') );
}