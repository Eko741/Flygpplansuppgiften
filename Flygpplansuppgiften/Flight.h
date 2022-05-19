#pragma once
#include <iostream>
#include "Constants.h"
class Flight
{
public:// Fixa dynamisk bindning
	Flight(unsigned int F, bool AB) : fuel(F), airborne(AB){}
	Flight* FlightAfter() { return flightAfter; }
	Flight* FlightBefore() { return flightBefore;}
	unsigned int Fuel() { return fuel; }

	void FlightAfter(Flight* ptr) { flightAfter = ptr; }
	void FlightBefore(Flight* ptr) { flightBefore = ptr; }
	bool tick(); //Returns false if a plane is out of fuel. Also ticks every planes fuel down and adds wait time
	const Flight& operator<<(const Flight& a); //Copy data but not pointers used for swapping planes

private:

	Flight* flightAfter = nullptr; //The flight that is in front of this one in the queue
	Flight* flightBefore = nullptr; //The flight that is behind this one in the queue
	unsigned int fuel;
	bool airborne;

};