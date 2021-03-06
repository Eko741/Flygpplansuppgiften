#pragma once
#include "FlightHandlerD.h"
#include "Data.h"
#include "Constants.h"
#include <iostream>
#include "FlightD.h"


FlightHandlerD::~FlightHandlerD()
{
	QueueObject* current;
	if (current = firstA) {
		while (current = current->Prev()) 
			delete current->Next();
		delete lastA;
	}

	if (current = firstG) {
		while (current = current->Prev())
			delete current->Next();
		delete lastG;
	}
}

void FlightHandlerD::addFlightA()
{
	QueueObject* f = new QueueObject(new FlightD(rand() % 6 + 6, true)); // All planes have 30 to 60 minutes of fuel left
	if (!lengthA)
		firstA = f;
	else {
		f->Next(lastA);
		lastA->Prev(f);
	}
	lastA = f;
	lengthA++;
}

void FlightHandlerD::addFlightG()
{
	QueueObject* f = new QueueObject(new FlightD(std::rand() % 6 + 6, true)); // All planes have 30 to 60 minutes of fuel left
	if (!lengthG)
		firstG = f;
	else {
		f->Next(lastG);
		lastG->Prev(f);
	}
	lastG = f;
	lengthG++;
}

void FlightHandlerD::useStrip()
{
	if (lengthA) {
		// If a flight will crash before the next flight is done landing, make that flight be the one landing
		QueueObject* currentF = firstA;
		while (currentF) {
			if (dynamic_cast<FlightD*>(currentF->getData())->Fuel() <= 3) {
				QueueObject::swapFlights(firstA, currentF);
				break;
			}
			currentF = currentF->Prev();
		}

		QueueObject* thisPlane = firstA;
		FlightD* thisFlight = (FlightD*)(firstA->getData());//For easier reading of code
		firstA = thisPlane->Prev();
		if (firstA)
			firstA->Next(nullptr);
		if (thisFlight->WaitTimeAir() > longestWaitTimeAir)
			longestWaitTimeAir = thisFlight->WaitTimeAir();
		averageWaitTimeAir += thisFlight->WaitTimeAir();
		delete thisPlane;
		lengthA--;
		planesLanded++;
		stripCooldown = TIMETOUSESTRIP - 1;
		return;
	}

	// A plane is taking off
	if (lengthG) {
		QueueObject* thisPlane = firstG;
		FlightD* thisFlight = (FlightD*)(firstG->getData()); //For easier reading of code
		firstG = thisPlane->Prev();
		if (firstG)
			firstG->Next(nullptr);
		if (thisFlight->WaitTimeGround() > longestWaitTimeGround)
			longestWaitTimeGround = thisFlight->WaitTimeGround();
		averageWaitTimeGround += thisFlight->WaitTimeGround();
		delete thisPlane;
		lengthG--;
		planesTakeOf++;
		stripCooldown = TIMETOUSESTRIP - 1;
		return;
	}
	unusedTime += TIMEPERIOD;
	stripCooldown = 0;
	return;
}

QueueObject& FlightHandlerD::getFlightA(const int point) const
{
	QueueObject* current = firstA;
	for (int i = 0; i < point; i++)
		current = current->Next();
	return *current;
}

QueueObject& FlightHandlerD::getFlightG(const int point) const
{
	QueueObject* current = firstG;
	for (int i = 0; i < point; i++)
		current = current->Next();
	return *current;
}

bool FlightHandlerD::tick(int spawnRate)
{
	if (!stripCooldown--)
		useStrip();


	QueueObject* current = firstA;
	while (current) {
		if (!current->getData()->tick()) {
			if (planesLanded)
				averageWaitTimeAir /= planesLanded;
			if (planesTakeOf)
				averageWaitTimeGround /= planesTakeOf;
			return false;
		}
		current = current->Prev();
	}
	current = firstG;
	while (current) {
		current->getData()->tick();
		current = current->Prev();
	}
	if (rand() % 1000 > 1000 - spawnRate)
		addFlightA();
	if (rand() % 1000 > 1000 - spawnRate)
		addFlightG();
	return true;
}