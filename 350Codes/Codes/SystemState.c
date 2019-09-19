// 350 System State

#define USDS_PREV_MASK (1<<0);
#define USDS_CURR_MASK (1<<1);
#define BED_PREV_MASK (1<<2);
#define BED_CURR_MASK (1<<3);

// Variables to be set in other functions
static int usds_current;
static int usds_prev;
static int bed_current;
static int bed_prev;
static int cancel = /* GPIO pin*/;

char system_set(void){

char system_state = 0;
char state_temp = 0;


        if(bed_prev == 1){state_temp |= BED_PREV_MASK};
        else{state_temp &= ~BED_PREV_MASK};

        if(usds_prev == 1){state_temp |= USDS_PREV_MASK};
        else{state_temp &= ~USDS_PREV_MASK};

        if(get_bed_state()){state_temp |= BED_CURR_MASK};
        else {state_temp &= ~BED_CURR_MASK};

        if(get_usds_state()){state_temp |= BED_CURR_MASK};
        else {state_temp &= ~BED_CURR_MASK};

        // if-else - truth table conditional program
        if (state_temp&(BED_CURR_MASK | BED_PREV_MASK | USDS_CURR_MASK | USDS_PREV_MASK) == 0) // system = [ 0 0 0 0 ]
        {
            system_state = 0; // "reset_Default"; // no sensors triggered. Default case.
        }
        else if (state_temp&(BED_CURR_MASK | BED_PREV_MASK | USDS_CURR_MASK | USDS_PREV_MASK) == 3) // system = [0 0 1 1 ]
        {
            system_state = 1; //"PUE"; // usds triggered but system empty. 'PUE' = Possible User Error
        }
        else if (state_temp&(BED_CURR_MASK | BED_PREV_MASK | USDS_CURR_MASK | USDS_PREV_MASK) == 4) // system = [ 0 1 0 0 ]
        {
            system_state = 2; // "fall_Out"; // system vacated, no change in usds
        }
        else if (state_temp&(BED_CURR_MASK | BED_PREV_MASK | USDS_CURR_MASK | USDS_PREV_MASK) == 7) // system = [ 0 1 1 1 ]
        {
            system_state = 3; // "sit_Exit";
        }
        else if (state_temp&(BED_CURR_MASK | BED_PREV_MASK | USDS_CURR_MASK | USDS_PREV_MASK) == 13) // system = [ 1 1 0 1 ]
        {
            system_state = 4; //"bed_Occupied";
        }
        else if (state_temp&(BED_CURR_MASK | BED_PREV_MASK | USDS_CURR_MASK | USDS_PREV_MASK) == 11) // system = [ 1 0 1 1 ]
        {
            system_state = 5; //"bed_Enter";
        }
        else if (state_temp&(BED_CURR_MASK | BED_PREV_MASK | USDS_CURR_MASK | USDS_PREV_MASK) == 15) // system = [ 1 1 1 1 ]
        {
            system_state = 6; //"sit_Warning";
        }
        else if ((usds_current == 2) | (usds_prev == 2))
        {
            system_state = 7 // "usds_Covered";
        }
        else
        {
            system_state = 8; // "error";
        }

        return(system_state);
}