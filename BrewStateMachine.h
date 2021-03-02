/*
 * BrewStateMachine.h
 *
 *  Created on: May 30, 2020
 *      Author: tsugua
 */

#ifndef BREWSTATEMACHINE_H_
#define BREWSTATEMACHINE_H_

#include "DeviceControl.h"
#include "MachineStatusStateMachine.h"
#include "Arduino.h"

class BrewStateMachine {
	enum BrewStates {idle, dist_man, refill_boiler, dist_vol, dist_vol_finished, dist_tim, dist_tim_finished, error_tank_empty, error_probe,
		preinf_buildup, preinf_wait, cleaning_buildup, cleaning_flushing, cooling_flush };
public:
	BrewStateMachine();
	bool isBrewing();
	bool isPreinfusing();
	bool isCleaning();
	bool isCoolingFlush();
	int getCurrentCleaningCycle();
	virtual ~BrewStateMachine();
	void update();
private:
	BrewStates state;
	DeviceControl *dev;
	MachineStatusStateMachine *machStat;
	unsigned long cleaningBuildupTime;
	unsigned long cleaningFlushingTime;
	int numberCleaningCycles;
	int currentCleaningCycle;
};

#endif /* BREWSTATEMACHINE_H_ */
