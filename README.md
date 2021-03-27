# ESP-Riders

Use an ESP32 to control a small three-wheeled cars direction and speed with a L9110S H-bridge.

This will set up a wifi access point on the car which will allow a user to log in and access 192.168.4.1 which is the car's control html page.

Sample login on wifi, Network: ESPRidersBlueCar
                      Password: bbbbbbbb

then go to 192.168.4.1       

Parts required:  ESP32 devkit or similar.
                 L9110S H-bridge    
                 small 9g servo
                 buck converter if you power the car from a non-usb source
