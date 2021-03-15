/*
 Arduino --> ThingSpeak Channel via MKR1000 Wi-Fi
 
 The ThingSpeak Client sketch is designed for the Arduino MKR1000 Wi-Fi.
 This sketch writes analog and Wi-Fi signal strength data to a ThingSpeak channel
 using the ThingSpeak API (https://www.mathworks.com/help/thingspeak).
 Arduino Requirements:
 
   * Arduino MKR1000 with ATMEL WINC1500 Wi-Fi module
   * Arduino 1.6.5+ IDE
 
 ThingSpeak Setup:
 
   * Sign Up for New User Account - https://thingspeak.com/users/sign_up
   * Create a new Channel by selecting Channels, My Channels, and then New Channel
   * Enable two fields
   * Note the Write API Key and Channel ID
   Tutorial: http://community.thingspeak.com/tutorials/arduino/using-the-arduino-mkr1000-with-thingspeak/
 
 Setup Wi-Fi
  * Enter SSID
  * Enter Password
   
 Created: May 7, 2016 by Hans Scharler (http://www.nothans.com)
 
 Additional Credits:
 Example sketches from Arduino team, Tom Igoe, and Federico Vanzati
 
*/

#include <SPI.h>
#include <WiFi101.h>

#include "TSIC.h"       // include the library

// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 6; // Analog output pin that the LED is attached to

int sensorValue = 0;        // value read from the pot
float outputValue = 0;        // value output to the PWM (analog out)


// instantiate the library, representing the sensor
TSIC Sensor1(4, 2);    // Signalpin, VCCpin, Sensor Type
//TSIC Sensor1(4, 2, TSIC_30x);    // Signalpin, VCCpin, Sensor Type
//TSIC Sensor2(5, 2, TSIC_50x);  // Signalpin, VCCpin, Sensor Type NOTE: we can use the same VCCpin to power several sensors

uint16_t temperature = 0;
float Temperatur_C = 0;
 
char ssid[] = "IPGOne"; //  your network SSID (name)
char pass[] = "grppa000"; // your network password

int status = WL_IDLE_STATUS;

// Initialize the Wifi client library
WiFiClient client;

// ThingSpeak Settings
char server[] = "api.thingspeak.com";
String writeAPIKey = "68FOW81CR62RBCF8";

unsigned long lastConnectionTime = 0; // track the last connection time
const unsigned long postingInterval = 20L * 1000L; // post data every 20 seconds

void setup() {
  // attempt to connect to Wifi network
  while ( status != WL_CONNECTED) {
    // Connect to WPA/WPA2 Wi-Fi network
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection
    delay(10000);
  }
  
  Serial.begin(9600);
}

void loop() {
  // if interval time has passed since the last connection,
  // then connect again and send data
  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
  }  

}

void httpRequest() {
  // read analog pin 0
  // read Wi-Fi signal strength (rssi)
  long rssi = WiFi.RSSI();

  
    // read the analog in value:
  sensorValue = analogRead(analogInPin);
  // map it to the range of the analog out:
  outputValue = float(sensorValue)/(700/2.22);
  //outputValue = map(sensorValue, 0, 700, 0, 3.3);
  // change the analog out value:
  
  analogWrite(analogOutPin, outputValue);

   if (Sensor1.getTemperature(&temperature)) {
   // Serial.print("uint_16: ");
   // Serial.println(temperature);
    Temperatur_C = Sensor1.calc_Celsius(&temperature)-20;
    Serial.print("Temperature: ");
    Serial.print(Temperatur_C);
    Serial.println(" °C");
  }

  
  // create data string to send to ThingSpeak
  String data = String("field1=" + String(sensorValue, DEC) + "&field2=" + String(Temperatur_C, DEC)); 
  
  // close any connection before sending a new request
  client.stop();

  // POST data to ThingSpeak
  if (client.connect(server, 80)) {
    client.println("POST /update HTTP/1.1");
    client.println("Host: api.thingspeak.com");
    client.println("Connection: close");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("X-THINGSPEAKAPIKEY: " +writeAPIKey);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.print(data.length());
    client.print("\n\n");
    client.print(data);

    // note the last connection time
    lastConnectionTime = millis();
    
  }

  
}
