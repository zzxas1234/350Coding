/* ADC1 Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   64          //Multisampling

static esp_adc_cal_characteristics_t *adc_chars;

static const adc_channel_t fsr1Channel = ADC1_CHANNEL_4;    //Pin32
static const adc_channel_t fsr2Channel = ADC1_CHANNEL_5;    //Pin33
static const adc_channel_t utsChannel = ADC1_CHANNEL_6;     //PinA2

static const adc_atten_t atten = ADC_ATTEN_DB_0;
static const adc_unit_t unit = ADC_UNIT_1;

static int fsr1Value;
static int fsr2Value;
static int utsValue;

static void systemInitialise(void)
{
    //Configure ADC
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(fsr1Channel, atten);
    adc1_config_channel_atten(fsr2Channel, atten);
    adc1_config_channel_atten(utsChannel, atten);

    //Characterize ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
    print_char_val_type(val_type);
}

static void check_efuse(void)
{
    //Check TP is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
        printf("eFuse Two Point: Supported\n");
    } else {
        printf("eFuse Two Point: NOT supported\n");
    }
    //Check Vref is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK) {
        printf("eFuse Vref: Supported\n");
    } else {
        printf("eFuse Vref: NOT supported\n");
    }
}

static void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        printf("Characterized using Two Point Value\n");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        printf("Characterized using eFuse Vref\n");
    } else {
        printf("Characterized using Default Vref\n");
    }
}

static void fsr1Read(void)
{
    uint32_t fsr1_reading = 0;
        //Multisampling
    for (int i = 0; i < NO_OF_SAMPLES; i++) 
    {
        if (unit == ADC_UNIT_1) {
            fsr1_reading += adc1_get_raw((adc1_channel_t)fsr1Channel);
        } 
        else 
        {
            int raw;
            adc2_get_raw((adc2_channel_t)fsr1Channel, ADC_WIDTH_BIT_12, &raw);
            fsr1_reading += raw;
        }
    }
    fsr1_reading /= NO_OF_SAMPLES;
    //Convert adc_reading to voltage in mV
    uint32_t voltage = esp_adc_cal_raw_to_voltage(fsr1_reading, adc_chars);
    printf("FSR1 Reading --> Raw: %d\tVoltage: %dmV\n", fsr1_reading, voltage);
    vTaskDelay(pdMS_TO_TICKS(1000));
    fsr1Value = (int)fsr1_reading;

}
static void fsr2Read(void)
    {

    }
static void utsRead(void)
    {

    }

void app_main(void)
    {
    
    }

