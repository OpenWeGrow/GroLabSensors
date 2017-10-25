/*
* Copyright (C) 2017 OpenGrow GroLab by João Melo <joao.melo@opengrow.pt>
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include <Arduino.h>
#include "GroLabMoistureSensor.h"


/****************************************************************************/

#define SENSOR_TIMEOUT 2000 //in microseconds
#define MAX_MOIST_SAMPLE 5


//Arduino Physical pins
int mySamplePin;
int myPowerPin = 0;
unsigned char EEPROMAddress;


//Calibration Default parameters
parameters_Moisture calibParams = {900,200,0.143};

//Temp variables for data aquisition
uint32_t rawValue = 0;
unsigned char iSample = 0;

/*GroLab Moisture Sensor C++ Object initializer

	analogPin - The Analog pin that is sampling the sensor output.
    powerPin  - The Arduino Pin that is turning ON/OFF the sensor, if the sensor 
				is directly connected to +5V set to 0(zero), when initializing the object.
	EEPROMAddressPointer  - Initial Adress where the calibration data is saved, must be always low then MAX EPPROM - sizeOf(parameters_Moisture);
*/
OpenMoistureSensor::OpenMoistureSensor(int analogPin, int powerPin, unsigned char EEPROMAddressPointer)
{
	mySamplePin = analogPin;
	myPowerPin = powerPin;
	EEPROMAddress= EEPROMAddressPointer;
	pinMode(mySamplePin, INPUT);
	if(powerPin != 0)
		pinMode(myPowerPin, OUTPUT);
}

float OpenMoistureSensor::getMoisture()
{
	readCalibParameters();

	float result;
	int rawMoisture = getRawMoisture();
	
	if((float)rawMoisture<calibParams.moistMin)
	{
		result = 0;	
	}
	else if ((float)rawMoisture>calibParams.moistMax)
	{
		result = 100;
	}
	else
	{
		result += ((float)((float)rawMoisture-(float)calibParams.moistMin)*calibParams.m);	
		result = result/2;
	}
	
	if(result>100)
		result = 100;

	 return result;
}

int OpenMoistureSensor::getRawMoisture()
{
	iSample  = 0;
	rawValue = 0;
	for(iSample = 0; iSample< MAX_MOIST_SAMPLE; iSample++)
	{
		pinMode(mySamplePin, INPUT);
		
		if(myPowerPin != 0)
			digitalWrite(myPowerPin, HIGH);	
		
		rawValue += analogRead(mySamplePin);
		
		if(myPowerPin != 0)
			digitalWrite(myPowerPin, LOW);
		
		pinMode(mySamplePin, OUTPUT);		
		delay(5);
	}
	rawValue = rawValue/MAX_MOIST_SAMPLE;	
	
	return rawValue;
}

void OpenMoistureSensor::calibHighMoisture()
{
	readCalibParameters();
	calibParams.moistMax = getRawMoisture();
	calibParams.m = 100/(float)(calibParams.moistMax - calibParams.moistMin);
	saveCalibParameters ();
}


void OpenMoistureSensor::calibLowMoisture()
{
	readCalibParameters();
	calibParams.moistMin = getRawMoisture();
	calibParams.m = 100/(float)(calibParams.moistMax - calibParams.moistMin);
	saveCalibParameters ();
}

void OpenMoistureSensor::readCalibParameters()
{
	eeprom_read_block(&calibParams, (void *)EEPROMAddress, sizeof(calibParams));
}

void OpenMoistureSensor::saveCalibParameters()
{
	eeprom_write_block(&calibParams, (void *)EEPROMAddress, sizeof(calibParams));

}

void OpenMoistureSensor::clearCalibParameters()
{
	calibParams.moistMax = 900;
	calibParams.moistMin = 200;
	calibParams.m =1.43;
	
	eeprom_write_block(&calibParams, (void *)EEPROMAddress, sizeof(calibParams));	

}

void OpenMoistureSensor::printCalibParameters()
{
	readCalibParameters();
	Serial.print(F("MoistMax: "));Serial.print(calibParams.moistMax,DEC);Serial.print(F("MoistMin: "));Serial.print(calibParams.moistMin,DEC); Serial.print(F("M: "));Serial.println(calibParams.m,DEC);

}




/****************************************************************************/