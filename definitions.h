// Header file containing definitions used by multiple modules

// Tregs en forklaring??
typedef enum{
	FIRST,
	SECOND,
	THIRD,
	FOURTH
} el_floor;

/**
  Motor direction for function elev_set_motor_direction().
*/
typedef enum tag_elev_motor_direction { 
    DIRN_DOWN = -1,
    DIRN_STOP = 0,
    DIRN_UP = 1
} elev_motor_direction_t;

/**
  Button types for function elev_set_button_lamp() and elev_get_button().
*/
typedef enum tag_elev_lamp_type { 
    BUTTON_CALL_UP = 0,
    BUTTON_CALL_DOWN = 1,
    BUTTON_COMMAND = 2
} elev_button_type_t;

// Number of floors
#define N_FLOORS 4

// Number of button types
#define N_BUTTONS 3

// Constant for logic operations
#define TRUE 1;
#define FALSE 0;