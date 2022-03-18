#pragma once
#include "FlightHandlerD.h"

FlightHandlerD::~FlightHandlerD()
{
	QueueObject* current;
	if (current = firstA) {
		while (current->Prev()) {
			current = current->Prev();
			delete current->Next();
		}
		delete lastA;
	}

	if (current = firstG) {
		while (current->Prev()) {
			current = current->Prev();
			delete current->Next();
		}
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
	QueueObject* f = new QueueObject(new FlightD(rand() % 6 + 6, true)); // All planes have 30 to 60 minutes of fuel left
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
	/*
	Better system but not the assignment
	if (lengthA) { // A flight is landing
		// "this flight" means the one that is landing
		Flight* thisPlane = firstA;
		if(lastG)
			lastG->FlightBefore(thisPlane);// Last flight in the ground queue knows that this plane has landed
		thisPlane->FlightAfter(lastG); // This flight knows which plane is ahead in the ground queue
		lastG = thisPlane; // Last flight in ground queue is now this flight
		if (!firstG)
			firstG = thisPlane;
		firstA = thisPlane->FlightBefore();// First flight in air  queue is now the flight before this one
		if (firstA) {
			firstA->FlightAfter(nullptr); // First flight in air  queue now has no flight after it
		}
		thisPlane->FlightBefore(nullptr); // This flight doesn't have a flight before
		lengthA--;
		lengthG++;
		return;
	}
	*/

	if (lengthA) {
		QueueObject* currentF = firstA;
		int current = 0;
		while (currentF) {
			if (dynamic_cast<FlightD*>(currentF->getData())->Fuel() <= 3) {
				swapFlightsA(0, current);
				break;
			}
			currentF = currentF->Prev();
			current++;
		}

		QueueObject* thisPlane = firstA;
		FlightD* thisFlight = dynamic_cast<FlightD*>(firstA->getData());
		firstA = thisPlane->Prev();
		thisPlane->Next(nullptr);
		lastA->Prev(nullptr);
		//Record plane stats
		if (dynamic_cast<FlightD*>(currentF->getData())->WaitTimeAir() > longestWaitTimeAir)
			longestWaitTimeAir = thisFlight->WaitTimeAir();
		averageWaitTimeAir += thisFlight->WaitTimeAir();
		delete thisPlane;
		lengthA--;
		planesLanded++;
		stripCooldown = 3;
		return;
	}

	// A plane is taking off
	if (lengthG) {
		QueueObject* thisPlane = firstG;
		FlightD* thisFlight = dynamic_cast<FlightD*>(firstG->getData());
		firstG = thisPlane->Prev();
		thisPlane->Next(nullptr);
		lastG->Prev(nullptr);
		//Record plane stats
		if (thisFlight->WaitTimeGround() > longestWaitTimeGround)
			longestWaitTimeGround = thisFlight->WaitTimeGround();
		averageWaitTimeGround += thisFlight->WaitTimeGround();
		delete thisPlane;
		lengthG--;
		planesTakeOf++;
		stripCooldown = 3;
		return;
	}
	unusedTime += 5;
	stripCooldown = 0;
	return;
}

void FlightHandlerD::swapFlightsA(unsigned int a, unsigned int b)
{
	if (a >= lengthA || b >= lengthA || a == b)
		return;

	QueueObject* flightA = nullptr;
	QueueObject* flightB = nullptr;
	QueueObject* currentF = firstA;

	int current = 0;
	while (currentF) {
		if (current == a)
			flightA = currentF;
		else if (current == b)
			flightB = currentF;
		currentF = currentF->Prev();
		current++;
	}

	Data* tmp = flightA->getData();
	flightA->setData(flightB->getData());
	flightB->setData(tmp);
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
