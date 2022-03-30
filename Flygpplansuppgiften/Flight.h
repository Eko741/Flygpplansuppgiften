#pragma once
#include <iostream>
#include "Constants.h"
class Flight
{
public:// Fixa dynamisk bindning
	Flight(unsigned int F, bool AB) : fuel(F), airborne(AB), waitTimeAir(0), waitTimeGround(0){}
	Flight* FlightAfter() { return flightAfter; }
	Flight* FlightBefore() { return flightBefore;}
	unsigned int Fuel() { return fuel; }
	unsigned int WaitTimeAir() { return waitTimeAir; }
	unsigned int WaitTimeGround() { return  waitTimeGround; }

	void FlightAfter(Flight* ptr) { flightAfter = ptr; }
	void FlightBefore(Flight* ptr) { flightBefore = ptr; }
	bool tick(); //Returns false if a plane is out of fuel. Also ticks every planes fuel down and adds wait time
	const Flight& operator<<(const Flight& a); //Copy data but not pointers used for swapping planes

private:

	Flight* flightAfter = nullptr; //The flight that is in front of this one in the queue
	Flight* flightBefore = nullptr; //The flight that is behind this one in the queue
	unsigned int fuel;
	unsigned int waitTimeAir; // In mins
	unsigned int waitTimeGround; // In mins
	bool airborne;

};