/**
 * V. Hunter Adams (vha3@cornell.edu)
 */

#include "pico/stdlib.h"

// The LED is connected to GPIO 25
#define LED_PIN 25
#define ALARM0_PIN 2
#define ALARM1_PIN 3
#define ALARM2_PIN 4
#define ALARM3_PIN 5

bool alarm_state[4] = {false, false, false,false};
struct repeating_timer timers[4];
bool led_state = false;

// Timer0 ISR
bool repeating_timer0_callback(struct repeating_timer *t)
 {
    gpio_put(ALARM0_PIN, alarm_state[0]);

    alarm_state[0] = !alarm_state[0];

    return true;
}

// Timer1 ISR
bool repeating_timer1_callback(struct repeating_timer *t)
 {
    gpio_put(ALARM1_PIN, alarm_state[1]);

    alarm_state[1] = !alarm_state[1];

    return true;
}

// Timer2 ISR
bool repeating_timer2_callback(struct repeating_timer *t)
 {
    gpio_put(ALARM2_PIN, alarm_state[2]);

    alarm_state[2] = !alarm_state[2];

    return true;
}


// Timer ISR
bool repeating_timer3_callback(struct repeating_timer *t)
 {
    gpio_put(ALARM3_PIN, alarm_state[3]);

    alarm_state[3] = !alarm_state[3];

    return true;
}

// Main (runs on core 0)
int main()
{

    
     // Initialize the gpio pins
    gpio_init(LED_PIN);
    gpio_init(ALARM0_PIN);
    gpio_init(ALARM1_PIN);
    gpio_init(ALARM2_PIN);
    gpio_init(ALARM3_PIN);

    // Configure the LED and ALARM pins as an output
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_set_dir(ALARM0_PIN, GPIO_OUT);
    gpio_set_dir(ALARM1_PIN, GPIO_OUT);
    gpio_set_dir(ALARM2_PIN, GPIO_OUT);
    gpio_set_dir(ALARM3_PIN, GPIO_OUT);
    
    // Create a repeating timer that calls repeating_timer_callback.
    // If the delay is > 0 then this is the delay between the previous callback ending and the next starting.
    // If the delay is negative (see below) then the next call to the callback will be exactly x us after the
    // start of the call to the last callback



    // Negative delay so means we will call repeating_timer_callback, and call it again
    // 25us (40kHz) later regardless of how long the callback took to execute
    add_repeating_timer_us(-100, repeating_timer0_callback, NULL, &timers[0]);
    add_repeating_timer_us(-200, repeating_timer1_callback, NULL, &timers[1]);
    add_repeating_timer_us(-400, repeating_timer2_callback, NULL, &timers[2]);
    add_repeating_timer_us(-800, repeating_timer3_callback, NULL, &timers[3]);


    // Loop
    while (true)
    {
        gpio_put(LED_PIN, led_state);
        led_state = !led_state;
        sleep_ms(500);
    }

    return 0;
}
