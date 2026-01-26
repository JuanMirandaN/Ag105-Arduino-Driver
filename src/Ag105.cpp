#include <Arduino.h>
#include <Wire.h>
#include <Ag105.h>


Ag105::Ag105(uint8_t Ag105_Address_, TwoWire* i2c_port_, Stream* debug_port_)
    :Ag105_Address(Ag105_Address_),
     i2c_port(i2c_port_),
     debug_port(debug_port_){

}


// put function definitions here:


uint8_t Ag105::setChargeCurrent(float current_value_mA){
    uint8_t I2C_value;

    if(current_value_mA > 2500 || current_value_mA < 100){
        if(debug_port){
            (*debug_port).print("Error, input current is off limits\n");
        }

        return 1;
    }else{
        if(current_value_mA >= 250 && current_value_mA <= 2000.1){
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


uint8_t Ag105::setBatteryVoltage(float battery_voltage){
    uint8_t I2C_value;
    if(battery_voltage >= 3.89 && battery_voltage <= 4.21){
        I2C_value = round((battery_voltage - 3.9)/0.1) + 1;
    }else if(battery_voltage >= 7.79 && battery_voltage <= 8.49){
        I2C_value = round((battery_voltage - 7.8)/0.2) + 5;
    }else if(battery_voltage >= 11.69 && battery_voltage <= 12.69){
        I2C_value = round((battery_voltage - 11.7)/0.3) + 9;
    }else if(battery_voltage < 0){
        I2C_value = 0;//External resistor config
    }else{
        if(debug_port){
            debug_port -> println("Error: The battery voltage is not valid.");
            return 1;
        }
    }

    
    i2c_port -> beginTransmission(Ag105_Address);
    i2c_port -> write(BATTERY_VOLTAGE_SETTING);
    i2c_port -> write(I2C_value);

    uint8_t error = i2c_port -> endTransmission();
    if (error != 0){

        if(debug_port){
            debug_port -> print("Error has ocurred in the transmission\n");
        }

        return 1;    
    }

    return 0;
}


uint8_t Ag105::setMPPTVoltage(float MPPT_Voltage){
    uint8_t I2C_value = round((MPPT_Voltage - 11.0)/0.088);
    if(I2C_value > 250){
        I2C_value = 250;
    }else if(I2C_value < 0){
        I2C_value = 0;
    }

    if(MPPT_Voltage == -1){
        I2C_value = 251;
    }
    
    i2c_port -> beginTransmission(Ag105_Address);
    i2c_port -> write(MPPT_VOLTAGE);
    i2c_port -> write(I2C_value);

    uint8_t error = i2c_port -> endTransmission();
    if (error != 0){

        if(debug_port){
            debug_port -> print("Error has ocurred in the transmission\n");
        }

        return 1;    
    }

    return 0;

}

uint8_t Ag105::setTimeout(float Timeout){
    
    if(Timeout > 762.0){
        Timeout = 762;
    }else if(Timeout < 0.0){
        Timeout = 0.0;
    }
    
    uint8_t I2C_value = round((Timeout)/3.0);

    i2c_port -> beginTransmission(Ag105_Address);
    i2c_port -> write(TIMEOUT);
    i2c_port -> write(I2C_value);

    uint8_t error = i2c_port -> endTransmission();
    if (error != 0){

        if(debug_port){
            debug_port -> print("Error has ocurred in the transmission\n");
        }

        return 1;    
    }

    return 0;

}


uint8_t Ag105::setRecoveryTime(float RecoveryTime){
    
    if(RecoveryTime > 762.0){
        RecoveryTime = 762;
    }else if(RecoveryTime < 0.0){
        RecoveryTime = 0.0;
    }
    
    uint8_t I2C_value = round((RecoveryTime)/3.0);

    i2c_port -> beginTransmission(Ag105_Address);
    i2c_port -> write(RECOVERY_TIME);
    i2c_port -> write(I2C_value);

    uint8_t error = i2c_port -> endTransmission();
    if (error != 0){

        if(debug_port){
            debug_port -> print("Error has ocurred in the transmission\n");
        }

        return 1;    
    }

    return 0;

}




float Ag105::getChargeCurrent(){
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
        }else if(I2C_value == 0){
            current_value_mA = -333.333;//external resistor mode
        }else{
            current_value_mA = 2500.0; 
        }

        return current_value_mA;
    }
}


float Ag105::getBatteryVoltage(){
    uint8_t I2C_value;
    i2c_port -> beginTransmission(Ag105_Address);
    i2c_port -> write(BATTERY_VOLTAGE_SETTING);
    i2c_port -> endTransmission();

    i2c_port -> requestFrom(Ag105_Address, 2);

    if(i2c_port -> available() != 2){
        if(debug_port){
            debug_port -> print("Error: There are less than 2 bytes available for reading");
        }
        return -1.0;

    }else{
        i2c_port -> read();
        I2C_value = i2c_port -> read();
        
        float Battery_Voltage = -1.0;

        if(I2C_value >=1 && I2C_value <=4){
            Battery_Voltage = 3.8 + I2C_value * 0.1;
        }else if(I2C_value >=5 && I2C_value <= 8){
            Battery_Voltage = 7.8 + (I2C_value - 5) * 0.2;
        }else if(I2C_value >=9 && I2C_value <= 12){
            Battery_Voltage = 11.7 + (I2C_value - 9) * 0.3;
        }else if(I2C_value == 0){
            Battery_Voltage = -333.333;//external resistor mode
        }

        return Battery_Voltage;
    }

}


float Ag105::getMPPTVoltage(){
    uint8_t Status;
    uint8_t I2C_value;

    i2c_port -> beginTransmission(Ag105_Address);
    i2c_port -> write(MPPT_VOLTAGE);

    i2c_port -> endTransmission();

    i2c_port -> requestFrom(Ag105_Address, 2);



    if(i2c_port -> available() != 2){
        if(debug_port){
            debug_port -> print("Error: There are less than 2 bytes available for reading");
        }
        return -1.0;

    }else{
        Status = i2c_port -> read();
        I2C_value = i2c_port -> read();

        float MPPT_Voltage;

        if(I2C_value >=251){
            MPPT_Voltage = -333.333; //external resistor config
        }else{
            MPPT_Voltage = I2C_value*0.088 + 11;
        }
        
        return MPPT_Voltage;
    }
}


int16_t Ag105::getTimeout(){
    uint8_t I2C_value;
    int16_t Timeout;

    i2c_port -> beginTransmission(Ag105_Address);
    i2c_port -> write(TIMEOUT);
    i2c_port -> endTransmission();

    if((i2c_port -> requestFrom(Ag105_Address, 2)) != 2){
       
        if(debug_port){
            debug_port -> print("Error: There are less than 2 bytes available for reading");
        }
        return -1;

    }

    i2c_port -> read();//Status is ignored

    I2C_value = i2c_port -> read();

    Timeout = (int16_t)I2C_value * 3;
    return Timeout;
}



int16_t Ag105::getRecoveryTime(){
    uint8_t I2C_value;
    int16_t RecoveryTime;

    i2c_port -> beginTransmission(Ag105_Address);
    i2c_port -> write(RECOVERY_TIME);
    i2c_port -> endTransmission();

    if((i2c_port -> requestFrom(Ag105_Address, 2)) != 2){
       
        if(debug_port){
            debug_port -> print("Error: There are less than 2 bytes available for reading");
        }
        return -1;

    }

    i2c_port -> read();//Status is ignored

    I2C_value = i2c_port -> read();

    RecoveryTime = (int16_t)I2C_value * 3;
    return RecoveryTime;
}