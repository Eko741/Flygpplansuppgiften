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
		int leastFuel = 100, planesWithLeastFuel = 0;
		QueueObject* planeWithLeastFuel = nullptr;
		QueueObject* currentF = firstA;

		while (currentF) {
			int actualFuel = (((FlightD*)currentF->getData())->Fuel() - 1) / 3; // Cualculate the actual fuel
			if (actualFuel < leastFuel) { //If this is the lowest yet
				leastFuel = actualFuel;	// Update the least fuel
				planesWithLeastFuel = 1;
				planeWithLeastFuel = currentF;
			}
			else if (actualFuel == leastFuel) { //If another plane also has this amount of fuel increment the amount
				if (((FlightD*)currentF->getData())->Fuel() < ((FlightD*)currentF->getData())->Fuel()) // Better for more than one landing strip
					planeWithLeastFuel = currentF;
				planesWithLeastFuel++;
			}
			currentF = currentF->Prev();
		}
		if (leastFuel < planesWithLeastFuel) // If too avoid a crash a change of the order needs to be made
			swapFlightsA(firstA, planeWithLeastFuel);

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

void FlightHandlerD::swapFlightsA(QueueObject* const  a, QueueObject* const b)
{
	Data* tmp = a->getData();
	a->setData(b->getData());
	b->setData(tmp);
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