
//
// Analog Input Sensor Reading (Polling)
//

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include <string.h>
#include <stdlib.h>
#include "driver/adc.h"
#include "driver/gpio.h"

#define USDS_PREV_MASK (1<<0);
#define USDS_CURR_MASK (1<<1);
#define BED_PREV_MASK (1<<2);
#define BED_CURR_MASK (1<<3);

int get_bed_state();
int get_us_state();


adc1_config_width(adc_width_10bit);                       // 10 bit resolution ( 0-3.3v == 0-1023 arbitrary value scale). High resolution, slow read.
                                                          // Width reccommended by USDS datasheet for calcualtion purposes
adc1_config_channel_atten(adc1_channel_0, adc_atten_max); // attenuation = 0, no reduction in input voltage (done externally via hardware)


int get_bed_state()
{
    // Read analog values from input pins
    // will return a value on the 0-4095 scale which corresponds to Vout and therefore mass/cm^2
    int fsr1_value = adc1_get_raw(adc1_channel_0); // GPIO36
    int fsr2_value = adc1_get_raw(adc1_channel_3); // GPIO39

    // display value for testing, calibrate scale to mass applied
    printf("fsr1 value = %d\n", fsr1_value);
    printf("fsr2 value = %d\n", fsr2_value);

    // Compare to conditional threshold values (If fsr_force >= x...)
    if (fsr1_value <= x)
    {
        int fsr1_state = y;
    }
    else
    {
        fsr1_state = z;
    }

    // Compare to conditional threshold values (If fsr_force >= x...)
    if (fsr2_value <= x)
    {
        int fsr2_state = y;
    }
    else
    {
        fsr2_state = z;
    }
    // If fsr1 = 0 && fsr2 = 0, bed_state = 0
    if (fsr1 == 0 && fsr2 == 0)
    {
        int bed_state = 0;
    }
    else
    {
        int bed_state = 1;
    }
    return bed_state;

}

int get_us_state()
{
    // Read analog values from input pins (serial.read())
    int usds_value = adc1_get_raw(adc1_channel_6); // GPIO34

    // us_voltage to us_dist conversion
    int usds_dist = 5 * usds_value; // conversion supplied on datasheet for nominal bit width

    printf("usds dist = %d\n", usds_dist);

    // Compare to conditional threshold values
    if (usds_dist <= x && usds_dist >= y)
    {
        int us_state = 1;
    }
    else if (usds_dist <= z)
    {
        us_state = 2 // error
    }
    else
    {
        us_state = 0;
    }

    return (0);
}

// calibration routine

// truth table - system state

// set bed state

// set us state

// action required


int main(void)
{
    int usds_current;
    int usds_prev;
    int bed_current;
    int bed_prev;
    int cancel = /* GPIO pin*/;
    char system_state = 0;
    char state = 0;

    // calibration: set system prev

   while (/* calibration pin set*/)
   {
        if (get_bed_state() == 0 && get_us_state() == 0;)
        {
            bed_prev = 0;
            usds_prev = 0;
            break;
        }
        else
        {
            printf("Recalibration required");
        }
    }

    while (1)
    {
        if(bed_prev == 1){state |= BED_PREV_MASK};
        else{system_state &= ~BED_PREV_MASK};

        if(usds_prev == 1){state |= USDS_PREV_MASK};
        else{system_state &= ~USDS_PREV_MASK};

        if(get_bed_state()){state |= BED_CURR_MASK};
        else {state &= ~BED_CURR_MASK};

        if(get_usds_state()){state |= BED_CURR_MASK};
        else {state &= ~BED_CURR_MASK};

        // if-else - truth table conditional program
        if (state&(BED_CURR_MASK | BED_PREV_MASK | USDS_CURR_MASK | USDS_PREV_MASK) == 0) // system = [ 0 0 0 0 ]
        {
            system_state = 0; // "reset_Default"; // no sensors triggered. Default case.
        }
        else if (state&(BED_CURR_MASK | BED_PREV_MASK | USDS_CURR_MASK | USDS_PREV_MASK) == 3) // system = [0 0 1 1 ]
        {
            system_state = 1; //"PUE"; // usds triggered but system empty. 'PUE' = Possible User Error
        }
        else if (state&(BED_CURR_MASK | BED_PREV_MASK | USDS_CURR_MASK | USDS_PREV_MASK) == 4) // system = [ 0 1 0 0 ]
        {
            system_state = 2; // "fall_Out"; // system vacated, no change in usds
        }
        else if (state&(BED_CURR_MASK | BED_PREV_MASK | USDS_CURR_MASK | USDS_PREV_MASK) == 7) // system = [ 0 1 1 1 ]
        {
            system_state = 3; // "sit_Exit";
        }
        else if (state&(BED_CURR_MASK | BED_PREV_MASK | USDS_CURR_MASK | USDS_PREV_MASK) == 13) // system = [ 1 1 0 1 ]
        {
            system_state = 4; //"bed_Occupied";
        }
        else if (state&(BED_CURR_MASK | BED_PREV_MASK | USDS_CURR_MASK | USDS_PREV_MASK) == 11) // system = [ 1 0 1 1 ]
        {
            system_state = 5; //"bed_Enter";
        }
        else if (state&(BED_CURR_MASK | BED_PREV_MASK | USDS_CURR_MASK | USDS_PREV_MASK) == 15) // system = [ 1 1 1 1 ]
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
        }*/

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
            // send warning - set warning pin connected to VitalCare chip
            break;
        case 3: //"sit_Exit":
            printf("Warning: Sit-Up Exit\n");
            // send warning
            break;
        case 4: //"bed_Occupied":
            printf("Bed Occupied\n");
            break;
        case 5: //"bed_Enter" : 
            printf("Bed Entered\n");
            break;
        case 6: //"sit_Warning":
            printf("Warning: Sit-Up, Possible Exit");
            break;
        case 7: //"usds_Covered":
            printf("Distance Sensor Covered");
            break;
        case 8: //"error":
            printf("System State Error");
            break;
        }

        if (cancel == 1)
        {
            // send cancel - set cancel pin connected to VitalCare chip
            // disable warning pin
        }

        bed_prev = bed_current; // store previous state for next loop
        usds_prev = usds_current;

        vTaskDelay(1000 / portTICK_PERIOD_MS); // delay
    }
    return (0);
}
