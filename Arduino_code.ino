
/* Πτυχιακή Εργασία
 * Τσιανόγλου Ανδριάννα
 * Ανάλυση και κατασκευή συστήματος γαντιού με αισθητήρα για μετάφραση κινήσεων σε νοηματική γλώσσα

Η παρακάτω εργασία διαβάζει και επεξεργάζεται τιμές που προέρχονται από αισθητήρες , τις μετατρέπει σε σύμβολα της Ελληνικής Νοηματικής Γλώσσας και τις εμφανίζει μέσω 
bluetooth στην οθόνη κινητού.

/*Thesis project
 *Tsianoglou Andrianna
 *Analysis and construction of a glove-sensors system for the translation of movements into sign language.
 
THIS PROJECT READS SENSOR VALUE AND TRY TO UNDERSTAND SYMBOLS GENERATED FROM GREEK SIGN LANGUAGE (GSL) CHART
AND DISPLAY ON A SMARTPHONE.

-------PIN CONFIGURATION----------------
A0,A1,A2,A3,A6 : FLEX SENSORS
D4&D5 : FOR BLUETOOTH RX AND TX
SCL&SDA (A4,A5) : XPIN AND YPIN FOR ACCELEROMETER
D2    :FOR INT
*/



//Importing Libraries for the proper operation of the program
//The I2Cdev.h library must be installed for the MPU 6050 to be working.


#include <SoftwareSerial.h>
#include <Wire.h>
#include <MPU6050.h>
#include "I2Cdev.h"



SoftwareSerial Bluetooth (11,10); // οι RX | TX  είσοδοι του Arduino Nano//


//variable initializtion for MPU-6050 //



const int MPU_addr=0x69;   // I2C address of MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ; // Data of MPU-6050
int minVal=265; // Declaring the smallest value the accelerometer will have
int maxVal=402; // Declaring the largest value the accelerometer will have

double x;  //Doubling the variable of  x
double y; //Doubling the variable of y
double z; //Doubling the variable of z

//variable initializtion for sensors

const int FLEX_PIN1 = A0; //Thumb - Αντίχειρας
const int sensorMin1 = 0;  //Declaring the minimun value of the analog input
const int sensorMax1 = 1023; //Declaring the highest value of the analog input

const int FLEX_PIN2 = A1; //Index - Δείκτης
const int sensorMin2 = 0; 
const int sensorMax2 = 1023;

const int FLEX_PIN3 = A2; //Middle - Μέσος
const int sensorMin3 = 0; 
const int sensorMax3 = 1023;

const int FLEX_PIN4 = A3; //Ring - Παράμεσος
const int sensorMin4 = 0; 
const int sensorMax4 = 1023;

const int FLEX_PIN5 = A6; //Pinky - Μικρός
const int sensorMin5 = 0; 
const int sensorMax5 = 1023;
  

//The following functions are executed once during microcontroller startup
void setup(void){

//Checking if Bluetooth communication is in control//
 
  
  Bluetooth.begin(9600);
  Bluetooth.println("THESIS PROJECT TSIANOGLOY ANDRIANNA");
  
  while (!Serial) 
  {
  ; //waiting the serial port to communicate with Bluetooth//
  }

  
  delay(2000);







//communication with Gyroscope//
  
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // σετάρει στο 0 (ενεργοποιεί τον MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);

}

void loop(void){


 String letter; {  //We declare the sequence of characters
   

 if (Bluetooth.available()) 
  
  {
    Serial.write(Bluetooth.read());
  }
  else if (Serial.available()) {
     Serial.write(Serial.read());
     Bluetooth.write(Serial.read());
  }
}






//Gyroscope status measurement//

  
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)


// Through the map function, we change the range of analog values that we recieve from MPU-6050 and after converting them into digital ones, we set the range of values from -90 to 90
  int xAng = map(AcX,minVal,maxVal,-90,90); 
  int yAng = map(AcY,minVal,maxVal,-90,90); 
  int zAng = map(AcZ,minVal,maxVal,-90,90);

//Alternating rads to degrees
  x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI); 
  y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI); 
  z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);





//While running the program we can see the MPU 6050 values on the Arduino program screen only//

  Serial.print("AngleX= "); 
  Serial.println(x);

  Serial.print("AngleY= "); 
  Serial.println(y);

  Serial.print("AngleZ= "); 
  Serial.println(z); 
  Serial.println("-----------------------------------------"); 
  delay(400); 

  
  //Test program I ran at the beginning to check the values of the MPU6050
  //Serial.print("AcX = "); Serial.print(AcX);
  //Serial.print(" | AcY = "); Serial.print(AcY);
  //Serial.print(" | AcZ = "); Serial.print(AcZ);
  //Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet
  //Serial.print(" | GyX = "); Serial.print(GyX);
  //Serial.print(" | GyY = "); Serial.print(GyY);
  //Serial.print(" | GyZ = "); Serial.println(GyZ);
  delay(333);


// Reading sensors value //

 int thumb = analogRead(FLEX_PIN1);
 int index = analogRead(FLEX_PIN2);
 int middle = analogRead(FLEX_PIN3);
 int ring = analogRead(FLEX_PIN4);
 int pinky = analogRead(FLEX_PIN5);

 // Analog pins change values from 0V-5V to values 0-1023.//
 //The values must be converted to digital voltage values. //

//With the constrain function we limit the values we will get for each finger flex sensor to minimum and maximum values, which is 0-1023.

  thumb = constrain(thumb, sensorMin1, sensorMax1);
  index = constrain(index, sensorMin2, sensorMax2);
  middle = constrain(middle, sensorMin3, sensorMax3);
  ring = constrain(ring, sensorMin4, sensorMax4);
  pinky = constrain(pinky, sensorMin5, sensorMax5);
  
  // Through the map function, we change the range of analog values that we will get from the flex sensros and after the values are converted to digital, we set the range of values from 0 to 100. The float allows the values to be seen as data types.

  float angle1= map(thumb, sensorMin1, sensorMax1, 0, 100);
  float angle2= map(index, sensorMin2, sensorMax2, 0, 100);
  float angle3= map(middle, sensorMin3, sensorMax3, 0, 100);
  float angle4= map(ring, sensorMin4, sensorMax4, 0, 100);
  float angle5= map(pinky, sensorMin5, sensorMax5, 0, 100); 

//During the execution of the program we can see the values of the flex sensors on the screen of the Arduino program only.
  
  Serial.println(angle1);
  Serial.println(angle2);
  Serial.println(angle3);
  Serial.println(angle4);
  Serial.println(angle5);
  Serial.println(GyX);
  Serial.println(GyY);
  Serial.println(","); 
//The main conditions of the values we get from all the connected sensors of the glove, based on them we get the desired result (letter) on the mobile screen via BLUETOOTH.

  if 
    (((angle1>=83)&&(angle1<=85))&&((angle2>=84)&&(angle2<=87))&&((angle3>=83)&&(angle3<=86))&&((angle4>=78)&&(angle4<=80))&&((angle5>=89)&&(angle5<=93)))
    letter = ("A");
   
    else if
    
      (((angle1>=86)&&(angle1<=89))&&((angle2>=79)&&(angle2<=81))&&((angle3>=78)&&(angle3<=81))&&((angle4>=74)&&(angle4<=76))&&((angle5>=83)&&(angle5<=86)))
    letter = ("B");

    else if
    
      (((angle1>=83)&&(angle1<=87))&&((angle2>=79)&&(angle2<=82))&&((angle3>=87)&&(angle3<=90))&&((angle4>=79)&&(angle4<=81))&&((angle5>=89)&&(angle5<=92)))
    letter = ("Γ");
    
    else if
    
      (((angle1>=86)&&(angle1<=89))&&((angle2>=78)&&(angle2<=82))&&((angle3>=83)&&(angle3<=85))&&((angle4>=78)&&(angle4<=81))&&((angle5>=89)&&(angle5<=92)))
    letter = ("Δ");
    
    else if
    
      (((angle1>=87)&&(angle1<=89))&&((angle2>=85)&&(angle2<=88))&&((angle3>=85)&&(angle3<=87))&&((angle4>=77)&&(angle4<=79))&&((angle5>=87)&&(angle5<=91)))
    letter = ("E");

    else if
    
      (((angle1>=87)&&(angle1<=90))&&((angle2>=79)&&(angle2<=82))&&((angle3>=87)&&(angle3<=90))&&((angle4>=79)&&(angle4<=81))&&((angle5>=83)&&(angle5<=86))&&((x>=100)&&(x<=200)))
    letter = ("Ζ");

    else if
    
      (((angle1>=85)&&(angle1<=87))&&((angle2>=79)&&(angle2<=81))&&((angle3>=85)&&(angle3<=87))&&((angle4>=78)&&(angle4<=80))&&((angle5>=82)&&(angle5<=84))&&((x>=50)&&(x<=100)))
    letter = ("Η");
        
    else if
    
      (((angle1>=86)&&(angle1<=88))&&((angle2>=78)&&(angle2<=81))&&((angle3>=78)&&(angle3<=81))&&((angle4>=80)&&(angle4<=82))&&((angle5>=88)&&(angle5<=92))&&((x>=100)&&(x<=200)))
    letter = ("Θ");
    
    else if
    
      (((angle1>=88)&&(angle1<=90))&&((angle2>=87)&&(angle2<=89))&&((angle3>=85)&&(angle3<=87))&&((angle4>=79)&&(angle4<=81))&&((angle5>=82)&&(angle5<=85)))
    letter = ("Ι");

    else if
    
      (((angle1>=84)&&(angle1<=86))&&((angle2>=79)&&(angle2<=81))&&((angle3>=79)&&(angle3<=81))&&((angle4>=78)&&(angle4<=80))&&((angle5>=89)&&(angle5<=91)))
    letter = ("Κ");

    else if
    
      (((angle1>=86)&&(angle1<=88))&&((angle2>=78)&&(angle2<=81))&&((angle3>=77)&&(angle3<=80))&&((angle4>=78)&&(angle4<=81))&&((angle5>=88)&&(angle5<=91))&&((x>=250)&&(x<=350)))
    letter = ("Λ");
    
    
    else if
    
      (((angle1>=86)&&(angle1<=88))&&((angle2>=83)&&(angle2<=86))&&((angle3>=83)&&(angle3<=87))&&((angle4>=75)&&(angle4<=77))&&((angle5>=88)&&(angle5<=92)))
    letter = ("Μ");
    
    else if
    
      (((angle1>=86)&&(angle1<=89))&&((angle2>=83)&&(angle2<=85))&&((angle3>=82)&&(angle3<=84))&&((angle4>=79)&&(angle4<=80))&&((angle5>=89)&&(angle5<=92)))
    letter = ("Ν");

    else if
    
      (((angle1>=85)&&(angle1<=88))&&((angle2>=79)&&(angle2<=81))&&((angle3>=78)&&(angle3<=81))&&((angle4>=74)&&(angle4<=76))&&((angle5>=89)&&(angle5<=92)))
    letter = ("Ξ");
    
    else if
    
      (((angle1>=86)&&(angle1<=88))&&((angle2>=84)&&(angle2<=88))&&((angle3>=83)&&(angle3<=86))&&((angle4>=77)&&(angle4<=79))&&((angle5>=85)&&(angle5<=87))&&((y>=200)&&(y<=320)))
    letter = ("Ο");
    
    else if
    
      (((angle1>=84)&&(angle1<=86))&&((angle2>=79)&&(angle2<=81))&&((angle3>=87)&&(angle3<=89))&&((angle4>=78)&&(angle4<=81))&&((angle5>=82)&&(angle5<=86)))
    letter = ("Π");
    
    else if
    
      (((angle1>=85)&&(angle1<=87))&&((angle2>=78)&&(angle2<=80))&&((angle3>=78)&&(angle3<=80))&&((angle4>=80)&&(angle4<=82))&&((angle5>=88)&&(angle5<=91))&&((x>=50)&&(x<=150)))
    letter = ("Ρ");
    
    else if
    
(((angle1>=88)&&(angle1<=91))&&((angle2>=87)&&(angle2<=90))&&((angle3>=86)&&(angle3<=89))&&((angle4>=79)&&(angle4<=82))&&((angle5>=88)&&(angle5<=91))&&((x>=50)&&(x<=150)))
    letter = ("Σ");

    else if
    
      (((angle1>=86)&&(angle1<=88))&&((angle2>=79)&&(angle2<=81))&&((angle3>=87)&&(angle3<=89))&&((angle4>=78)&&(angle4<=80))&&((angle5>=87)&&(angle5<=90)))
    letter = ("Τ");
    
    else if
    
      (((angle1>=85)&&(angle1<=87))&&((angle2>=86)&&(angle2<=88))&&((angle3>=85)&&(angle3<=87))&&((angle4>=77)&&(angle4<=79))&&((angle5>=82)&&(angle5<=85)))
    letter = ("Υ");
    
    else if
    
      (((angle1>=84)&&(angle1<=86))&&((angle2>=82)&&(angle2<=84))&&((angle3>=78)&&(angle3<=80))&&((angle4>=74)&&(angle4<=76))&&((angle5>=83)&&(angle5<=85)))
    letter = ("Φ");
    
    else if
    
      (((angle1>=86)&&(angle1<=88))&&((angle2>=84)&&(angle2<=86))&&((angle3>=84)&&(angle3<=86))&&((angle4>=79)&&(angle4<=81))&&((angle5>=89)&&(angle5<=91)))
    letter = ("Χ");
    
    else if
    
      (((angle1>=0)&&(angle1<=0))&&((angle2>=0)&&(angle2<=0))&&((angle3>=0)&&(angle3<=0))&&((angle4>=0)&&(angle4<=0))&&((angle5>=0)&&(angle5<=0))&&((x>=50)&&(x<=250)))
    letter = ("Ψ");
    
    else if
    
      (((angle1>=84)&&(angle1<=86))&&((angle2>=83)&&(angle2<=85))&&((angle3>=81)&&(angle3<=83))&&((angle4>=75)&&(angle4<=77))&&((angle5>=86)&&(angle5<=88)))
    letter = ("Ω");
    
  

//Display the desired letter on the Arduino screen


  Serial.println(letter);
  Serial.println(",");
  
  //Through Bluetooth communication, the desired letter is displayed on the mobile screen

  Bluetooth.println(letter);
  Bluetooth.println(",");
  
  //Finds the loop number of values
  
  uint8_t sendbuffer[20];
  letter.getBytes(sendbuffer, 20);
  char lettersize = min(20, letter.length());
  Serial.write(sendbuffer, lettersize);
 delay(4000);
}
  
  // Now we see which letter is formed //
