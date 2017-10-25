/*
* Copyright (C) 2017 OpenGrow GroLab by João Melo <joao.melo@opengrow.pt>
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef __GROLABMOISTURE_H__
#define __GROLABMOISTURE_H__

	


struct parameters_Moisture
{
	unsigned long moistMax;
	unsigned long moistMin;
	float m;
};

class OpenMoistureSensor
{
	private:		

	protected:

	public:
		
		OpenMoistureSensor(int analogPin, int powerPin,unsigned char EEPROMAddressPointer);
		float getMoisture();	
		int getRawMoisture();
		void calibHighMoisture();
		void calibLowMoisture();
		void readCalibParameters();
		void saveCalibParameters();
		void clearCalibParameters();
		void printCalibParameters();
};

#endif // __GROLABMOISTURE_H__
/****************************************************************************/
