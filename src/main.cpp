/*
 * Firmware for the Zest project
 *
 * This firmware is for the ATTINY85 microcontroller
 * It takes an input square wave and mirrors it on the output,
 * minus any pulses that are <53us or so.
 * A side effect of this filtering is that all output pulses
 * begin 53us after the corresponding input pulse (i.e.: they are 53us shorter).
 *
 * see also https://github.com/rahji/50us_filter
 *
 * See included GPL v3 license
 * August 11, 2023 Rob Duarte github.com/rahji
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

// ATTINY85 pinout
// Pin 2 (PB3) is the LED pin
// Pin 3 (PB4) is the output pin
// Pin 7 (PB2) is the input pin (INT0)

#define INPUT_PIN PB2
#define OUTPUT_PIN PB4
#define LED_PIN PB3

// When the input pin changes, this interrupt is triggered
ISR(INT0_vect)
{
    // if this is a rising edge, start the 53us timer
    if (PINB & (1 << INPUT_PIN)) // input pin is high
    {
        TCCR1 |= (1 << CS10); // start timer with no prescaler
    }
    else
    {
        // it's a falling edge, so mirror this end-of-pulse on the output pin and the LED pin
        PORTB &= ~((1 << OUTPUT_PIN) | (1 << LED_PIN));
    }
}

// When the timer reaches 53us, this interrupt is triggered
ISR(TIMER1_COMPA_vect)
{
    // if the input pin is still high that means it wasn't a <53us pulse
    if (PINB & (1 << INPUT_PIN))
    {
        // so set the output pin and LED pins high
        PORTB |= (1 << OUTPUT_PIN) | (1 << LED_PIN);
    }

    // stop the timer. it will get restarted when the next pulse comes in
    TCCR1 &= ~(1 << CS10);
}

int main()
{
    // Set the input and output pins
    DDRB &= ~(1 << INPUT_PIN);                  // Set input pin as input
    DDRB |= (1 << LED_PIN) | (1 << OUTPUT_PIN); // set LED_PIN and OUTPUT_PIN as output

    // Set up timer 1 for a 53 microsecond period
    TCCR1 |= (1 << CTC1);  // CTC mode; timer stopped since CS10=0
    OCR1A = 211 - 1;       // 210 ticks / 8 MHz = 52.5 microsecond period (I think)
    TIMSK = (1 << OCIE1A); // Enable the timer compare match interrupt

    // Enable the interrupt for the input pin
    GIMSK |= (1 << PCIE);   // Enable pin change interrupts
    PCMSK |= (1 << PCINT0); // Enable INT0 interrupt

    // Enable global interrupts
    sei();

    // zzz (the interrupts take care of everything)
    sleep_enable();
    while (1)
    {
        sleep_cpu();
    }

    return 0; // never reached
}
