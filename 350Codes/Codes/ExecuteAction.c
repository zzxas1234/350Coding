// Action Execute Function
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "driver/dac.h"
#include "esp_adc_cal.h"
#include "time.h"

static int system_state = 0;

static void execute_action(void){
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
            // gpio_set_level(gpio_num_tgpio_num, uint32_t level)
            break;
        case 3: //"sit_Exit":
            printf("Warning: Sit-Up Exit\n");
            // gpio_set_level(gpio_num_tgpio_num, uint32_t level)
            break;
        case 4: //"bed_Occupied":
            printf("Bed Occupied\n");
            // gpio_set_level(gpio_num_tgpio_num, uint32_t level)
            break;
        case 5: //"bed_Enter" : 
            printf("Bed Entered\n");
            // gpio_set_level(gpio_num_tgpio_num, uint32_t level)
            break;
        case 6: //"sit_Warning":
            printf("Warning: Sit-Up, Possible Exit");
            // gpio_set_level(gpio_num_tgpio_num, uint32_t level)
            break;
        case 7: //"usds_Covered":
            printf("Distance Sensor Covered");
            // gpio_set_level(gpio_num_tgpio_num, uint32_t level)
            break;
        case 8: //"error":
            printf("System State Error");
            break;
    }

return();
}