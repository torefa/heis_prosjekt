#include "queue.h"

#include <stdio.h>

#define NO_ORDER 0
#define ORDER 1

static int up_queue[4] = {0};
static int down_queue[4] = {0};

// Set queue with orders from internal panel.
void queue_set_queue(int order, int pos){
	if(order == pos){			
		//If order is for the same floor as elevators current position.
		up_queue[order] = down_queue[order] = ORDER;
	} else if (order > pos){	
		//If order is over elevators current position.
		up_queue[order] = ORDER;
	} else if (order < pos){	
		//If order is under elevators current position.
		down_queue[order] = ORDER;
	}
}

// Set queue with orders from UP buttons.
void queue_set_up_queue(int order){
	up_queue[order] = ORDER;
}

// Set queue with orders from DOWN buttons.
void queue_set_down_queue(int order){
	down_queue[order] = ORDER;
}

// Remove floor from queue when elevator stops in floor.
void queue_delete_floor(int floor){
	up_queue[floor] = down_queue[floor] = NO_ORDER;
}

// Remove all floors from all queues. (Stop button pressed).
void queue_delete_queue(void){
	int i;
	for (i = FIRST; i < 4; i++){
		queue_delete_floor(i);
	}
}

// Check if current floor is in the queue for the current direction.
int queue_check_floor(int floor, int motor_dir){
	if (motor_dir == 1){			
		return up_queue[floor];
	} else if (motor_dir == -1){	
		return down_queue[floor];

	}
	return NO_ORDER;
}


// Check the queues for any orders.
int queue_get_queue(int floor, int motor_dir){
	int i;
	int temp;
	for (i = floor; (i > -1) && (i < 4); i = i + motor_dir){	//Checks all floors in direction elevator is moving for orders
		if(queue_check_floor(i, motor_dir) == ORDER){
			temp = ORDER;
			break;
		} else{
			temp = NO_ORDER;
		}
	}
	return temp;
}

