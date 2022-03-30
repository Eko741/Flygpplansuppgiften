#pragma once
#include "Constants.h"
#include "Flight.h"
#include <iostream>
#include <fstream>
//"Optimized" version of the simulation
class FlightHandler {
public:
	FlightHandler(int& LWTA, int& LWTG, double& AWTA, double & AWTG, double & UT) : longestWaitTimeAir(LWTA),
		longestWaitTimeGround(LWTG), averageWaitTimeAir(AWTA), averageWaitTimeGround(AWTG), unusedTime(UT),
		lengthA(0), firstA(nullptr), lastA(nullptr), lengthG(0), firstG(nullptr), lastG(nullptr), planesLanded(0),
		planesTakeOf(0), stripCooldown(0){}
	~FlightHandler();

	unsigned int Length() { return lengthG + lengthA; }
	void addFlightA();
	void addFlightG();
	void useStrip();
	void swapFlightsA(Flight* const a,Flight* const b);
	Flight& getFlightA(const int point) const;
	Flight& getFlightG(const int point) const;
	bool tick(int spawnRate);
	std::ostream* operator>>(std::ostream* cout)const; 
	void reset();

private:
	unsigned int lengthA;
	Flight* firstA;
	Flight* lastA;

	unsigned int lengthG;
	Flight* firstG;
	Flight* lastG;

	int stripCooldown;
	int planesLanded;
	int planesTakeOf;

	int& longestWaitTimeAir;
	int& longestWaitTimeGround;
	double& averageWaitTimeAir;
	double& averageWaitTimeGround;
	double& unusedTime;

	static uint64_t x;
	static uint64_t numberGenerator();
};