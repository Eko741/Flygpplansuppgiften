#pragma once
#include "Constants.h"
#include "Flight.h"
#include <iostream>
#include <fstream>
//"Optimized" version of the simulation
class FlightHandler {
public:
	// Implement many landingstrips, different airplane priorites algorithms, landingstrip use time
	FlightHandler(int NOS, int UT, int algo);
	~FlightHandler();

	unsigned int Length() { return lengthG + lengthA; }
	void addFlightA();
	void addFlightG();
	void useStrip();
	void swapFlightsA(Flight* const a,Flight* const b);
	Flight& getFlightA(const int point) const;
	Flight& getFlightG(const int point) const;
	bool tick(int spawnRate);

private:
	int numberOfStrips;
	int useTime;
	int algorithm;

	unsigned int lengthA;
	Flight* firstA;
	Flight* lastA;

	unsigned int lengthG;
	Flight* firstG;
	Flight* lastG;

	int* stripCooldown;

	static uint64_t x;
	static uint64_t numberGenerator();
};