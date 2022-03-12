#include "Flight.h"
bool Flight::tick() {
	if (airborne) {
		waitTimeAir += 5;
		return --fuel;
	}
	else
	{
		waitTimeGround += 5;
		return true;
	}
}

const Flight& Flight::operator<<(const Flight& a)
{ 
	fuel = a.fuel;;
	waitTimeAir = a.waitTimeAir;
	waitTimeGround = a.waitTimeGround;
	airborne = a.airborne;
	return *this;
}
