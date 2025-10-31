#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/timer.h"

#define LED_PIN 2 

void IRAM_ATTR timer_isr(void *arg) {
    gpio_set_level(LED_PIN, !gpio_get_level(LED_PIN)); // i hope thats how it works sob
    TIMERG0.int_clr_timers.t0 = 1;                     
}

void app_main(void) {
    // setup LED pin
    gpio_pad_select_gpio(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    // configure timer 0
    timer_config_t config = {
        .divider = 80,       // 80 MHz / 80 = 1 MHz timer
        .counter_dir = TIMER_COUNT_UP,
        .counter_en = TIMER_PAUSE,
        .alarm_en = TIMER_ALARM_EN,
        .auto_reload = true
    };
    timer_init(TIMER_GROUP_0, TIMER_0, &config);
    timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, 1000000); // 1 second
    timer_enable_intr(TIMER_GROUP_0, TIMER_0);
    timer_isr_register(TIMER_GROUP_0, TIMER_0, timer_isr, NULL, ESP_INTR_FLAG_IRAM, NULL);
    timer_start(TIMER_GROUP_0, TIMER_0);

    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS); // optional, main loop idle
    }
}

