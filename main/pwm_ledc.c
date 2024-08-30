

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "driver/ledc.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define PWM_PIN_LED         5
#define PWM_DUTY_RESOLUTION 16

const char *TAG = "PWM"; 

void duty_att(void);

void app_main(void)
{   
    ledc_timer_config_t timer_config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .clk_cfg = LEDC_APB_CLK,
        .duty_resolution = PWM_DUTY_RESOLUTION,
        .freq_hz = 60
    };
    ledc_timer_config(&timer_config);

    ledc_channel_config_t channel_config = {
        .channel = LEDC_CHANNEL_0,
        .gpio_num = PWM_PIN_LED,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_sel = LEDC_TIMER_0
    };

    ledc_channel_config(&channel_config);

    xTaskCreatePinnedToCore(duty_att, "Duty Cycle Att", 3000, NULL, 2, NULL, 0);


    while(true){
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void duty_att(void){
    int DC = 0;
    while(true){
        while(DC < pow(2, PWM_DUTY_RESOLUTION)){
            DC = DC + 1000;
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, (uint32_t)DC);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
            
            ESP_LOGW(TAG, "DUTY CYCLE: %lu", ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
            vTaskDelay(pdMS_TO_TICKS(1000));        
        }
        DC = 0;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    vTaskDelay(pdMS_TO_TICKS(50));
}