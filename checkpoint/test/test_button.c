/**
 * @author Noe Maillet & Mathéo Mercier
 * @file test_button.c
 * This file is made to test the emergency button.
*/

#include "../lib/WiringPi/wiringPi/wiringPi.h"

#define EMERGENCY_BUTTON_PIN 5 // Wiring pi notation pin number

#include <stdio.h>

int main(int argc, char* argv[])
{
    int prev_state = 1;

    // Init emergency button and wiringPi
    wiringPiSetup();
    pinMode(EMERGENCY_BUTTON_PIN, INPUT);

    while (1) {
        if (digitalRead(EMERGENCY_BUTTON_PIN) == LOW) {
            prev_state = 0;
        }
        if (digitalRead(EMERGENCY_BUTTON_PIN) == HIGH && prev_state == 0) {
            prev_state = 1;
            printf("Button Pressed\n");
        }
    }
    return 0;
}
