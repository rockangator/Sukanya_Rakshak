#include <DFRobot_TFmini.h>

SoftwareSerial mySerial(12, 13); // RX, TX

DFRobot_TFmini  TFmini;
uint16_t distance,strength;

void setup(){
    Serial.begin(115200);
    TFmini.begin(mySerial);
}

void loop(){
    if(TFmini.measure()){                      //Measure Distance and get signal strength
        distance = TFmini.getDistance();       //Get distance data
        strength = TFmini.getStrength();       //Get signal strength data
        Serial.print("Distance = ");
        Serial.print(distance);
        Serial.println("cm");
        Serial.print("Strength = ");
        Serial.println(strength);
        delay(500);
    }
    delay(500);
}
