#pragma once
#include "Queue_object.h"
#include <iostream>
#include <fstream>
#include "FlightD.h"

class FlightHandlerD {
public:
	FlightHandlerD(int& LWTA, int& LWTG, double& AWTA, double& AWTG, double& UT) : lengthA(0), firstA(nullptr), lastA(nullptr),
		lengthG(0), firstG(nullptr), lastG(nullptr), longestWaitTimeAir(LWTA), longestWaitTimeGround(LWTG), averageWaitTimeAir(AWTA),
		averageWaitTimeGround(AWTG), unusedTime(UT) {}
	~FlightHandlerD();
	unsigned int Length() { return lengthG + lengthA; }
	void addFlightA();
	void addFlightG();
	void useStrip();
	void swapFlightsA(unsigned int a, unsigned int b);
	QueueObject& getFlightA(const int point) const;
	QueueObject& getFlightG(const int point) const;
	bool tick(int spawnRate);

private:
	int stripCooldown = 0;

	unsigned int lengthA;
	QueueObject* firstA;
	QueueObject* lastA;

	unsigned int lengthG;
	QueueObject* firstG;
	QueueObject* lastG;

	int& longestWaitTimeAir;
	int& longestWaitTimeGround;
	double& averageWaitTimeAir;
	double& averageWaitTimeGround;
	double& unusedTime;

	int planesLanded = 0;
	int planesTakeOf = 0;
};