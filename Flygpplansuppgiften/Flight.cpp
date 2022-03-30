#pragma once
#include "Flight.h"

bool Flight::tick() {
	if (airborne) {
		waitTimeAir += TIMETOUSESTRIP;
		if (flightBefore) //If this is not the last plane in the queue
			return --fuel && flightBefore->tick(); //If fuel is 0 return false else continue the loop
		return --fuel; //Return false if the fuel is 0
	}
	else{
		waitTimeGround += TIMETOUSESTRIP;
		return flightBefore && flightBefore->tick(); //If this is not the last plane in the queue tick the next plane
	}
}

const Flight& Flight::operator<<(const Flight& a)
{ 
	fuel = a.fuel;
	waitTimeAir = a.waitTimeAir;
	waitTimeGround = a.waitTimeGround;
	airborne = a.airborne;
	return *this;
}