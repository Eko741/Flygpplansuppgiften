#pragma once
#include "Flight.h"
#include <iostream>
class FlightHandler {
public:
	FlightHandler() : lengthA(0), firstA(nullptr), lastA(nullptr), lengthG(0), firstG(nullptr), lastG(nullptr) {}

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
	unsigned int lengthA;
	Flight* firstA;
	Flight* lastA;

	unsigned int lengthG;
	Flight* firstG;
	Flight* lastG;
};