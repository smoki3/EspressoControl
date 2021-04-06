/*
 * BoilerStateMachine.h
 *
 *  Created on: May 30, 2020
 *      Author: tsugua
 */

#ifndef BOILERSTATEMACHINE_H_
#define BOILERSTATEMACHINE_H_

#include "DeviceControl.h"
#include "MachineStatusStateMachine.h"
#include <PID_v1.h>

//#define BOILER_QUICKSTART
#define BOILER_SOFTSTART
#define BOILER_SOFTSTART_OFFSET_TEMP	5
#define BOILER_SOFTSTART_TIME			5*60*1000


class BoilerStateMachine {
	enum BoilerStates {enabled, disabled};

public:
	BoilerStateMachine();
	virtual ~BoilerStateMachine();
	void update();
private:
	BoilerStates state;
	DeviceControl *dev;
	MachineStatusStateMachine *machStat;
	bool quickStart;
	bool softStart;
	double pid_input;
	long softStartTime;
	double pid_output;
	double pid_setpoint;
	PID *boilerPID;
};

#endif /* BOILERSTATEMACHINE_H_ */
