// The state machine for the elevator
#pragma once

// Event button is pressed
void evButtonPressed(void);

// Event floor is reached
void evFloor_reached(void);

// Event timer has timed out
void evTime_out(void);

// Event Stop button is pressed
void evStop_button_signal(void);
