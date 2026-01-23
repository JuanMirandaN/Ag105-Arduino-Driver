#include <Arduino.h>
#include <Wire.h>
#include <Ag105.h>


Ag105 :: Ag105(uint8_t Ag105_Address_, TwoWire* i2c_port_, Stream* debug_port_)
    :Ag105_Address(Ag105_Address_),
     i2c_port(i2c_port_),
     debug_port(debug_port_){

}


// put function definitions here:


uint8_t Ag105 :: SetChargeCurrent(float current_value_mA){
    uint8_t I2C_value;

    if(current_value_mA > 2500 || current_value_mA < 100){
        if(debug_port){
            (*debug_port).print("Error, input current is off limits\n");
        }

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

    (*i2c_port).beginTransmission(Ag105_Address);
    (*i2c_port).write(CHARGE_CURRENT_SETTING);
    (*i2c_port).write(I2C_value);

    uint8_t error = (*i2c_port).endTransmission();
    if (error != 0){

        if(debug_port){
            (*debug_port).print("Error has ocurred in the transmission\n");
        }

        return 1;    
    }
    return 0;

}

float Ag105 :: ReadChargeCurrent(){
    uint8_t Status;
    uint8_t I2C_value;
    float current_value_mA;

    (*i2c_port).beginTransmission(Ag105_Address);
    
    (*i2c_port).write(CHARGE_CURRENT_SETTING);
    
    (*i2c_port).endTransmission();//STOP
    (*i2c_port).requestFrom(Ag105_Address, 2); //START ON READ MODE request 2 bytes STATUS + Current value 

    if((*i2c_port).available() != 2){
        if(debug_port){
            (*debug_port).print("Error: There are less than 2 bytes available for reading");
        }

        return -1.0;
    }else{
        Status = (*i2c_port).read();
        I2C_value = (*i2c_port).read();

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
