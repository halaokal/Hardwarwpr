#include <Wire.h>
// #include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

//Servo------------------------------------------------
 Servo myservo1;  
 Servo myservo2; 
 int servoPos = 0;
 int servoflage=0; 
// moisture sensor-----------------------------
int moistureSensor_pin = A1;
int moistureSensor_value = 0;
int moistureSensor_led = 4;
boolean moistureSensor_flag = false;
// ldr light sensor-----------------------------
const int lightSensor_pin = 3;
int lightSensor_value = 0;
int lightSensor_led = 9;
//Dc motor -------------------------------------
int DcMotor_IN1 = 7;
int DcMotor_IN2 = 8;
int DcMotor_ENA = 5;
int DcMotor_speed = 210;
unsigned long DcMotor_startTime = 0;
const unsigned long DcMotor_movementTime = 2000;

//////////dc with esp;
int rundcval=9;
//toy_PIN--------------------
int toy_PIN=39;
int toyval=22;
//int toF;
//Room & baby temp-----------------------------
#define ONE_WIRE_BUS 11
OneWire oneWire(ONE_WIRE_BUS);
LiquidCrystal_I2C lcd(0x3F, 16, 2);
int FAN_PIN = 6;
DallasTemperature sensors(&oneWire);
int gate = 2;
volatile unsigned long duration = 0;
unsigned char i[5];
unsigned int j[40];
unsigned char value = 0;
unsigned answer = 0;
int z = 0;
int b = 1;
unsigned long previousMillis = 0; // Store the last time the temperature was updated
const long interval = 5000;  
//motion-----------------------------
const int motionSensor_pin = 12;
int motionSensor_led = 13;
int motionSensor_value ;
//sound-----------------------------
int sensorApin = A0;
int sensorDpin = 13;
int analogValue = 0;
int digitalValue;
int led = 13;
int swingCount = 0;
const int totalSwings = 3; 


void setMotorDirection(boolean direction) {
  digitalWrite(DcMotor_IN1, direction);
  digitalWrite(DcMotor_IN2, !direction);
}

void setup() {
  
  Serial.begin(9600);
  
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
   //Servo------------------------------------------------
   //myservo1.attach(4);  
   myservo2.attach(10);  
   //myservo1.write(180);
// moisture sensor-----------------------------
  pinMode(moistureSensor_pin, INPUT);
  pinMode(moistureSensor_led, OUTPUT);
// ldr light sensor-----------------------------
  pinMode(lightSensor_pin, INPUT);
  pinMode(lightSensor_led, OUTPUT);
//Dc motor-----------------------------
  pinMode(DcMotor_IN1, OUTPUT);
  pinMode(DcMotor_IN2, OUTPUT);
  pinMode(DcMotor_ENA, OUTPUT);
  analogWrite(DcMotor_ENA, DcMotor_speed);
  digitalWrite(DcMotor_IN1, HIGH);
  digitalWrite(DcMotor_IN2, LOW);
  DcMotor_startTime = millis();
  //Room & baby temp-----------------------------
  sensors.begin();
  lcd.init();
  lcd.backlight();
    Wire.begin(8); // Set up I2C communication with address 8
    Wire.onReceive(receiveEvent); // Register the receiveEvent function
    Wire.onRequest(requestEvent);
//motion-----------------------------
  pinMode(motionSensor_pin, INPUT);
  pinMode(motionSensor_led, OUTPUT);
//sound-----------------------------
  pinMode(sensorDpin, INPUT);
  pinMode(led, OUTPUT);
  //////////////////////////
  
  pinMode(rundcval,INPUT);
  pinMode(toyval,INPUT);///toy
  
}

void loop() {
  //dc----------------------------
 int DCF=digitalRead(rundcval);
 Serial.print("run dc value is :");
 Serial.println(DCF);
 //toyval---------------------------------------
 int toF=0;
 toF=digitalRead(toyval);
 Serial.print("toy value is :");
 Serial.println(toF);
  // moisture sensor-----------------------------
  moistureSensor_value = analogRead(moistureSensor_pin);
  Serial.print("moisture = ");
  Serial.println(moistureSensor_value);
  if ((moistureSensor_value <= 300) && (moistureSensor_flag == false)) {
    digitalWrite(moistureSensor_led, HIGH);

    moistureSensor_flag = true;
  //  delay(1000);
  }
  if ((moistureSensor_value > 300) && (moistureSensor_flag == true)) {
    digitalWrite(moistureSensor_led, LOW);
    moistureSensor_flag = false;
   // delay(1000);
  }
// ldr light sensor-----------------------------
  lightSensor_value = digitalRead(lightSensor_pin);
  Serial.print("light Sensor value = ");
  Serial.println(lightSensor_value);
  ////servo
  if (lightSensor_value == HIGH ) {
    Serial.println("NO light");
     if(servoflage==1){

      myservo2.write(0);
      servoflage=0;
    }
    //digitalWrite(lightSensor_led, LOW);
    
  }
  if (lightSensor_value == LOW) {
    
    Serial.println("there is light");
    if(servoflage==0){
        for (int angle = 0; angle <= 120; angle++) {

      myservo2.write(120);
      servoflage=1;
        }
    }
   // digitalWrite(lightSensor_led, HIGH);
    
    
  }
//Dc motor-----------------------------

if(DCF==1){

 // while (swingCount < totalSwings) {
    unsigned long DcMotor_currentTime = millis();
    unsigned long DcMotor_elapsedTime = DcMotor_currentTime - DcMotor_startTime;

    if (DcMotor_elapsedTime >= DcMotor_movementTime) {
      setMotorDirection(!digitalRead(DcMotor_IN1));
      DcMotor_startTime = DcMotor_currentTime;
      swingCount++;
    }
    
    delay(500);  // Delay between swings
  }

  // unsigned long DcMotor_currentTime = millis();
  // unsigned long DcMotor_elapsedTime = DcMotor_currentTime - DcMotor_startTime;
  // if (DcMotor_elapsedTime >= DcMotor_movementTime) {
  //   setMotorDirection(!digitalRead(DcMotor_IN1));
  //   DcMotor_startTime = DcMotor_currentTime;
  // }

  //delay(500);
//}
else if (DCF==0) {
  digitalWrite(DcMotor_IN1, LOW);
  digitalWrite(DcMotor_IN2, LOW);
}

//////////////////////////////////////
 // millis();
if(toF==1){
  pinMode(toy_PIN, OUTPUT);
  digitalWrite(toy_PIN, LOW);
}
 else if(toF==0){
   pinMode(toy_PIN, OUTPUT);
   digitalWrite(toy_PIN, HIGH);
 }
  


////////////////////////////////////////





//Room & baby temp-----------------------------
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    sensors.requestTemperatures();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("babyTemp:");
    lcd.setCursor(0, 1);
    lcd.print(sensors.getTempCByIndex(0));
    lcd.setCursor(6, 1);
    lcd.print("C");
    pinMode(gate, OUTPUT);
    digitalWrite(gate, LOW);
    delay(20);
    digitalWrite(gate, HIGH);
    pinMode(gate, INPUT_PULLUP);
    duration = pulseIn(gate, LOW);
  if (duration <= 84 && duration >= 72)
  {
    while (1)
    {
        duration = pulseIn(gate, HIGH);

        if (duration <= 26 && duration >= 20)
        {
            value = 0;
        }
        else if (duration <= 74 && duration >= 65)
        {
            value = 1;
        }
        else if (z == 40)
        {
            break;
        }

        i[z / 8] |= value << (7 - (z % 8));
        j[z] = value;
        z++;
    }
  }
  answer = i[0] + i[1] + i[2] + i[3];
  if (answer == i[4] && answer != 0)
  {
    lcd.init();
    lcd.backlight();
    lcd.print("Temp = ");
    lcd.setCursor(0, 1);
    lcd.print("Humidity = ");
    lcd.setCursor(9, 0);
    lcd.write(1);
    lcd.print("C");
    lcd.setCursor(13, 1);
    lcd.print("%");

    lcd.setCursor(7, 0);
    lcd.print(i[2]);
    Serial.println(i[2]);
    lcd.setCursor(11, 1);
    lcd.print(i[0]);
    Serial.println(i[0]);
  }
  z = 0;
    i[0] = i[1] = i[2] = i[3] = i[4] = 0;
  }
  delay(6000);
  //FAN
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);  // Index 0 for the first sensor
  if (temperature >= 20) {
    // Do something when temperature is above 35°C
    digitalWrite(FAN_PIN, LOW);
  } else {
    digitalWrite(FAN_PIN, HIGH);
  }
  delay(1000);
//motion-----------------------------
   motionSensor_value = digitalRead(motionSensor_pin);
  if (motionSensor_value == HIGH) {
    digitalWrite(motionSensor_led, HIGH);
    Serial.println("there is a motion for baby ");
  } else {
    digitalWrite(motionSensor_led, LOW);
    Serial.println("no for baby ");
  }

  delay(1000);
  //sound-----------------------------
  digitalValue=0;
  analogValue = analogRead(sensorApin);
  digitalValue = digitalRead(sensorDpin);
  Serial.print("/////////////////sound is :");
  Serial.println(analogValue);
  Serial.print("/////////////////digital sound is :");
  Serial.println(digitalValue);
  

  // if (digitalValue == HIGH) {
  //   digitalWrite(led, HIGH);
  // } else {
  //   digitalWrite(led, LOW);
  // }
  
  //delay(5000);


   //Servo------------------------------------------------ 
}

void receiveEvent(int howMany) {
  while (0 < Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
    if(c=='D'){
      Serial.println("hi D");
    }
  }
  Serial.println();
}
void requestEvent() {
  if ((moistureSensor_value <= 300)){
    //Wire.beginTransmission(8);  
    Wire.write('m');//baby diabres
    //Wire.endTransmission(); 
  }
  if (digitalValue==1){
    //Wire.beginTransmission(8); 
    Wire.write('c'); //soundsensor
    //Wire.endTransmission(); 
  }
  if (motionSensor_value == HIGH) {
    // digitalWrite(motionSensor_led, HIGH);
    // Serial.println("there is a motion for baby ");
   // Wire.beginTransmission(8); 
    Wire.write('n'); // motinbaby 
   //Wire.endTransmission(); 
   Serial.print('n');
  }
}
  