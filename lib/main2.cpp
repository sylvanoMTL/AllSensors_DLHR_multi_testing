#include <Arduino.h>
#include <Wire.h>
#include <AllSensors_DLHR.h>

#define TCAADDR 0x70

AllSensors_DLHR_F05D_8 gagePressure(&Wire);

void tcaselect(uint8_t i);

void setup() {
    Wire.begin();
    Serial.begin(115200);
    gagePressure.setPressureUnit(AllSensors_DLHR::PressureUnit::PASCAL);
}

void loop() {
     for (uint8_t t=0; t<8; t++){
      tcaselect(t); 
      Serial.print("TCA Port #"); Serial.print(t);
      for (uint8_t addr = 0; addr<=127; addr++) {
        if (addr == TCAADDR) continue;
        Wire.beginTransmission(addr);
        if (!Wire.endTransmission()) {
          Serial.print("Found I2C 0x");  Serial.println(addr,HEX); Serial.print("\t");
        }
      }
      Serial.println("");
    }

  delay(1000);


  Serial.println("\n\nTrying Measurement\n\n");


  tcaselect(1);
    gagePressure.startMeasurement();
    gagePressure.readData(true);

  Serial.print("Pressure: ");
  Serial.print(gagePressure.pressure);
  Serial.print(" Temperature: ");
  Serial.println(gagePressure.temperature);
  Serial.print("\n\n");
  delay(100);
}

void tcaselect(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}