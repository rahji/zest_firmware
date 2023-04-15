/*
 * firmware for the zestshield project
 * see also https://github.com/rahji/50us_filter
 * 
 * See included GPL v3 license
 * April 15, 2023 Rob Duarte github.com/rahji
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define INPUT_PIN 2
#define OUTPUT_PIN 3
#define MIN_PULSE_WIDTH 53

// When the input pin changes, this interrupt is triggered
ISR(INT0_vect)
{
  // if this is a rising edge, start the 53us timer
  if (PIND & (1 << INPUT_PIN)) // input pin is high
  {
    TCCR1B |= (1 << CS10); // start timer with no prescaler
  } else {
    // it's a falling edge, so mirror this end-of-pulse on the output pin
    PORTD &= ~(1 << OUTPUT_PIN);
  }
}

// When the timer reaches 53us, this interrupt is triggered
ISR(TIMER1_COMPA_vect)
{
  // if the input pin is still high that means it wasn't a <53us pulse
  if (PIND & (1 << INPUT_PIN))
  {
    // so set the output pin high
    PORTD |= (1 << OUTPUT_PIN);
  }

  // stop the timer. it will get restarted when the next pulse comes in
  TCCR1B &= ~(1 << CS10);
}

int main()
{
  // Set the input and output pins
  DDRD &= ~(1 << INPUT_PIN); // Set input pin as input
  DDRD |= (1 << OUTPUT_PIN); // Set output pin as output

  // Set up timer 1 for a 53 microsecond period
  TCCR1A = 0;
  TCCR1B = (1 << WGM12); // CTC mode, no prescaler (timer/counter stopped)
  OCR1A = 832-1; // 832 ticks / 16 MHz = 53 microseconds
  TIMSK1 = (1 << OCIE1A); // Enable the timer interrupt

  // Enable the interrupt for the input pin
  EICRA |= (1 << ISC00); // Set INT0 to trigger on any change
  EIMSK |= (1 << INT0);  // Enable INT0 interrupt

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
