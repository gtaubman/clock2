#include <avr/io.h>
#include "usitwislave.h"

typedef struct digit {
  unsigned int a : 1;
  unsigned int b : 1;
  unsigned int c : 1;
  unsigned int d : 1;
  unsigned int e : 1;
  unsigned int f : 1;
  unsigned int g : 1;
  unsigned int dp : 1;
} Digit;

// Digit segment configuration for each digit.
Digit zero =  {1, 1, 1, 1, 1, 1, 0, 0};
Digit one =   {0, 1, 1, 0, 0, 0, 0, 0};
Digit two =   {1, 1, 0, 1, 1, 0, 1, 0};
Digit three = {1, 1, 1, 1, 0, 0, 1, 0};
Digit four =  {0, 1, 1, 0, 0, 1, 1, 0};
Digit five =  {1, 0, 1, 1, 0, 1, 1, 0};
Digit six =   {0, 0, 1, 1, 1, 1, 1, 0};
Digit seven = {1, 1, 1, 0, 0, 0, 0, 0};
Digit eight = {1, 1, 1, 1, 1, 1, 1, 0};
Digit nine =  {1, 1, 1, 0, 0, 1, 1, 0};
Digit clear = {0, 0, 0, 0, 0, 0, 0, 0};

// A lookup table from actual integer to 7-segment display integer.
Digit* digits[11] = {
  &zero,
  &one,
  &two,
  &three,
  &four,
  &five,
  &six,
  &seven,
  &eight,
  &nine,
  &clear,
};

// Ports for serial clock, data, and latch.
#define SDA_PORT PORTB
#define CLK_PORT PORTB
#define SDL_PORT PORTB

// Pins for serial clock, data, and latch.
#define SDA_PIN 3
#define CLK_PIN 1
#define SDL_PIN 4

#define CLOCK_LOW() (CLK_PORT &= ~(1 << CLK_PIN))
#define CLOCK_HIGH() (CLK_PORT |= 1 << CLK_PIN)

#define DATA_LOW() (SDA_PORT &= ~(1 << SDA_PIN))
#define DATA_HIGH() (SDA_PORT |= 1 << SDA_PIN)

#define LATCH_LOW() (SDL_PORT &= ~(1 << SDL_PIN))
#define LATCH_HIGH() (SDL_PORT |= 1 << SDL_PIN)

volatile int time[6] = {0, 0, 0, 0, 0, 0};
volatile uint8_t decimal_places = 0;

void set_digit(int digit_number, int digit_value) {
  // Set the clock low to start with.
  CLOCK_LOW();

  // Also make sure we're not latched.
  LATCH_LOW();

  // Shift out the digit value data first.
  for (int i = 7; i >= 0; --i) {
    // TODO(gtaubman): The first bit out is whether or not the decimal place
    // should be active.

    // Reinterpret our digit struct as an int to pull out each segment's bit.
    (*((uint8_t*) digits[digit_value]) & (1 << i)) ? DATA_HIGH() : DATA_LOW();
    CLOCK_HIGH();
    CLOCK_LOW();
  }

  // Shift out the digit selector.
  for (int i = 7; i >= 0; --i) {
    digit_value < 0 ? DATA_LOW() : (i == digit_number) ? DATA_HIGH() : DATA_LOW();
    CLOCK_HIGH();
    CLOCK_LOW();
  }

  LATCH_HIGH();
  LATCH_LOW();
}

void idle_callback() {
  // Loop through and display all the digits.
  for (int i = 0; i < 6; ++i) {
    set_digit(i, time[i]);
  }
}

void data_callback(uint8_t input_buffer_length, const uint8_t* input_buffer,
                   uint8_t *output_buffer_length, uint8_t *output_buffer) {
  *output_buffer_length = 0;

  // Read at most 6 values from the i2c transaction.
  for (int i = 0; i < 6 && i < input_buffer_length; ++i) {
    time[i] = input_buffer[i];
  }

  // TODO(gtaubman): Also read an optional 7th bit containing a bit mask of
  // where the decimal points should go.
}

int main(void) {
  // Make the serial control ports all output pins.  use_twi_slave() will set up
  // the directions for the i2c pins appropriately so there's no need to do that
  // here.
  DDRB |= 1 << SDA_PIN;
  DDRB |= 1 << CLK_PIN;
  DDRB |= 1 << SDL_PIN;

  const uint8_t slave_address = 27;
  const uint8_t use_sleep = 0;

  usi_twi_slave(slave_address, use_sleep, &data_callback, &idle_callback);

  return 0;
}
