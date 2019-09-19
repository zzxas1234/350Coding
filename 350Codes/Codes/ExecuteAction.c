// Action Execute Function

void execute_action(void){
//switch-case - action based on system state

    switch (system_state)
    {
        case 0: //"reset_Default":
            printf("Reset default\n");
            break;
        case 1: //"PUE":
            printf("Possible User Error\n");
            break;
        case 2: //"fall_Out":
            printf("Emergency Warning: Fall Out\n");
            // send warning (high priority)
            break;
        case 3: //"sit_Exit":
            printf("Warning: Sit-Up Exit\n");
            // send warning (low priority)
            break;
        case 4: //"bed_Occupied":
            printf("Bed Occupied\n");
            break;
        case 5: //"bed_Enter" : 
            printf("Bed Entered\n");
            break;
        case 6: //"sit_Warning":
            printf("Warning: Sit-Up, Possible Exit");
            // send warning (low priority)
            break;
        case 7: //"usds_Covered":
            printf("Distance Sensor Covered");
            break;
        case 8: //"error":
            printf("System State Error");
            break;
    }

    if(system_state == 2|3|6){
        // Check cancel for 10 seconds
        int i; 
        for (i = 0; i < 10; i++) { 
            // delay of one second 
            vTaskDelay(1000/portTICK_PERIOD_MS);

            printf("%d seconds have passed\n", i + 1);
        
            if (cancel == 1){
            // send cancel - set cancel pin connected to VitalCare chip
            // disable warning pin
            }
        } 
    }

return();
}