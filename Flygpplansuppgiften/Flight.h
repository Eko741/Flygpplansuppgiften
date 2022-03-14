#pragma once
#include <fstream>
class Flight
{
public:
	Flight(unsigned int F, bool AB) : flightAfter(nullptr), flightBefore(nullptr), fuel(F), airborne(AB) {}

	Flight* FlightAfter() { return flightAfter; }
	Flight* FlightBefore() { return flightBefore;}
	unsigned int Fuel() { return fuel; }
	unsigned int WaitTimeAir() { return waitTimeAir; }
	unsigned int WaitTimeGround() { return  waitTimeGround; }

	void FlightAfter(Flight* ptr) { flightAfter = ptr; }
	void FlightBefore(Flight* ptr) { flightBefore = ptr; }
	bool tick();
	const Flight& operator<<(const Flight& a); //Copy data but not pointers

private:

	Flight* flightAfter; //The flight that is in front of this one in the queue
	Flight* flightBefore; //The flight that is behind this one in the queue
	unsigned int fuel;
	unsigned int waitTimeAir = 0; // In mins
	unsigned int waitTimeGround = 0; // In mins
	bool airborne;
};