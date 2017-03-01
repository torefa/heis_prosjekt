// The state machine for the elevator
#pragma once

#include "elev.h"

// Event button is pressed
void evButtonPressed(elev_button_type_t button, int floor);

// Event floor is reached
void evFloor_reached(int floor);

// Event timer has timed out
void evTime_out(void);

// Event Stop button is pressed
void evStop_button_signal(void);
