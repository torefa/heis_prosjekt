
static int up_queue[4] = {0};
static int down_queue[4] = {0};

void queue_set_queue(int order, int pos){
	
	if(order == pos){
		queue_up_queue[order] == down_queue[order] = 1;
	}
	else if ((order > pos){
		queue_up_queue[order] = 1;
	}
	else if ((order > pos){
		queue_down_queue[order] = 1;
	}
	
}

void queue_set_up_queue(int order){
	queue_up_queue[order] = 1;
}

void queue_set_down_queue(int order){
	queue_down_queue[order] = 1;
}

void queue_delete_floor(int floor){
	queue_up_queue[floor] = queue_down_queue[floor] = 0;
}

void queue_delete_queue(void){
	int i;
	for (i =0; i < 4; i++){
		queue_delete_floor(i);
	}
}

int queue_get_queue(int floor, int motor_dir){
	int i;
	for (i = floor; (i > -1) && (i < 4); i + motor_dir){
		if(queue_check_floor(i, motor_dir) == 1){
			return 1;
			break;
		}
	}
	return -1;
}

int queue_check_floor(int floor, int motor_dir){
		if (motor_dir > 0){
		return queue_up_queue[floor];
	}
	else if (motor_dir < 0){
		return queue_down_queue[floor];
	}
}