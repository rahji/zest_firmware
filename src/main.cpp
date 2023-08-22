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
        TCCR0B |= (1 << CS01); // start timer (with /8 prescaler
    }
    else
    {
        // it's a falling edge, so mirror this end-of-pulse on the output pin and the LED pin
        PORTB &= ~((1 << OUTPUT_PIN) | (1 << LED_PIN));
    }
}

// When the timer reaches 53us, this interrupt is triggered
ISR(TIMER0_COMPA_vect)
{
    // if the input pin is still high that means it wasn't a <53us pulse
    if (PINB & (1 << INPUT_PIN))
    {
        // so set the output pin and LED pins high
        PORTB |= (1 << OUTPUT_PIN) | (1 << LED_PIN);
    }

    // stop the timer. it will get restarted when the next pulse comes in
    TCCR0B &= ~(1 << CS01); // clearing the CS01 bit makes CS0[2:0] = 000, which stops the timer
}

int main()
{
    // Set the input and output pins
    DDRB &= ~(1 << INPUT_PIN);                  // Set input pin as input
    PORTB |= (1 << INPUT_PIN);                  // Set input pin pullup resistor
    DDRB |= (1 << LED_PIN) | (1 << OUTPUT_PIN); // set LED_PIN and OUTPUT_PIN as output

    // clear all the timer registers
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0 = 0;

    // Set up timer 0 for a 53 microsecond period
    OCR0A = 53 - 1; // 53 cycles = 53us since the clock is effectively 1MHz (8MHz/8 prescaler)
    TCCR0A |= (1 << WGM01);  // CTC mode
    // TCCR0B |= (1 << CS01);   // prescaler = clock/8, BUT DON'T START THE TIMER YET
    TIMSK  |= (1 << OCIE0A); // Enable the timer compare match interrupt

    // Enable the interrupt for the input pin
    GIMSK |= (1 << INT0);   // Enable pin change interrupts
    MCUCR |= (1 << ISC00);  // Trigger interrupt on any logical change

    // Enable global interrupts
    sei();

    while (1)
    {
    }

    return 0; // never reached
}
