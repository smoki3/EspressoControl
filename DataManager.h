/*
 * DataManager.h
 *
 *  Created on: Aug 20, 2020
 *      Author: tsugua
 */

#ifndef DATAMANAGER_H_
#define DATAMANAGER_H_

#include "Arduino.h"
#include "DeviceControl.h"

#define DEFAULT_TEMP_BOILER		110
#define	DEFAULT_TEMP_BU			94
#define	MAX_TEMP_BOILER			120
#define	MAX_TEMP_BU				110
#define	MIN_TEMP_BOILER			10
#define	MIN_TEMP_BU				10

#define	DEFAULT_FILL_BOILER_OVER_SONDE_TIME		5000
#define	DEFAULT_BOILER_FILL_MAX_TIME			120000
#define MAX_FILL_BOILER_OVER_SONDE_TIME
#define	MIN_FILL_BOILER_OVER_SONDE_TIME
#define MAX_BOILER_FILL_MAX_TIME
#define	MIN_BOILER_FILL_MAX_TIME

#define	DEFAULT_DISTRIBUTION_VOLUME				30
#define	MAX_DISTRIBUTION_VOLUME					200
#define	MIN_DISTRIBUTION_VOLUME					1
#define DEFAULT_VOLUME_OFFSET					5
#define	MAX_VOLUME_OFFSET						100
#define	MIN_VOLUME_OFFSET						0

#define	DEFAULT_BLYNK_ENABLED					true

#define	DEFAULT_BOILER_CONTROLLER_P				1
#define	MAX_BOILER_CONTROLLER_P					100
#define	MIN_BOILER_CONTROLLER_P					0.01
#define	DEFAULT_BU_CONTROLLER_P					1
#define	MAX_BU_CONTROLLER_P						100
#define	MIN_BU_CONTROLLER_P						0.01

#define	IDLE_BLYNK_MIN_TEMP_UPDATE_INTERVAL		3000
#define	BREWING_BLYNK_MIN_TEMP_UPDATE_INTERVAL	500


class DataManager {
public:
	static void pushTempBoiler(double temp);
	static void pushTempBU(double temp);
	static void pushTempTube(double temp);

	static double getTargetTempBoiler();
	static double getTargetTempBU();
	static void setTargetTempBoiler(double temp, bool updateBlynk);
	static void setTargetTempBU(double temp, bool updateBlynk);

	static long getFillBoilerOverSondeTime();
	static long getBoilerMaxFillTime();
	static double getDistributionVolume();
	static void setDistributionVolume(double volume, bool updateBlynk);
	static double getVolumeOffset();
	static void setVolumeOffset(double offset, bool updateBlynk);
	static double getBoilerControllerP();
	static void setBoilerControllerP(double p, bool updateBlynk);
	static double getBUControllerP();
	static void setBUControllerP(double p, bool updateBlynk);

	static bool getBlynkEnabled();
	static void setBlynkEnabled(bool enabled);

	static void init();
	static void update();
private:
	DataManager(){}
	virtual ~DataManager(){}

	static DeviceControl *dev;

	static double targetTempBoiler;
	static double targetTempBU;

	static long fillBoilerOverSondeTime;
	static long boilerMaxFillTime;

	static double distributionVolume;
	static double volumeOffset;

	static bool blynkEnabled;

	static double boilerControllerP;
	static double BUControllerP;

	static uint32_t calculateWIFIChecksum();
	static void eepromWrite(uint8_t *src, int addr, int len, bool commit);
	static void eepromRead(uint8_t *dst, int addr, int len);
};
#endif /* DATAMANAGER_H_ */
