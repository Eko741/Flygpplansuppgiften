#pragma once
#include "Queue_object.h"
class FlightHandlerD {
	//Alomst exactly the same as Flight Handler but with some tweaks to make it work with a general linked list 
public:
	~FlightHandlerD();
	FlightHandlerD(int& LWTA, int& LWTG, double& AWTA, double& AWTG, double& UT) : lengthA(0), firstA(nullptr), lastA(nullptr),
		lengthG(0), firstG(nullptr), lastG(nullptr), longestWaitTimeAir(LWTA), longestWaitTimeGround(LWTG), averageWaitTimeAir(AWTA),
		averageWaitTimeGround(AWTG), unusedTime(UT), planesLanded(0), planesTakeOf(0), stripCooldown(0) {}
	
	unsigned int Length() { return lengthG + lengthA; }
	void addFlightA();
	void addFlightG();
	void useStrip();
	void swapFlightsA(QueueObject* const  a, QueueObject* const b);
	QueueObject& getFlightA(const int point) const;
	QueueObject& getFlightG(const int point) const;
	bool tick(int spawnRate); 

private:
	unsigned int lengthA;
	QueueObject* firstA;
	QueueObject* lastA;

	unsigned int lengthG;
	QueueObject* firstG;
	QueueObject* lastG;

	int planesLanded;
	int planesTakeOf;
	int stripCooldown;

	int& longestWaitTimeAir;
	int& longestWaitTimeGround;
	double& averageWaitTimeAir;
	double& averageWaitTimeGround;
	double& unusedTime;
};