/*
   Adapted from the following:
   
   ESP32 Web Server Demo using Accesspoint
   https://circuits4you.com
   21-11-2018
*/


#include <Servo.h>
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

// GPIO the servo is attached to
static const int servoPin = 15;
const int Flight = 2;

int pos = 90; // intitial servo postion to be at 90 degrees to make it straight different from 90 for mechanical adjustment
// wired connections
#define MOTOR_PWM 33 //  Speed
#define MOTOR_DIR 32 //  Direction


// the actual values for "fast" and "slow" depend on the motor
#define dir_Forward 0
#define dir_Back 255

#define PWM_STOP 0
#define PWM_SLOW 220 // arbitrary slow speed PWM duty cycle
#define PWM_FAST 255 // arbitrary fast speed PWM duty cycle
#define DIR_DELAY 1000 // brief delay for abrupt motor changes

const int freq = 5000;
const int ledChannel_PWM = 10;
const int ledChannel_DIR = 11;
const int resolution = 8;



#include <WiFi.h>
//#include <WiFiClient.h>
#include <WebServer.h>
//#include <AsyncTCP.h>

//#include <ESPAsyncWebServer.h>

#include "index.h"  //Web page header file



//Enter your desired WiFi SSID and PASSWORD
const char* ssid = "ESP_Rider_Little_Truck";
const char* password = "********";

WebServer server(80);

//===============================================================
// Setup
//===============================================================
void setup(void) {

  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");

  pinMode(servoPin, OUTPUT);
  myservo.attach(servoPin);
  myservo.write(pos);
  Serial.println(pos);

  pinMode(Flight, OUTPUT);

  
  // configure LED PWM functionalitites
  ledcSetup(ledChannel_PWM, freq, resolution);
  ledcSetup(ledChannel_DIR, freq, resolution);


  // attach the channel to the GPIO to be controlled
  ledcAttachPin(MOTOR_PWM, ledChannel_PWM);
  ledcAttachPin(MOTOR_DIR, ledChannel_DIR);

  pinMode( MOTOR_DIR, OUTPUT );
  pinMode( MOTOR_PWM, OUTPUT );
  ledcWrite( ledChannel_DIR, dir_Forward );
  ledcWrite( ledChannel_PWM, PWM_STOP );

  digitalWrite(Flight, LOW); //Flashlight will turn off

  //ESP32 As access point IP: 192.168[.4.1
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP(ssid, password);    //Password length minimum 8 char

 
  server.on("/", handle_OnConnect);
  server.on("/A", handle_Forward_Fast);
  server.on("/B", handle_Forward_Slow);
  server.on("/C", handle_Back_Slow);
  server.on("/D", handle_Back_Fast);
  server.on("/off", handle_Stop);
  server.on("/R", handle_Go_Righter);
  server.on("/L", handle_Go_Lefter);
  server.on("/FO", handle_Flashlight_Off);
  server.on("/Pos", handle_Steering_Slider_Input);


  server.onNotFound(handle_NotFound);

  server.begin();                  //Start server
  Serial.println("HTTP server started");
}

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void loop(void) {
  server.handleClient();

}
void handle_NotFound () {

}


void handle_OnConnect() {

  String s = MAIN_page; //Read HTML contents
  server.send(200, "text/html", s); //Send web page

}

void handle_Steering_Slider_Input() {
//  server.send(200, "text/plain", "ok");
  if(server.arg("value") ){
  int slidervalue = server.arg("value").toInt();   //
  
  Serial.print("slidervalue = ");
  
 // myservo.write(slidervalue);
  Serial.println(slidervalue);
  pos = slidervalue;
  myservo.write(slidervalue);
  }
 
server.send(200, "text/plain", "ok");
}

void handle_Forward_Fast() {
  digitalWrite(Flight, HIGH); // Flash light will turn on
  ledcWrite( ledChannel_DIR, dir_Forward ); // direction = forward
  ledcWrite( ledChannel_PWM, PWM_FAST ); // PWM speed = fast
  server.send(200, "text/plain", "ok");
}

void handle_Forward_Slow() {

  digitalWrite(Flight, LOW);
  ledcWrite( ledChannel_DIR, dir_Forward ); // direction = forward
  ledcWrite( ledChannel_PWM, PWM_SLOW  ); // PWM speed = fast
  server.send(200, "text/plain", "ok");
}

void handle_Back_Slow() {
  digitalWrite(Flight, LOW);
  ledcWrite( ledChannel_DIR, dir_Back ); // direction = Back
  ledcWrite( ledChannel_PWM, 320 - PWM_SLOW ); // PWM speed = Slow  ledChannel_PWM, 180 - PWM_SLOW
  server.send(200, "text/plain", "ok");
}

void handle_Back_Fast() {
  digitalWrite(Flight, LOW);
  ledcWrite( ledChannel_DIR, dir_Back ); // direction = Back
  ledcWrite( ledChannel_PWM, 255 - PWM_FAST ); // PWM speed = Retro
  server.send(200, "text/plain", "ok");
}

void handle_Stop() {
  digitalWrite(Flight, LOW);
  ledcWrite( ledChannel_PWM, PWM_STOP );
  ledcWrite( ledChannel_DIR, dir_Forward);
  server.send(200, "text/plain", "ok");
}

void handle_Go_Lefter() { // Receive an HTTP GET request for steering.  Depending on the position of sg90 servo could change name to Righter
  digitalWrite(Flight, HIGH);
  pos = pos - 7;
  if (pos < 7) {
    pos = 7;
  }
  myservo.write(pos);

  server.send(200, "text/plain", "ok");
}


void handle_Go_Righter() { // Receive an HTTP GET request for steering.  And from above comment this could be renamed Lefter instead
  digitalWrite(Flight, HIGH);
  pos = pos + 7;
  if (pos > 173) {
    pos = 173;
  }
  myservo.write(pos);

  server.send(200, "text/plain", "ok");
}

void handle_Flashlight_Off() {
  digitalWrite(Flight, LOW);
  server.send(200, "text/plain", "ok");
}
