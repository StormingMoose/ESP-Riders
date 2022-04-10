# ESP-Riders

Uses an ESP32 Dev kit module to control a small cars direction and speed with a L9110S H-bridge.

The code uses PWM and channels.

This will set up a wifi access point on the car which will allow a user to log in and access 192.168.4.1 which is the car's control html page.

Esp32 must be set up in preferences in Arduino IDE.  https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/


Library required:   
                 ServoESP32 

Parts required:       
                 ESP32 devkit or similar.    
                 L9110S H-bridge.     
                 Small 9g servo, two wires are switched to plug in the esp32 dev kit module.      
                 USB charging block with two outputs.    
                 Buck converter if you power the car from a non-usb source.   
                 Electric motor of your choice.   
                 
January 2022 added a lock to the screen to stop pinch to zoom when using two buttons at once.
                 Works for Android phones.
                 
