/*
  Connect Arduino WiFi to AskSensors
 * Description:  This sketch connects the Arduino to AskSensors IoT Platform (https://asksensors.com) using an ESP8266 WiFi.
 *  Author: https://asksensors.com, 2018
 *  github: https://github.com/asksensors
 */
 

#include <SoftwareSerial.h>

// serial config
#define     RX    10
#define     TX    11
SoftwareSerial AT(RX,TX); 

// TODO: change user config
String ssid     = "............."; //Wifi SSID
String password = "............."; //Wifi Password
String apiKeyIn = "............."; // API Key

String host = "asksensors.com";  // host
String port = "80";      // port

int AT_cmd_time; 
boolean AT_cmd_result = false; 

void setup() {
  Serial.begin(9600);
  // open serial 
  Serial.println("*****************************************************");
  Serial.println("********** Program Start : Connect Arduino WiFi to AskSensors");
  AT.begin(115200);
  Serial.println("Initiate AT commands with ESP ");
  sendATcmd("AT",5,"OK");
  sendATcmd("AT+CWMODE=1",5,"OK");
  Serial.print("Connecting to WiFi:");
  Serial.println(ssid);
  sendATcmd("AT+CWJAP=\""+ ssid +"\",\""+ password +"\"",20,"OK");
}

void loop() {

 // Create the URL for the request
  String url = "GET /api.asksensors/write/";
  url += apiKeyIn;
  url += "?module1=";
  url += random(10, 100);
  Serial.println("*****************************************************");
  Serial.println("********** Open WiFi connection ");
  sendATcmd("AT+CIPMUX=1",10,"OK");
  sendATcmd("AT+CIPSTART=0,\"TCP\",\""+ host +"\","+ port,20,"OK");
  sendATcmd("AT+CIPSEND=0," + String(url.length()+ 4), 10, ">");
  
  Serial.print("********** requesting URL: ");
  Serial.println(url);
  AT.println(url);
  delay(2000);
  sendATcmd("AT+CIPCLOSE=0",10,"OK");
  
  Serial.println("********** WiFi Connection closed ");
  Serial.println("*****************************************************");
  
  delay(20000);   // delay
  }



// sendATcmd
void sendATcmd(String AT_cmd, int AT_cmd_maxTime, char readReplay[]) {
  Serial.print("AT command:");
  Serial.println(AT_cmd);

  while(AT_cmd_time < (AT_cmd_maxTime)) {
    AT.println(AT_cmd);
    if(AT.find(readReplay)) {
      AT_cmd_result = true;
      break;
    }
  
    AT_cmd_time++;
  }
  Serial.print("...Result:");
  if(AT_cmd_result == true) {
    Serial.println("DONE");
    AT_cmd_time = 0;
  }
  
  if(AT_cmd_result == false) {
    Serial.println("FAILED");
    AT_cmd_time = 0;
  }
  
  AT_cmd_result = false;
 }
