#pragma once
#include "FlightHandler.h"

FlightHandler::~FlightHandler()
{
	
	Flight* current;
	if (current = firstA) {
		while (current->FlightBefore()) {
			current = current->FlightBefore();
			delete current->FlightAfter();
		}
		delete lastA;
	}

	if (current = firstG) {
		while (current->FlightBefore()) {
			current = current->FlightBefore();
			delete current->FlightAfter();
		}
		delete lastG;
	}
}

void FlightHandler::addFlightA()
{
	Flight* f = new Flight(rand() % 6 + 6, true); // All planes have 30 to 60 minutes of fuel left
	if (!lengthA)
		firstA = f;
	else {
		f->FlightAfter(lastA);
		lastA->FlightBefore(f);
	}
	lastA = f;
	lengthA++;
}

void FlightHandler::addFlightG()
{
	Flight* f = new Flight(100, false); // All planes have 30 to 60 minutes of fuel left
	if (!lengthG)
		firstG = f;
	else {
		f->FlightAfter(lastG);
		lastG->FlightBefore(f);
	}
	lastG = f;
	lengthG++;
}

void FlightHandler::useStrip() 
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
		Flight* currentF = firstA;
		int current = 0;
		while (currentF) {
			if (currentF->Fuel() <= 3) {
				swapFlightsA(0, current);
				break;
			}
			currentF = currentF->FlightBefore();
			current++;
		}

		Flight* thisPlane = firstA;
		firstA = thisPlane->FlightBefore();
		thisPlane->FlightAfter(nullptr);
		lastA->FlightBefore(nullptr);
		//Record plane stats
		if (thisPlane->WaitTimeAir() > longestWaitTimeAir)
			longestWaitTimeAir = thisPlane->WaitTimeAir();
		averageWaitTimeAir += thisPlane->WaitTimeAir();
		delete thisPlane;
		lengthA--;
		planesLanded++;
		stripCooldown = 3;
		return;
	}

	// A plane is taking off
	if (lengthG) {
		Flight* thisPlane = firstG;
		firstG = thisPlane->FlightBefore();
		thisPlane->FlightAfter(nullptr);
		lastG->FlightBefore(nullptr);
		//Record plane stats
		if (thisPlane->WaitTimeGround() > longestWaitTimeGround)
			longestWaitTimeGround = thisPlane->WaitTimeGround();
		averageWaitTimeGround += thisPlane->WaitTimeGround();
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

void FlightHandler::swapFlightsA(unsigned int a, unsigned int b)
{
	if (a >= lengthA || b >= lengthA || a==b)
		return;

	Flight* flightA = nullptr;
	Flight* flightB = nullptr;
	Flight* currentF = firstA;
	
	int current = 0;
	while (currentF) {
		if (current == a)
			flightA = currentF;
		else if (current == b)
			flightB = currentF;
		currentF = currentF->FlightBefore();
		current++;
	}

	Flight tmp(*flightA);
	*flightA << *flightB;
	*flightB << tmp;
}

Flight& FlightHandler::getFlightA(const int point) const
{
	Flight* current = firstA;
	for (int i = 0; i < point; i++)
		current = current->FlightAfter();
	return *current;
}

Flight& FlightHandler::getFlightG(const int point) const
{
	Flight* current = firstG;
	for (int i = 0; i < point; i++)
		current = current->FlightAfter();
	return *current;
}

bool FlightHandler::tick(int spawnRate)
{
	if (!stripCooldown--)  
		useStrip();
	

	Flight* current = firstA;
	while (current) {
		if (!current->tick()) {
			if(planesLanded)
				averageWaitTimeAir /= planesLanded;
			if(planesTakeOf)
				averageWaitTimeGround /= planesTakeOf;
			return false;
		}
		current = current->FlightBefore();
	}
	current = firstG;
	while (current) {
		current->tick();
		current = current->FlightBefore();
	}
	if (rand() % 1000 > 1000 - spawnRate)
		addFlightA();
	if (rand() % 1000 > 1000 - spawnRate)
		addFlightG();
	return true;
}

std::ostream* FlightHandler::operator>>(std::ostream* cout) const
{

	Flight* current = firstA;
	*cout << "Airborne: " << std::endl << "Wait time air:  Wait time ground:  Fuel:" << std::endl;

	while (current) {
		*cout << current->WaitTimeAir() << "  " << current->WaitTimeGround() << "  " << current->Fuel() << std::endl;
		current = current->FlightBefore();
	}

	current = firstG;
	*cout << "On ground: " << std::endl << "Wait time air, Wait time ground, Fuel" << std::endl;

	while (current) {
		*cout << current->WaitTimeAir() << "  " << current->WaitTimeGround() << "  " << current->Fuel() << std::endl;
		current = current->FlightBefore();

		*cout << std::endl;
		return cout;
	}
}
