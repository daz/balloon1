#include <Arduino.h>    //assuming inclusion of string.h
#include "Module.h"

//Checks whether balloon is descending. Aim to call every 3-5 cycles

class CheckLanded: public Module { 
        HardwareSerial *serial;
        IMU *theIMU;
        GPS *theGPS;
        static const int SAVED_VALUES=3;    //number of values saved in array
        int altitude[SAVED_VALUES]={0};    //most recent stored in altitude[0]
        int pressure[SAVED_VALUES]={0};     //for compiling
        bool deltaAltitude=false;
        bool deltaPressure=false;
        unsigned long timer;
        static const int CYCLES=3;      //run every x CYCLES       
        bool landed=false;   //RETURN value for function
        static const int PRESSURE_VARIES=0; //at rest, pressure probably fluctuates, depends on sensors
        static const int ALTITUDE_VARIES=0;
    
public:
        CheckLanded(HardwareSerial*,IMU*,GPS*);
        void begin();
        void tick();
        int enable();
        void disable();
        const char* dataToPersist();
        bool isLanded();
};
