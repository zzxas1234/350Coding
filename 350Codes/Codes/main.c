#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "driver/dac.h"
#include "esp_adc_cal.h"
#include "time.h"

#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   64          //Multisampling

static esp_adc_cal_characteristics_t *adc_chars;

static const adc_channel_t fsr1Channel = ADC1_CHANNEL_4;            //Pin32
static const adc_channel_t fsr2Channel = ADC1_CHANNEL_5;            //Pin33
static const adc_channel_t utsChannel = ADC1_CHANNEL_6;             //PinA2
static const adc_channel_t calibrationChannel = ADC1_CHANNEL_3;     //PinA3

static const adc_atten_t atten = ADC_ATTEN_DB_0;
static const adc_unit_t unit = ADC_UNIT_1;

static int fsr1Value;
static int fsr2Value;
static int utsValue;
static int calibrationValue[2];

static  int fsr1MaxValue;
static  int fsr2MaxValue;
static  int utsMaxValue;

static  int currentFsr1Value;
static  int currentFsr2Value;
static  int currentUtsValue;

static  int calibrationBotton;

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
        uint32_t fsr2_reading = 0;
        //Multisampling
    for (int i = 0; i < NO_OF_SAMPLES; i++) 
    {
        if (unit == ADC_UNIT_1) {
            fsr2_reading += adc1_get_raw((adc1_channel_t)fsr2Channel);
        } 
        else 
        {
            int raw;
            adc2_get_raw((adc2_channel_t)fsr2Channel, ADC_WIDTH_BIT_12, &raw);
            fsr2_reading += raw;
        }
    }
    fsr2_reading /= NO_OF_SAMPLES;
    //Convert adc_reading to voltage in mV
    uint32_t voltage = esp_adc_cal_raw_to_voltage(fsr2_reading, adc_chars);
    printf("FSR2 Reading --> Raw: %d\tVoltage: %dmV\n", fsr2_reading, voltage);
    vTaskDelay(pdMS_TO_TICKS(1000));
    fsr2Value = (int)fsr2_reading;
    }

static void utsRead(void)
    {
        uint32_t uts_reading = 0;
        //Multisampling
    for (int i = 0; i < NO_OF_SAMPLES; i++) 
    {
        if (unit == ADC_UNIT_1) {
            uts_reading += adc1_get_raw((adc1_channel_t)utsChannel);
        } 
        else 
        {
            int raw;
            adc2_get_raw((adc2_channel_t)utsChannel, ADC_WIDTH_BIT_12, &raw);
            uts_reading += raw;
        }
    }
    uts_reading /= NO_OF_SAMPLES;
    //Convert adc_reading to voltage in mV
    uint32_t voltage = esp_adc_cal_raw_to_voltage(uts_reading, adc_chars);
    printf("FSR1 Reading --> Raw: %d\tVoltage: %dmV\n", uts_reading, voltage);
    vTaskDelay(pdMS_TO_TICKS(1000));
    utsValue = (int)uts_reading;
    }

static void calibrationBottonInputCapture(void)
{
    
    int calibrationTemp;
    {
        uint32_t calibration_Reading = 0;
        //Multisampling
    for (int i = 0; i < NO_OF_SAMPLES; i++) 
    {
        if (unit == ADC_UNIT_1) {
            calibration_Reading += adc1_get_raw((adc1_channel_t)calibrationChannel);
        } 
        else 
        {
            int raw;
            adc2_get_raw((adc2_channel_t)calibrationChannel, ADC_WIDTH_BIT_12, &raw);
            calibration_Reading += raw;
        }
    }
    calibration_Reading /= NO_OF_SAMPLES;
    //Convert adc_reading to voltage in mV
    uint32_t voltage = esp_adc_cal_raw_to_voltage(calibration_Reading, adc_chars);
    printf("FSR1 Reading --> Raw: %d\tVoltage: %dmV\n", calibration_Reading, voltage);
    vTaskDelay(pdMS_TO_TICKS(1000));
    calibrationTemp = (int)calibration_Reading;
    //botton is pressed or not ?
    //1=old value 2=new value
    calibrationValue[1] = calibrationValue[2];

        if (calibrationTemp > 0)
        {
            calibrationValue[2] = true;
        }
        else
        {
            calibrationValue[2] = false;
        }
        
        if (calibrationValue[1] == 0 && calibrationValue[2] == 1)
        {
            calibration();
        }
        else{}
    }
}

static void calibration(void)
    {
//program = 1 ----> check if the botton hold for 5 second
//program = 2 ----> getting data from all sensors
//program = 3 ----> check if the botton hold for 5 second
    int holdingTime = 5;//hold botton for 5 second
    int timer = 0;
    bool calibrationStart = true;
    bool timerStart = false;
    while (calibrationStart)
    {
        if (calibrationValue[1] == 0 && calibrationValue[2] == 0)
        {
            calibrationBottonInputCapture();
        }

        else if (calibrationValue[1] == 0 && calibrationValue[2] == 1)
        {
            calibrationBottonInputCapture();
        }

        else if (calibrationValue[1] == 1 && calibrationValue[2] == 0)
        {
            calibrationBottonInputCapture();
        }

        else if (calibrationValue[1] == 1 && calibrationValue[2] == 1)
        {
            calibrationBottonInputCapture();
            timer++;
        }
            if ((timer/portTICK_PERIOD_MS) >= holdingTime)
            {
                //turn LED on >>>go to update value function
                calibrationUpdateValue()
            }
    }

    

    }
//usinng adc1 for switch>>A1
static void calibrationUpdateValue(void)
{
    fsr1MaxValue = ((fsr1Value >= fsr1MaxValue) == 1)? fsr1Value:fsr1MaxValue;
    fsr2MaxValue = ((fsr2Value >= fsr2MaxValue) == 1)? fsr2Value:fsr2MaxValue;
}




void app_main(void)
{
        //Check if Two Point or Vref are burned into eFuse
    check_efuse();
    while(1)
    {
        fsr1Read();
        fsr2Read();
        utsRead();
    }
    
}

