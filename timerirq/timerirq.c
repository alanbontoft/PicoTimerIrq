#include "pico/stdlib.h"

// The LED is connected to GPIO 25
#define LED_PIN 25

// Alarms on GPIO pins 2-5
#define ALARM0_PIN 2
#define ALARM1_PIN 3
#define ALARM2_PIN 4
#define ALARM3_PIN 5

bool alarm_state[4] = {false, false, false, false};
struct repeating_timer timers[4];
bool led_state = false;
int index;
uint pin;


// Timer ISR
// Alarm 0 has ID 1, 1 has ID 2, etc.
bool repeating_timer_callback(struct repeating_timer *t)
{
    index = t->alarm_id-1;

    // select pin to toggle
    switch (index)
    {
        case 0: pin = ALARM0_PIN;
                break;
        case 1: pin = ALARM1_PIN;
                break;
        case 2: pin = ALARM2_PIN;
                break;
        case 3: pin = ALARM3_PIN;
                break;
        
        default:
            break;
    }

    // set pin level
    gpio_put(pin, alarm_state[index]);

    // toggle state
    alarm_state[index] = !alarm_state[index];

    // return true to keep repeating
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

    // Configure the LED and ALARM pins as output
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
    // x us later regardless of how long the callback took to execute
    add_repeating_timer_us(-100, repeating_timer_callback, NULL, &timers[0]);
    add_repeating_timer_us(-200, repeating_timer_callback, NULL, &timers[1]);
    add_repeating_timer_us(-400, repeating_timer_callback, NULL, &timers[2]);
    add_repeating_timer_us(-800, repeating_timer_callback, NULL, &timers[3]);


    // Loop to flash LED to shown running
    while (true)
    {
        gpio_put(LED_PIN, led_state);
        led_state = !led_state;
        sleep_ms(500);
    }

    return 0;
}
