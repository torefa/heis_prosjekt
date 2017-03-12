// The state machine for the elevator
#ifndef elev_state_machine
#define elev_state_machine

#include <stdio.h>
#include "elev.h"
#include "timer.h"
#include "queue.h"


void evInitialize(void);

// Event button is pressed
void evButton_pressed(elev_button_type_t button, int floor);

// Event floor is reached
void evFloor_reached(int floor);

// Event timer has timed out
void evTime_out(void);

// Event Stop button is pressed
void evStop_button_signal(int signal);

#endif