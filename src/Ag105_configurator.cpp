#include <Arduino.h>
#include <Wire.h>
#include <Ag105.h>


// put function declarations here:
//The Ag105 can be addressed using the addresses set in field 0xE5, by default this is set to 0x30

void setup() {

    Wire.begin();
    Serial.begin(9600);     
}

uint8_t Ag105_Address = 0x30;
Ag105 MPPT_Controller(Ag105_Address, &Wire, &Serial);

uint16_t charge_current_new = 1020;//1020;

float Battery_Voltage = 4.2;

void loop() {
    // put your main code here, to run repeatedly:
    MPPT_Controller.setChargeCurrent((float)charge_current_new);
    delay(1000);

    float read_charge_current = MPPT_Controller.getChargeCurrent();

    Serial.print("Read Charge current is:");
    Serial.println(read_charge_current);
    delay(1000);



    float newMPPTVoltage = 12;//17.43;

    MPPT_Controller.setMPPTVoltage(newMPPTVoltage);

    Serial.print("MPPT voltage set to:");
    Serial.println(newMPPTVoltage);
    delay(1000); 

    float read_MPPT_Voltage = MPPT_Controller.getMPPTVoltage();

    Serial.print("MPPT voltage is:");
    Serial.println(read_MPPT_Voltage);
    delay(1000);



    MPPT_Controller.setBatteryVoltage(Battery_Voltage);


    MPPT_Controller.ReportStatus();
    MPPT_Controller.ReportConfig();
    delay(5000);

    

}







