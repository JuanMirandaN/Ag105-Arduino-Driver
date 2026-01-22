#include <Arduino.h>
#include <Wire.h>
#include <Ag105.h>


// put function declarations here:
void SetChargeCurrent();
float ReadChargeCurrent();

//The Ag105 can be addressed using the addresses set in field 0xE5, by default this is set to 0x30
uint8_t Ag105_Address = 0x30;

void setup() {
  // put your setup code here, to run once:

  Wire.begin(); //Init I2C as Master device
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:


}



Ag105::Ag105(uint8_t Ag105_Address_ = 0x30, TwoWire* i2c_port= &Wire){

}




// put function definitions here:

uint8_t SetChargeCurrent(float current_value_mA){
    uint8_t I2C_value;

    if(current_value_mA > 2500 || current_value_mA < 100){
        Serial.print("Error, input current is off limits\n");
        return 1;
    }else{
        if(current_value_mA >= 250 && current_value_mA <= 2000){
            I2C_value = (uint8_t) round(((2500.0 - current_value_mA)/250.0));
        }else if(current_value_mA < 250){
            I2C_value = (uint8_t) round(((700.0 - current_value_mA)/50.0));
        }else if(current_value_mA < 2250){
            I2C_value = 2;//2000mA set     
        }else{
            I2C_value = 1;//2500mA set 
        }

    }

    Wire.beginTransmission(Ag105_Address);
    Wire.write(CHARGE_CURRENT_SETTING);
    Wire.write(I2C_value);

    uint8_t error = Wire.endTransmission();
    if (error != 0){
        Serial.print("Error has ocurred in the transmission\n");
        return 1;    
    }
    return 0;

}

float ReadChargeCurrent(){
    uint8_t Status;
    uint8_t I2C_value;
    float current_value_mA;

    Wire.beginTransmission(Ag105_Address);
    
    Wire.write(CHARGE_CURRENT_SETTING);
    
    Wire.endTransmission();//STOP
    Wire.requestFrom(Ag105_Address, 2); //START ON READ MODE request 2 bytes STATUS + Current value 

    if(Wire.available() != 2){
        Serial.print("Error: There are less than 2 bytes available for reading");
        return -1.0;
    }else{
        Status = Wire.read();
        I2C_value = Wire.read();

        if(I2C_value >= 2 && I2C_value <= 9){
            current_value_mA = (float) (2500.0 - I2C_value * 250.0);
        }else if(I2C_value > 9){
            current_value_mA = (float) (700.0 - I2C_value * 50.0);    
        }else{
            current_value_mA = 2500.0; 
        }

        return current_value_mA;
    }
}
