#include <Arduino.h>
#include <Wire.h>
#include <AllSensors_DLHR.h>

#include <TI_TCA9548A.h>
#define TCAADDR 0x70


//#define SINGLE_SENSOR


TI_TCA9548A tca9548a(&Wire);

#ifdef SINGLE_SENSOR
  AllSensors_DLHR_F05D_8 gagePressure(&Wire);
#else
  AllSensors_DLHR_F05D_8 gagePressureArray[8] =  AllSensors_DLHR_F05D_8(&Wire);  
  bool condition[8]={0};
  float pressureArray[8]={0}; 

#endif

void setup() {
    Wire.begin();
    Serial.begin(115200);
    
    #ifdef SINGLE_SENSOR
      gagePressure.setPressureUnit(AllSensors_DLHR::PressureUnit::PASCAL);
    #else
      for(byte i = 0;i<8;i++){
      gagePressureArray[i].setPressureUnit(AllSensors_DLHR::PressureUnit::PASCAL);
      }                                      
    #endif
}



void loop(){
    #ifdef SINGLE_SENSOR
      tca9548a.selectChannel(1);
      if(gagePressure.readDataAsynchro())
      {
            Serial.print("Pressure: ");
            Serial.print(gagePressure.pressure);
            Serial.print(" Temperature: ");
            Serial.println(gagePressure.temperature);
          }
    #else
      for(byte i = 0;i<8;i++){
        tca9548a.selectChannel(i);
        condition[i] = gagePressureArray[i].readDataAsynchro();
        if(condition[i]){
            pressureArray[i] = gagePressureArray[i].pressure;
        }
      }

      for(byte i = 0;i<8;i++){
        Serial.print(pressureArray[i]);Serial.print("\t");
      }
      Serial.println();


    #endif
}



