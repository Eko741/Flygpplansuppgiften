#pragma once
#include "Data.h"
class FlightD : public Data{
public:
	void info() {}
	FlightD(unsigned int F, bool AB) : fuel(F), airborne(AB) {}

	unsigned int Fuel() { return fuel; }
	unsigned int WaitTimeAir() { return waitTimeAir; }
	unsigned int WaitTimeGround() { return  waitTimeGround; }

	bool tick();
	const FlightD& operator<<(const FlightD& a); //Copy data but not pointers
private:
	unsigned int fuel;
	unsigned int waitTimeAir = 0; // In mins
	unsigned int waitTimeGround = 0; // In mins
	bool airborne;
};
