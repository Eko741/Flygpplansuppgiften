#include "FlightD.h"

bool FlightD::tick() {
	if (airborne) {
		waitTimeAir += 5;
		return --fuel;
	}
	else {
		waitTimeGround += 5;
		return true;
	}
}

const FlightD& FlightD::operator<<(const FlightD& a)
{
	fuel = a.fuel;
	waitTimeAir = a.waitTimeAir;
	waitTimeGround = a.waitTimeGround;
	airborne = a.airborne;
	return *this;
}
