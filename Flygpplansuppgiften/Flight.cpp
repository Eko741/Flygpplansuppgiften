#pragma once
#include "Flight.h"

bool Flight::tick() {
		return --fuel; 
}

const Flight& Flight::operator<<(const Flight& a)
{ 
	fuel = a.fuel;
	airborne = a.airborne;
	return *this;
}