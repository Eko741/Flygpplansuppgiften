#pragma once
#include "FlightD.h"
#include "Constants.h"

bool FlightD::tick() {
	if (airborne) {
		waitTimeAir += TIMETOUSESTRIP;
		return --fuel;
	}
	else {
		waitTimeGround += TIMETOUSESTRIP;
		return true;
	}
}
