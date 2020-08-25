/*
 * BoilerStateMachine.h
 *
 *  Created on: May 30, 2020
 *      Author: tsugua
 */

#ifndef BOILERSTATEMACHINE_H_
#define BOILERSTATEMACHINE_H_

#include "DeviceControl.h"

class BoilerStateMachine {
	enum BoilerStates {enabled, disabled};

public:
	BoilerStateMachine();
	virtual ~BoilerStateMachine();
	void update();
private:
	BoilerStates state;
	DeviceControl *dev;
};

#endif /* BOILERSTATEMACHINE_H_ */