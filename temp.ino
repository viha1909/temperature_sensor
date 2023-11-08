#include <avr/io.h>
#include <avr/interrupt.h>

#define LED_PIN 13
#define LED 10


#define PIN A0
int raw=1;
int temp =0;

volatile bool ledState = false;
volatile unsigned long interval = 500; // Initial interval set to 500ms

void setup() 
{
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);

  // Set up Timer 1
  cli(); // Disable global interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  pinMode(LED,OUTPUT);
  OCR1A = 15624; // 500ms at 16 MHz

  TCCR1B |= (1 << WGM12); // Configure timer 1 for CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10); // Set prescaler to 1024

  TIMSK1 |= (1 << OCIE1A); // Enable timer compare interrupt
  sei(); // Enable global interrupts
}

void loop() 
{
  raw=analogRead(PIN);
  temp= raw*4.88;
  temp=temp/10;
  if (temp <=30) {
    interval = 250; // Set interval to 250ms
  } else {
    interval = 500; // Set interval to 500ms
  }
}

ISR(TIMER1_COMPA_vect)
{
  if (ledState) {
    ledState = false;
    digitalWrite(LED_PIN, LOW); // Turn off the LED
    digitalWrite(LED, LOW);
  } else {
    ledState = true;
    digitalWrite(LED_PIN, HIGH); // Turn on the LED
    digitalWrite(LED, HIGH);
  }

  // Update OCR1A to change the interval dynamically
  OCR1A = (int)(interval * 2); // 2 because the prescaler is set to 1024
}