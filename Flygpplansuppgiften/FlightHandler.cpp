#pragma once
#include "FlightHandler.h"
uint64_t FlightHandler::x = time(NULL); // Set random seed to seconds since 00:00 hours, Jan 1, 1970 UTC
//uinta 64 means that it has excactly 64bits like a long.

uint64_t FlightHandler::numberGenerator() // Fast sudo random generator. From the internet https://thompsonsed.co.uk/random-number-generators-for-c-performance-tested
{
	uint64_t z = (x += UINT64_C(0x9E3779B97F4A7C15));
	z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
	z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
	return z ^ (z >> 31);
}

FlightHandler::~FlightHandler()
{
	// Deletes all the flights controlled by this handler
	Flight* current;
	if (current = firstA) { // Set current to be the first plane in the queue only start this
							//loop if the queue is not empty
		while (current = current->FlightBefore())  // Set current to the plane behind and check if it's a nullptr
			delete current->FlightAfter(); // If not delete the one ahead of this one 
		delete lastA; //Delete the last plane in the queue
	}
	//Same as above but for ground queue
	if (current = firstG) {
		while (current = current->FlightBefore()) 
			delete current->FlightAfter();
		delete lastG;
	}
}

void FlightHandler::addFlightA() // Adds flight to the back of air queue
{
	Flight* f = new Flight(numberGenerator() % 6 + 6, true); // All planes have 30 to 60 minutes of fuel left
	if (!lengthA) //If there is no planes in queue set this plane to be the first
		firstA = f;
	else { // If there is, set this plane to point at the last plane then set that plane to point at this plane
		f->FlightAfter(lastA);
		lastA->FlightBefore(f);
	}
	lastA = f; // Set the last plane to be this plane
	lengthA++; // Incerement the last plane
}

void FlightHandler::addFlightG() // Adds flight to the back of ground queue
{
	//Same as addFlightA
	Flight* f = new Flight(100, false); // All planes in ground queue have 100 fuel
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
	if (lengthA) { // Landings will always be prioritized over take ofs
		//This system is made with one landing strip in mind. Quite crude. It would over prioritze with more landing strips
		int leastFuel = 100, planesWithLeastFuel = 0; // Leastfuel means the plane with the least actual fuel which is
		//How many 15 minutes it can stay in the air before crashing
		Flight* planeWithLeastFuel = nullptr;
		Flight* currentF = firstA;
		while (currentF) {
			int actualFuel = (currentF->Fuel() - 1) / 3; // Cualculate the actual fuel
			if (actualFuel < leastFuel) { //If this is the lowest yet
				leastFuel = actualFuel;	// Update the least fuel
				planesWithLeastFuel = 1;
				planeWithLeastFuel = currentF;
			}
			else if (actualFuel == leastFuel) { //If another plane also has this amount of fuel increment the amount
				if (currentF->Fuel() < planeWithLeastFuel->Fuel()) // Better for more than one landing strip
					planeWithLeastFuel = currentF;
				planesWithLeastFuel++;
			}
			currentF = currentF->FlightBefore();
		}
		if (leastFuel < planesWithLeastFuel) // If too avoid a crash a change of the order needs to be made
			swapFlightsA(firstA, planeWithLeastFuel); // the problematic plane is pushed to the front 

		Flight* thisPlane = firstA; // This plane is the first in the air queue
		firstA = thisPlane->FlightBefore(); // First in queue is now the plane behind this one
		//Statistics
		if (firstA)
			firstA->FlightAfter(nullptr); //The first plane in queue needs to be pointing at a nullpointer
		if (thisPlane->WaitTimeAir() > longestWaitTimeAir)
			longestWaitTimeAir = thisPlane->WaitTimeAir();
		averageWaitTimeAir += thisPlane->WaitTimeAir();
		
		delete thisPlane;
		lengthA--;
		planesLanded++;
		stripCooldown = TIMETOUSESTRIP - 1; // The landingstrip can't be used for 3 time units
		return;
	}

	// A plane is taking off
	if (lengthG) {
		Flight* thisPlane = firstG;
		firstG = thisPlane->FlightBefore();
		if (firstG)
			firstG->FlightAfter(nullptr);
		//Statistics
		if (thisPlane->WaitTimeGround() > longestWaitTimeGround)
			longestWaitTimeGround = thisPlane->WaitTimeGround();
		averageWaitTimeGround += thisPlane->WaitTimeGround();

		delete thisPlane;
		lengthG--;
		planesTakeOf++;
		stripCooldown = TIMETOUSESTRIP - 1;
		return;
	}
	//If there are no planes set stripcooldown to 1 and add 5 min to the unused time
	unusedTime += TIMEPERIOD;
	stripCooldown = 0;
	return;
} 

void FlightHandler::swapFlightsA( Flight* const  a, Flight* const b) // Swaps two flights
{
	Flight tmp(*a);
	*a << *b;
	*b << tmp;
}

Flight& FlightHandler::getFlightA(const int point) const // For bug testing purposes
{
	Flight* current = firstA;
	for (int i = 0; i < point; i++)
		current = current->FlightAfter();
	return *current;
}

Flight& FlightHandler::getFlightG(const int point) const // For bug testing purposes 
{
	Flight* current = firstG;
	for (int i = 0; i < point; i++)
		current = current->FlightAfter();
	return *current;
}

bool FlightHandler::tick(int spawnRate)
{
	if (!stripCooldown--) // If the strip is not in use 
		useStrip();
	
	if(firstA) // If there is a plane in the air queue
		if (!firstA->tick()) { // Tick returns false if a plane crashed
			// Calculate the averages of this cycle then return false to stop this try cycle
			if(planesLanded)
				averageWaitTimeAir /= planesLanded; 
			if(planesTakeOf)
				averageWaitTimeGround /= planesTakeOf;
			return false;
		}
	if (firstG) // If there is plane in the ground queue
		firstG->tick();

	// Randomly spawn planes both in air and ground queue
	if (numberGenerator() % 1000 > 1000 - spawnRate) 
		addFlightA();
	if (numberGenerator() % 1000 > 1000 - spawnRate)
		addFlightG();
	return true;
}

std::ostream* FlightHandler::operator>>(std::ostream* cout) const // For bug testing purposes 
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

void FlightHandler::reset() // Like creating a new object but without loosing the refrences
// Could be faster probably not though
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

	stripCooldown = 0;
	lengthA = 0;
	firstA = nullptr;
	lastA = nullptr;
	lengthG = 0;
	firstG = nullptr;
	lastG = nullptr;
	planesLanded = 0;
	planesTakeOf = 0;
}
