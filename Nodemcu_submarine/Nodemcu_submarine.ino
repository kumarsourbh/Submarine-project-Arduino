// Program by Sourbh Kumar under GPL 3.0
// Can be shared and reproduced for educational purposes
//Submarine control program to operate the propeller and DIVE IN - DIVE OUT Feature with NodeMCU wifi connectivity.
// App design can be found at MIT APP INVENTOR platform @Sourbh_Kumar Projects.



#define ENA   14          // Enable/speed motors Right        GPIO14(D5)
#define ENB   12          // Enable/speed motors Left         GPIO12(D6)
#define IN_1  15          // L298N in1 motors Rightx          GPIO15(D8)
#define IN_2  13          // L298N in2 motors Right           GPIO13(D7)
#define IN_3  2           // L298N in3 motors Left            GPIO2(D4)
#define IN_4  0           // L298N in4 motors Left            GPIO01(D3)
#define IN_5 5            ///L298N IN1 for Pump2              GPIO17(D1)
#define IN_6 4            //L293D IN2 for Pump2               GPIO04(D2)

const int buzzerPin = D0; // You can change this to any digital pin Need to be added into program but after test for sound 
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
String command; //String to store app command state.
const char* ssid = "Submarine";
ESP8266WebServer server(80);
 
void setup() {
pinMode(ENA, OUTPUT);
pinMode(ENB, OUTPUT);  
pinMode(IN_1, OUTPUT);
pinMode(IN_2, OUTPUT);
pinMode(IN_3, OUTPUT);
pinMode(IN_4, OUTPUT);
pinMode(IN_5, OUTPUT);
pinMode(IN_6, OUTPUT);
pinMode(buzzerPin, OUTPUT);
  
  Serial.begin(115200);
  
// Connecting WiFi
 
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
     server.on ( "/", HTTP_handleRoot );
     server.onNotFound ( HTTP_handleRoot );
     server.begin();    
}
 
void goAhead(){
 
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      digitalWrite(ENA, HIGH);
  }
 
void goBack(){
 
      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      digitalWrite(ENA, HIGH);
  }
 
void goRight()
  {
      digitalWrite(IN_5, LOW);
      digitalWrite(IN_6, HIGH);
  }
 
void goLeft()
  {
 
      digitalWrite(IN_5, HIGH);
      digitalWrite(IN_6, LOW);
  }
 
void stopRobot()
  {  
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, LOW);
      digitalWrite(ENA, HIGH);
      digitalWrite(IN_5, LOW);
      digitalWrite(IN_6, LOW); 
      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, LOW);
      digitalWrite(ENB, HIGH);
}

void playSubmarineRaidSound() {
  // Submarine raid sound frequencies
  int frequencies[] = {300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200};

  // Duration for each note
  int noteDuration = 100;

  // Iterate through the frequencies and play the raid sound
  for (int i = 0; i < 10; i++) {
    tone(buzzerPin, frequencies[i], noteDuration);
    delay(noteDuration);
    noTone(buzzerPin); // Stop the sound
    delay(50); // Add a small delay between notes
  }
}
 
void loop() {

      playSubmarineRaidSound();
      delay(5000);
      server.handleClient();
      command = server.arg("State");
      if (command == "F") goAhead();
      else if (command == "B") goBack();
      else if (command == "L") goLeft();
      else if (command == "R") goRight();  
      else if (command == "S") stopRobot();
}
 
void HTTP_handleRoot(void) {
 
if( server.hasArg("State") ){
       Serial.println(server.arg("State"));
  }
  server.send ( 200, "text/html", "" );
  delay(1);
}
