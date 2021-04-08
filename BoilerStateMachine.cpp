/*
 * BoilerStateMachine.cpp
 *
 *  Created on: May 30, 2020
 *      Author: tsugua
 */

#include "BoilerStateMachine.h"
#include "DataManager.h"

BoilerStateMachine::BoilerStateMachine() {
	state = disabled;
#ifdef BOILER_QUICKSTART
	quickStart = true;
#else
	quickStart = false;
#endif
#ifdef BOILER_SOFTSTART
	softStart = true;
#else
	softStart = false;
#endif

	dev = DeviceControl::instance();
	machStat = MachineStatusStateMachine::instance();
	pid_input = dev->getBoilerTemp();
	pid_output = 0;
	pid_setpoint = DataManager::getTargetTempBoiler();
	boilerPID = new PID(&pid_input, &pid_output, &pid_setpoint, DataManager::getBoilerControllerP(),
			DataManager::getBoilerControllerI(), DataManager::getBoilerControllerD(), DIRECT);
	boilerPID->SetOutputLimits(0, 100);
	boilerPID->SetMode(AUTOMATIC);
	softStartTime = 0;
}

BoilerStateMachine::~BoilerStateMachine() {
}

void BoilerStateMachine::update(){
	switch (state){
	case disabled:
		//set outputs
		dev->disableBoilerHeater();
		//check transitions
		if(dev->getBoilerFull() && !machStat->inStandbye() &&
				!dev->getBoilerFillSensorError() && !dev->getBoilerTempSensorError()){
			Serial.println("Boiler Heater an");
			state = enabled;
		}
		break;
	case enabled:
		pid_input = dev->getBoilerTemp();

		if(softStart){
			if(softStartTime == 0 && (DataManager::getTargetTempBoiler() - dev->getBoilerTemp()) < BOILER_SOFTSTART_OFFSET_TEMP){
				softStartTime = millis();
			}
			pid_setpoint = DataManager::getTargetTempBoiler() - BOILER_SOFTSTART_OFFSET_TEMP;
			if(millis() > softStartTime + BOILER_SOFTSTART_TIME){
				softStartTime = 0;
				softStart = false;
			}
		}
		else{
			pid_setpoint = DataManager::getTargetTempBoiler();
		}

		boilerPID->SetTunings(DataManager::getBoilerControllerP(), DataManager::getBoilerControllerI(),
				DataManager::getBoilerControllerD());
		boilerPID->Compute();

		//set outputs
		if(quickStart){
			dev->enableBoilerHeater(100);
			if(dev->getBoilerTemp() >= DataManager::getTargetTempBoiler()){
				quickStart = false;
			}
		}
		else{
			//Serial.println(dev->getBoilerTemp());
			dev->enableBoilerHeater(pid_output);
		}
		//check transitions
		if(!dev->getBoilerFull() || machStat->inStandbye() ||
				dev->getBoilerFillSensorError() || dev->getBoilerTempSensorError()){
		#ifdef QUICKSTART
			// reenable faststart after standbye
			if(machStat->inStandbye()){
				quickStart = true;
			}
		#endif
			Serial.println("Boiler Heater aus");
			state = disabled;
		}
		break;
	}

}

