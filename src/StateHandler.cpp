#include "StateHandler.h"

/* To Do :  needs checkBattery code --Matilda
            needs at-rest variance values for pressure/altitude
            is state logic represented correctly?
            correct getters for gps and IMU
*/


StateHandler::StateHandler(IMU *_imu, GPS *_gps, Battery *_battery)
	: imu(_imu)
	, gps(_gps)
	, battery(_battery)
{
    balloonState=PRE_FLIGHT;   //start in preflight
    stateChanged=false;
}

void StateHandler::begin(){
	altitude[0]=gps->getAltitude();
	pressure[0]=imu->getPressure();
}

void StateHandler::tick(){
    stateChanged=false; 
    //stateChanged should be modified in checker functions

    if (balloonState==PRE_FLIGHT){
        balloonState=DURING_FLIGHT; //only execute once
        stateChanged=true;
    }

    else if (balloonState==DURING_FLIGHT){
        if (checkBattery())     balloonState=LOW_BATTERY;
        else if (checkDescent()) balloonState=DURING_DESCENT;
    }

    else if (balloonState==DURING_DESCENT){
        if (checkBattery())     balloonState=LOW_BATTERY;
        else if (checkLanded()) balloonState=LANDED;
    }

    else if (balloonState==LOW_BATTERY){
        if (checkLanded())      balloonState=LANDED;
    }

    else if (balloonState==LANDED){
        balloonState=SCREAMING;    //purpose of landed is to reduce frequency???
        stateChanged=true;
    }

    else if (balloonState==SCREAMING){ 
        //should stay here until it runs out of battery
    }
}

bool StateHandler::hasStateChanged() {
	return stateChanged;
}

SystemState StateHandler::getSystemState() {
	return balloonState;
}

// SD formatter adds moduleName and \n
// outputs in format <value1>, ... ,<value4>,<value5>
const char* StateHandler::dataToPersist(){
	return NULL;
}

const char* StateHandler::getModuleName() {
	return "StateHandler";
}

bool CheckState::checkDescent(){
    //update value arrays
    for (int i=SAVED_VALUES-1;i>=1;i--){
        altitude[i]=altitude[i-1];
        pressure[i]=pressure[i-1];
    }
    altitude[0]=gps->getAltitude();
    pressure[0]=imu->getPressure();

    //calculate deltas (assume true, check for contradiction)
    deltaPressure=true; 
    deltaAltitude=true;
    for (int i=SAVED_VALUES-1;i>=1;i--){
        if (pressure[i]>pressure[i-1])  deltaPressure=false;
        if (altitude[i]<altitude[i-1])  deltaAltitude=false;
    } 

    stateChanged= (deltaAltitude && deltaPressure);
   
    return stateChanged;
}

bool CheckState::checkLanded(){
    //update value arrays
    for (int i=SAVED_VALUES-1;i>=1;i--){
        altitude[i]=altitude[i-1];
        pressure[i]=pressure[i-1];
    }
    altitude[0]=gps->getAltitude();
    pressure[0]=imu->getPressure();

    //calculate deltas (assume true, check for contradiction)
    deltaPressure=true; 
    deltaAltitude=true; 
    for (int i=SAVED_VALUES-1;i>=1;i--){
        if ( abs(pressure[i]-pressure[i-1])<LANDED_PRESSURE_VARIES)     deltaPressure=false;
        if ( abs(altitude[i]-altitude[i-1])<LANDED_ALTIUDE_VARIES)      deltaAltitude=false;
    } //needs to be drift-resistant 
     

    stateChanged= (deltaAltitude && deltaPressure);
    
    return stateChanged;
}

bool CheckState::checkBattery(){
    //waiting for code, should modify stateChanged here
    return false;
}