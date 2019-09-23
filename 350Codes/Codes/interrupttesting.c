/* GPIO Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_adc_cal.h"

/**
 * Brief:
 * This test code shows how to configure gpio and how to use gpio interrupt.
 *
 * GPIO status:
 * GPIO18: output
 * GPIO19: output
 * GPIO4:  input, pulled up, interrupt from rising edge and falling edge
 * GPIO5:  input, pulled up, interrupt from rising edge.
 *
 * Test:
 * Connect GPIO18 with GPIO4
 * Connect GPIO19 with GPIO5
 * Generate pulses on GPIO18/19, that triggers interrupt on GPIO4/5
 *
 */

#define GPIO_INPUT_IO_0    16
#define GPIO_INPUT_PIN_SEL  (1ULL<<GPIO_INPUT_IO_0) 
#define ESP_INTR_FLAG_DEFAULT 0

#define GPIO_OUTPUT_IO_0 17 //input into interrupt
#define GPIO_OUTPUT_PIN_SEL  (1ULL<<GPIO_OUTPUT_IO_0) 

//interrupt triggered > link to the task group xQueueSenndFromISR
static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    printf("interrupt is triggered YAY!!!!");
}


void init_gpio(void) {
    gpio_config_t io_conf;

    //interrupt of falling edge
    io_conf.intr_type = GPIO_PIN_INTR_NEGEDGE;
    //bit mask of the pins
    io_conf.pin_bit_mask = (1<<GPIO_INPUT_IO_0);
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    io_conf.pull_down_en = 0;
    gpio_config(&io_conf);

    //install gpio isr service
    gpio_install_isr_service(0); // no flags
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_IO_0);

        //change gpio intrrupt type for one pin
    gpio_set_intr_type(GPIO_INPUT_IO_0, GPIO_INTR_ANYEDGE);

}


void app_main(void)
{
    int cnt = 0;
    while(1) {
        printf("cnt: %d\n", cnt++);
        vTaskDelay(1000 / portTICK_RATE_MS);
        gpio_set_level(GPIO_OUTPUT_IO_0, cnt % 2);
    }
}