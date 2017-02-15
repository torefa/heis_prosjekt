//Header for the timer class
//
#include <stdio.h>
#include <unistd.h>

// Function to start the timer.
void timer_start(){
  sleep(3);
  time_out(0);
}

// Funtion to notify that the time is out
int time_out(int time){
  return time;
}
