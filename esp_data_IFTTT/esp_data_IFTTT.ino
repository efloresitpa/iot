/************************************************************
ESP_IFTTT_Maker_Data.ino
ESP WiFI Secure library - IFTTT Channel Maker Posting Example
Andres Sabas @ The Inventor's House
Original Creation Date: Jan 10, 2016

This example demonstrates how to use the TCP client
functionality of the ESP WiFiSecure library to post
data to a IFTTT event on
https://ifttt.com

Development environment specifics:
  IDE: Arduino 1.8.9
  Hardware Platform:
  ESP8266 and ESP32

This code is beerware; if you see me (or any other The Inventor's House
member) at the local, and you've found our code helpful,
please buy us a round!

Distributed as-is; no warranty is given.
************************************************************/
// Include the ESP8266 and ESP32 WiFiSecure library:
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "arduino_secrets.h"

Adafruit_BME280 bme;
float temperature = 0;
float humidity = 0;
float pressure= 0;
String temp ="";
String hum ="";
String pres ="";
String vr1;
String v1;
//String readHumidity;
/////////////////////
// IFTTT Constants //
/////////////////////
const char* IFTTTServer = "maker.ifttt.com";
// IFTTT https por:
const int httpsPort = 80;
// IFTTT Name Event:
const String MakerIFTTT_Event = "esp32_tweet";
// IFTTT private key:

String httpHeader = "POST /trigger/"+MakerIFTTT_Event+"/with/key/"+MakerIFTTT_Key +" HTTP/1.1\r\n" +
                    "Host: " + IFTTTServer + "\r\n" +
                    "Content-Type: application/json\r\n";


void setup()
{
  int status;
  Serial.begin(9600);
  Serial.println();
  Serial.print(F("connecting to "));
  Serial.println(mySSID);
  WiFi.begin(mySSID, myPSK);
  while (WiFi.status() != WL_CONNECTED) {
    
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address is: "));
  Serial.println(WiFi.localIP());

  Serial.println(F("Press any key to post to IFTTT!"));
}

void loop()
{
  // If a character has been received over serial:
  if (Serial.available())
  {
    // !!! Make sure we haven't posted recently
    // Post to IFTTT!
    postToIFTTT();
    // Then clear the serial buffer:
    while (Serial.available())
      Serial.read();
  }
}
/*
void handle_OnConnect() {
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 100.0F;
  //altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  //server.send(200, "text/html", SendHTML(temperature,humidity,pressure,altitude)); 
}*/


void postToIFTTT()
{
  // Create a client, and initiate a connection
  WiFiClient client;
  if (client.connect(IFTTTServer, httpsPort) <= 0)
  {
    Serial.println(F("Failed to connect to server."));
    return;
  }
  Serial.println(F("Connected."));
  
  float temperature = (bme.readTemperature(),0);
  temp.concat(temperature);
  float humidity = (bme.readHumidity(),0);
  hum.concat(humidity);
 float pressure = (bme.readPressure(),0);
 pres.concat(pressure);
  //  String data="{\"value1\":\""+temp+"\",\"value2\":\""+hum+"\",\"value3\":\""+pres+"\"}";

  //String data="{"value1 : "+temp+", value2 : " +hum+ ", value3 : "+pres+"}";
  //  String data="{value1 : 25, value2 : 32, value3 : 8122}";
   //String data="{value1 : "+temp+", value2 : "+hum+",value3 : " +pres+"}";
  String data="{\"value1\":\"1\",\"value2\":\"1\"}";


  Serial.println(F("Posting to IFTTT!"));
  
  //Serial.println(data.length());

  client.print(httpHeader);
  client.print("Content-Length: "); 
  client.println(data.length());
  client.println();
  client.println(data);

  // available() will return the number of characters
  // currently in the receive buffer.
  while (client.available())
    Serial.write(client.read()); // read() gets the FIFO char

  // connected() is a boolean return value - 1 if the
  // connection is active, 0 if it's closed.
  if (client.connected())
    client.stop(); // stop() closes a TCP connection.
}
