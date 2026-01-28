#ifndef AG105_H
    #include <stdint.h>
    #define AG105_H

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
            TwoWire* i2c_port;
            Stream* debug_port;

        public:
            //Constructor
            Ag105(uint8_t Ag105_Address_ = 0x30, TwoWire* i2c_port_ = &Wire, Stream* debug_port_= nullptr);

            //setters
            uint8_t setChargeCurrent(float current_value_mA);
            uint8_t setBatteryVoltage(float battery_voltage);
            uint8_t setMPPTVoltage(float MPPT_Voltage);
            uint8_t setTimeout(float Timeout);
            uint8_t setRecoveryTime(float RecoveryTime);
            uint8_t setJEITAProfile(bool Enable);
            uint8_t setI2CAddress(uint8_t Address);

            
            uint8_t ResetMemory();


            //Getters
            float getChargeCurrent();
            float getBatteryVoltage();
            float getMPPTVoltage();
            int16_t getTimeout();
            int16_t getRecoveryTime();
            float getMeasuredBatteryVoltage();
            float getMeasuredChargeCurrent();
            float getMeasuredInputVoltage();
            int8_t getJEITAProfile();
            int16_t getMajorFirmwareVersion();
            int16_t getMinorFirmwareVersion();
            int16_t getI2CAddress();
            int16_t getStatusRegister();

            int8_t getGENSTAT();


            bool isMPPTActive();


            bool isPowerTrackingActive();

            bool inConstantVoltage();

            bool inConstantCurrent();

            bool isThermalLimiting();

            int16_t ReportStatus();


            
            
    };


#endif