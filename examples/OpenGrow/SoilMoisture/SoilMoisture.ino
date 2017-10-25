/*
* Copyright (C) 2014-2015 OpenGrow GroLab by João Melo <joao.melo@opengrow.pt>
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

//Main GroBot Definitions
	
#include <Arduino.h>
#include "GroLabMoistureSensor.h"
#include "HardwareSerial.h"

#define moistSensorCalibAddress 0x01

//Pin A0 as the analog input and pin A5 does the ON/OFF
OpenMoistureSensor moistSensor(A0,A5,moistSensorCalibAddress);	



void setup(void)
{
	Serial.begin(230400);
	Serial.setTimeout(1);
}

void loop(void)
{
	if(Serial.available() ) 
	{
		char c = Serial.read();
		if(c == 'H')
		{
			//Calib HIGH
			moistSensor.calibHighMoisture();
			Serial.print(F("Calib High Saved!"));
		}
		if(c == 'L')
		{
			//Calib LOW
			moistSensor.calibLowMoisture();
			Serial.print(F("Calib Low Saved!"));
		}
		if(c == 'P')
		{
			//Print Calib Data
			moistSensor.printCalibParameters();
		}
		if(c == 'R')
		{
			//Reset Calib Data
			moistSensor.clearCalibParameters();
			Serial.print(F("Calib Clear!"));
		}
	}
	
	Serial.print(F("Moisture: "));
	Serial.print(moistSensor.getMoisture(), 2);
	Serial.println(F("%"));

}
/****************************************************************************/

