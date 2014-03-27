//============================================================
#include <FreeSixIMU.h>
#include <BMP085.h>

#include <Wire.h>

//============================================================

//==== Arduino Params - the same as in "MArduinoConnector.cpp": ====

const char CharTypeModeReadiness = 'R';
const char CharTypeModeTelemetry = 'T';
const char CharStopSendingData = 'S';

const char CharWriteTelemetryModeOccasionally = 'o';

//= For Height and Speed TabChannel "H, S":
const int   IndexTabChannel_HS = 5;
const char* CharsCodeHeightSpeed = "hs2 ";

//= For Pitch and Roll TabChannel "Angles":
const int   IndexTabChannel_Angles = 6;
const char* CharsCodeQuaternion = "q4 ";

//= For Temperature and Pressure TabChannel "T, P":
const int   IndexTabChannel_TP = 8;
const char* CharsCodeTemperaturePressure = "tp2 ";

const char CharSeparator = ' ';

//====

boolean IsTypeModeReadiness = false;
boolean IsTypeModeTelemetry = false;

boolean IsToWriteOccasionalValues = true;//false;


// For the FreeSixIMU object:
float quaternion[4]; // quaternion

// For the BMP085 object:
int32_t Temperature;
int32_t Pressure;
int32_t Altitude;

float Speed;

// Set the FreeSixIMU object:
FreeSixIMU sixDOF = FreeSixIMU();

// Set the BMP085 object:
BMP085 bmp = BMP085();

//============================================================
void setup() { 
  Serial.begin(38400);
  Wire.begin();
 
//  randomSeed(12345678);
 
  delay(50);
  sixDOF.init();
  delay(50);
  
  delay(50);//1000);
  bmp.init();   
  delay(50);//5000);  
}
//------------------------------------------------------------
void loop() { 

  if(Serial.available() > 0) {
//==== Switch Modes: ====
    char charMode;
    if(Serial.readBytes(&charMode, 1) > 0) {
      
      if(charMode == CharWriteTelemetryModeOccasionally) {
        IsToWriteOccasionalValues = true;
      }
      
      else if(charMode == CharTypeModeReadiness) {
        IsTypeModeReadiness = true;
        IsTypeModeTelemetry = false;
      }
      else if(charMode == CharTypeModeTelemetry) {
        IsTypeModeReadiness = false;
        IsTypeModeTelemetry = true;
      }
      else if(charMode == CharStopSendingData) {
        IsTypeModeReadiness = false;
        IsTypeModeTelemetry = false;
      }
      
      else return;
    }
  }
//---------

//  sixDOF.getYawPitchRoll(angles3);
  sixDOF.getQ(quaternion); 

  bmp.getAltitude(&Altitude);
  GetSpeed(&Speed);

  bmp.getTemperature(&Temperature); 
  bmp.getPressure(&Pressure);


  if(IsTypeModeTelemetry) {
//====  Telemetry Mode:  ====
    
//===  Continuously Sent Data:  ===

//== First Continuously Sent Data:
//= Quaternion for Pitch and Roll TabChannel "Angles":
//  "6oq4 0.123 -4.567 8.910 11.121\n"  <<---  for "IsToWriteOccasionalValues == true"
//  "6q4 0.123 -4.567 8.910 11.121\n"   <<---  for "IsToWriteOccasionalValues == false"
    Serial.print(IndexTabChannel_Angles);                             // "6"
    if(IsToWriteOccasionalValues)
      Serial.print(CharWriteTelemetryModeOccasionally);               // "o"
    Serial.print(CharsCodeQuaternion);                                // "q4 "
    Serial.print(quaternion[0], 3);     Serial.print(CharSeparator);  // "0.123 "
    Serial.print(quaternion[1], 3);     Serial.print(CharSeparator);  // "-4.567 "
    Serial.print(quaternion[2], 3);     Serial.print(CharSeparator);  // "8.910 "
    Serial.println(quaternion[3], 3);                                 // "11.121\n"
    Serial.flush();
//    delay(5); 

/*
//== Second Continuously Sent Data:
    Serial.print(i);
    if(IsToWriteOccasionalValues)
      Serial.print(CharWriteTelemetryModeOccasionally);               // "o"
    Serial.print(Chars...);
    Serial.print(...);     Serial.print(CharSeparator);
    ...
    Serial.println(...);

//= Next Continuously Sent Data:
//  ...
*/

    if(IsToWriteOccasionalValues) {
//===  Occasionally Sent Data:  ===

//= Height and Speed for TabChannel "H, S":
//  "5ohs2 12 28.123\n"
      Serial.print(IndexTabChannel_HS);                       // "5"
      Serial.print(CharWriteTelemetryModeOccasionally);       // "o"
      Serial.print(CharsCodeHeightSpeed);                     // "hs2 "
      Serial.print(Altitude);  Serial.print(CharSeparator);   // "12 "
      Serial.println(Speed, 3);                               // "28.123\n"
      Serial.flush();
//      delay(5); 

//= Temperature and Pressure for TabChannel "T, P":
//  "8otp2 25 101850\n"
      Serial.print(IndexTabChannel_TP);                         // "8"
      Serial.print(CharWriteTelemetryModeOccasionally);         // "o"
      Serial.print(CharsCodeTemperaturePressure);               // "tp2 "
      Serial.print(Temperature);  Serial.print(CharSeparator);  // "25 " 
      Serial.println(Pressure);                                 // "101850\n"
      Serial.flush();
//      delay(5); 
      
      
//= Last Operand for "IsToWriteOccasionalValues == true":
      IsToWriteOccasionalValues = false;
    }
  }    
  
//---------
  else if(IsTypeModeReadiness) {
//==== Readiness Mode: ==
  }


//---------

//  Serial.flush();
  
  delay(5); 
}
//------------------------------------------------------------
void GetSpeed(float* pSpeed) 
{
// Calculation of Speed:
// ...
//  *pSpeed = ...;
// remove the next line:
*pSpeed = 10.0;
}
//============================================================

