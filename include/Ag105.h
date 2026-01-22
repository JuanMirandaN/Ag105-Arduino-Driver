#ifndef AG105_CLASS
    #define AG105_CLASS

    //Field IDs
    #define CHARGE_CURRENT_SETTING 0x00
    #define BATTERY_VOLTAGE_SETTING 0x01
    #define MPPT_VOLTAGE 0x02
    #define TIMEOUT 0x03
    #define RECOVERY_TIME 0x04
    #define MEASURED_BATTERY_VOLTAGE 0x05
    #define MEASURED_CHARGE_CURRENT 0x06
    #define MEASURED_INPUT_VOLTAGE 0x07
    #define EXTENDED_JEITA_PROFILE 0x0a
    #define MAJOR_FIRMWARE_VERSION 0xfd
    #define MINOR_FIRMWARE_VERSION 0xfe
    #define CHANGE_I2C_ADDRESS 0xe5
    #define RESET_MEMORY 0xe6
    //

    class Ag105{
        private:
            uint8_t Ag105_Address;

    }


#endif