
static int up_queue[4] = {0};
static int down_queue[4] = {0};

void set_queue(int order, int pos){
	
	if(order == pos){
		up_queue[order] == down_queue[order] = 1;
	}
	else if ((order > pos){
		up_queue[order] = 1;
	}
	else if ((order > pos){
		down_queue[order] = 1;
	}
	
}

void set_up_queue(int order){
	up_queue[order] = 1;
}

void set_down_queue(int order){
	down_queue[order] = 1;
}

void delete_floor(int floor){
	up_queue[floor] = down_queue[floor] = 0;
}

void delete_queue(void){
	int i;
	for (i =0; i < 4; i++){
		delete_floor(i);
	}
}

int get_queue(int floor, int current_dir){
	int i;
	for (i = floor; (i > -1) && (i < 4); i + current_dir){
		if(check_floor(i, current_dir) == 1){
			return 1;
			break;
		}
	}
	return -1;
}

int check_floor(int floor, int current_dir){
		if (current_dir > 0){
		return up_queue[floor];
	}
	else if (current_dir < 0){
		return down_queue[floor];
	}
}