#define BLYNK_TEMPLATE_ID "TMPL62zJ8QCVd"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "#define BLYNK_TEMPLATE_ID "TMPL61fq_8by0"
#define BLYNK_TEMPLATE_NAME "firstproject"
#define BLYNK_AUTH_TOKEN "2LTSyz-eZpwCyDXXLt9JqMs3MnWrZ6Hn"
#include <Wire.h>
#include <BlynkSimpleEsp8266.h>
char c;
char auth[] = BLYNK_AUTH_TOKEN;
//#include "BlynkEdgent.h"
char ssid[] = "NetworksLab";  // WiFi SSID
char pass[] = "Networks123456";  // WiFi password
// char ssid[] = "Sahar's iPhone"; // Enter your WiFi name
// char pass[] = "11223344"; // Enter your WiFi password
// char ssid[] = "Banah hamdan";  // WiFi SSID
// char pass[] = "912345678";  // WiFi password
// // int M1F=D5;
// int M2F=D6;
// int pinval1;
// int pinval2;



void setup() {
  Serial.begin(9600);  // Initialize serial communication for debug
  Wire.begin(D1, D2);  // Join I2C bus with SDA=D1 and SCL=D2 of NodeMCU
    WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize Blynk library
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  Blynk.syncVirtual(V3);

  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);  // Initialize Blynk library
  //Wire.onReceive(receiveEvent);
  pinMode(D0, OUTPUT);  // Set the LED pin as an output pin
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);//////// ToY
}
 

void loop() {
  Wire.beginTransmission(8);  // Begin with device address 8
  //Wire.write("Hello Arduino");  // Send hello string
  //BLYNK_WRITE(V3);
  Wire.endTransmission();  // Stop transmitting
  
  Blynk.run();  // Run the Blynk library
   Blynk.virtualWrite(V1, 0);  // Trigger Blynk action for 'n'
    Blynk.virtualWrite(V2, 255); 
  delay(1000);
  Wire.requestFrom(8, 13);  // Request & read data of size 13 from slave
  while (Wire.available()) {
    c = Wire.read();  // Read the received character
    Serial.println(c);  // Print the received character for debugging
    
    // Perform actions based on the received character
    if (c == 'm') {
      Blynk.virtualWrite(V0, 255);  // Trigger Blynk action for 'm'
    } else if (c == 'c') {
      myTimerEvent();
      // Add your logic for 'c' here
      // For example: Blynk.virtualWrite(V2, 255);
    } else if (c == 'n') {
      Blynk.virtualWrite(V1, 255);  // Trigger Blynk action for 'n'
      Blynk.virtualWrite(V2, 0); 
    }
    Serial.println(c);
    delay(2000);
  }
  

  
}

// Get the button value from Blynk
// BLYNK_WRITE(V4) {
//   digitalWrite(D6, param.asInt());
// }
BLYNK_WRITE(V3) {
    digitalWrite(D5, param.asInt());
  //int i6=0;
  int buttonValue = param.asInt();
   if (buttonValue == HIGH){
        Serial.println('D');
        delay(50);
    }
}

BLYNK_WRITE(V5) {
    digitalWrite(D6, param.asInt());
  //int i6=0;
  int toyval = param.asInt();
   if (toyval == HIGH){
        Serial.println('t');
        delay(50);
    }
}



void myTimerEvent(){
  Blynk.logEvent("cry","your Baby is crying");
}
