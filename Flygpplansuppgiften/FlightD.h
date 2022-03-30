#pragma once
#include "Data.h"
class FlightD : public Data{
//Alomst exactly the same as Flight but with some tweaks to make it work with a more general linked list

public:
	FlightD(unsigned int F, bool AB) : fuel(F), airborne(AB), waitTimeAir(0), waitTimeGround(0){}

	unsigned int Fuel() { return fuel; }
	unsigned int WaitTimeAir() { return waitTimeAir; }
	unsigned int WaitTimeGround() { return  waitTimeGround; }

	bool tick(); //Not a recusrive function in this verison
private:
	unsigned int fuel;
	unsigned int waitTimeAir; 
	unsigned int waitTimeGround;
	bool airborne;
};
