#include<LiquidCrystal.h>
#include <NewPing.h>
#include <Wire.h>

#define Register_ID 0
#define Register_2D 0x2D
#define Register_X0 0x32
#define Register_X1 0x33
#define Register_Y0 0x34
#define Register_Y1 0x35
#define Register_Z0 0x36
#define Register_Z1 0x37

int ADXAddress = 0xA7 >> 1;  // the default 7-bit slave address
int reading = 0;
//int val=0;
int X0,X1,X_out;
int Y0,Y1,Y_out;
int Z1,Z0,Z_out;
double Xg,Yg,Zg;
 float volume;
 float volume_water;
 const float pie = 3.142;
 const float r = 3.5;
 const float radius =5.5;
 //const float total_volume = 1710.819;//blue
//const float total_volume = 846.77;// 22 cm
//const float total_volume = 885.2585;//23 cm
const float total_volume = 808.2795;//21 cm

 const int trigPin = 9;
    const int echoPin = 10;
NewPing sonar1(trigPin, echoPin, 200);
#define pingSpeed 100 // Ping frequency (in milliseconds), fastest we should ping is about 35ms per sensor
unsigned long pingTimer1;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  // sets the interfacing pins

float vout;  //temporary variable to hold sensor reading
int inPin = 7;   // choose the input pin (for a pushbutton)
int val = 0;     // variable for reading the pin status

    // defines variables
    long duration,cm;
    int distance;
    long microsecondsToCentimeters(long microseconds)
{
  
  return microseconds / 29 / 2;
}
void setup()
{
  Wire.begin();

Serial.begin(9600);
 
  pingTimer1 = millis() + pingSpeed; // Sensor 1 fires after 100ms (pingSpeed)
  pinMode(inPin, INPUT);    // declare pushbutton as input
   // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
     
  delay(100);
  // enable to measute g data
  Wire.beginTransmission(ADXAddress);
  Wire.write(Register_2D);
  Wire.write(8);                //measuring enable
  Wire.endTransmission();     // stop transmitting

    // Calculating the distance
    distance= duration*0.034/2;
    cm = microsecondsToCentimeters(duration);
   
  lcd.begin(16, 2);  // initializes the 16x2 LCD
}
//float calculation(){
//  float water_level = 0;// store level in every step
//  int read_value = 0; //read sensor reading in cm
//float average_water_level =0;
//  for(int i=0; i<5; i++){ //take five reading
//      read_value = sonar1.ping_cm();
//      Serial.println("Distance: ");
// Serial.print(read_value);
//      if(read_value>24 || read_value<3){// unstable reading
//          return -10000; //return to calling function because reading is unstable
//        }
//      else if(read_value<=24 && read_value>=3){//valid value      
//          water_level = water_level + read_value;
//        }
//      delay(10);
//      
//   }
  
//  average_water_level = 17 - water_level/5;
//  return average_water_level;}
void loop() 
{
  val = digitalRead(inPin);
Wire.beginTransmission(ADXAddress); // transmit to device
  Wire.write(Register_X0);
  Wire.write(Register_X1);
  Wire.endTransmission();
  Wire.requestFrom(ADXAddress,2); 
  if(Wire.available()<=2)   
  {
    X0 = Wire.read();
    X1 = Wire.read(); 
    X1=X1<<8;
    X_out=X0+X1;   
  }

  //------------------Y
  Wire.beginTransmission(ADXAddress); // transmit to device
  Wire.write(Register_Y0);
  Wire.write(Register_Y1);
  Wire.endTransmission();
  Wire.requestFrom(ADXAddress,2); 
  if(Wire.available()<=2)   
  {
    Y0 = Wire.read();
    Y1 = Wire.read(); 
    Y1=Y1<<8;
    Y_out=Y0+Y1;
  }
  //------------------Z
  Wire.beginTransmission(ADXAddress); // transmit to device
  Wire.write(Register_Z0);
  Wire.write(Register_Z1);
  Wire.endTransmission();
  Wire.requestFrom(ADXAddress,2); 
  if(Wire.available()<=2)   
  {
    Z0 = Wire.read();
    Z1 = Wire.read(); 
    Z1=Z1<<8;
    Z_out=Z0+Z1;
  }
  //
  Xg=X_out/256.0;
  Yg=Y_out/256.0;
  Zg=Z_out/256.0;
  Serial.print("X= ");
  Serial.print(Xg);
  Serial.print("       ");
  Serial.print("Y= ");
  Serial.print(Yg);
  Serial.print("       ");
  Serial.print("Z= ");
  Serial.print(Zg);
  Serial.println("  ");
  if(Xg<-0.85 && Xg>-1.15)
  {
     pingTimer1 += pingSpeed; // Make sensor 1 fire again 100ms later (pingSpeed)
  //float in1 = sonar1.ping_cm();
 
  float  in1 = sonar1.ping_cm();
   delay(50);
   float in2 = sonar1.ping_cm();
   if(in1==in2){
  if(in1>3 && in1<24){
  // Prints the distance on the Serial Monitor
    Serial.println("Distance: ");
   Serial.print(in1);
//   float in1 = calculation();
  volume = pie * r * r * in1;
// volume = pie * radius * radius * in1;
  volume_water= total_volume - volume;
  
   Serial.println("Volume : ");
  // Serial.print(volume_water);
Serial.println(volume_water);

Serial.println();
  }
 // send the value of analog input 0:
 // Serial.println(analogRead(A0));
  }
  delay(21);
delay(1000); //Delay of 1 second for ease of viewing 

    Serial.println("Volume : ");
  Serial.print(volume_water);
  Serial.println();
  }
  else
  {
  Serial.println("Bottle not placed erect or Lid removed for drinking");
  }
  delay(200);
  
 
  
  // read input value
//  Serial.print("Push button val =");
//Serial.print("\t");
//  Serial.println(val);
//  if (val == HIGH) {         // check if the input is HIGH (button released)
     lcd.clear();
        lcd.setCursor(0,0);           //sets the cursor at row 0 column 0

  // lcd.print(in1); 
  lcd.print(volume_water);
    lcd.setCursor(8,0);
     // lcd.print("cm"); 
 lcd.print("ml");
  

   
  
Serial.println();

  }
  
//}