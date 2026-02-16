#include <Arduino.h>
#include <Wire.h>
#include <Ag105.h>


// put function declarations here:
void WriteHandler(int byte_number);
void ReadHandler();

//The Ag105 can be addressed using the addresses set in field 0xE5, by default this is set to 0x30

void setup() {

    uint8_t Ag105_Address = 0x30;
    Wire.begin(Ag105_Address);
    
    Wire.onReceive(WriteHandler); //Write request
    Wire.onRequest(ReadHandler); //Read request

}




void loop() {
    
}

volatile uint8_t ChargeCurrent_I2C;
volatile uint8_t MPPTVoltage_I2C;

volatile uint8_t Selected_field;



void WriteHandler(int byte_number){
    Selected_field = Wire.read();
    if(byte_number == 1){
        return;
    }else{

        if(Selected_field == CHARGE_CURRENT_SETTING){
            ChargeCurrent_I2C = Wire.read();
        }else if (Selected_field == MPPT_VOLTAGE){
            MPPTVoltage_I2C = Wire.read();
        }
        
    }
}

void ReadHandler(){

    Wire.write(0xff);//false status

    if(Selected_field == CHARGE_CURRENT_SETTING){
        Wire.write(ChargeCurrent_I2C);
    }else if(Selected_field == MPPT_VOLTAGE){
        Wire.write(MPPTVoltage_I2C);
    }else{
        Wire.write(-1);
    }

}







