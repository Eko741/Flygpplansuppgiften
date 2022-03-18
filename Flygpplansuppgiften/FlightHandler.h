#pragma once
#include "Flight.h"
#include <iostream>
#include <fstream>
class FlightHandler {
public:
	FlightHandler(int& LWTA, int& LWTG, double& AWTA, double & AWTG, double & UT) : lengthA(0), firstA(nullptr), lastA(nullptr),
		lengthG(0), firstG(nullptr), lastG(nullptr), longestWaitTimeAir(LWTA), longestWaitTimeGround(LWTG), averageWaitTimeAir(AWTA),
		averageWaitTimeGround(AWTG), unusedTime(UT){}
	~FlightHandler();
	unsigned int Length() { return lengthG + lengthA; }
	void addFlightA();
	void addFlightG();
	void useStrip();
	void swapFlightsA(unsigned int a, unsigned int b);
	Flight& getFlightA(const int point) const;
	Flight& getFlightG(const int point) const;
	bool tick(int spawnRate);
	std::ostream* operator>>(std::ostream* cout)const;

private:
	int stripCooldown = 0;

	unsigned int lengthA;
	Flight* firstA;
	Flight* lastA;

	unsigned int lengthG;
	Flight* firstG;
	Flight* lastG;

	int & longestWaitTimeAir;
	int & longestWaitTimeGround;
	double & averageWaitTimeAir;
	double & averageWaitTimeGround;
	double & unusedTime;

	int planesLanded = 0;
	int planesTakeOf = 0;
};