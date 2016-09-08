#include <avr/io.h>

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

volatile int decimal_place = 0;
volatile int decimal_direction = 1;

void set_digit(int digit_number, int digit_value) {
  // Set the clock low to start with.
  CLOCK_LOW();

  // Also make sure we're not latched.
  LATCH_LOW();

  // Shift out the digit value data first.
  for (int i = 7; i >= 0; --i) {
    if (i == 7 && digit_number == decimal_place) {
      DATA_HIGH();
    } else {
    // Reinterpret our digit struct as an int to pull out each segment's bit.
    (*((uint8_t*) digits[digit_value]) & (1 << i)) ? DATA_HIGH() : DATA_LOW();
    }
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

volatile int time[6] = {0, 0, 0, 0, 0, 0};

int main(void) {
  // Make the serial control ports all output pins.
  DDRB |= 1 << SDA_PIN;
  DDRB |= 1 << CLK_PIN;
  DDRB |= 1 << SDL_PIN;

  int dc = 0;

  while (1) {
    // Loop through and display all the digits.
    for (int i = 0; i < 6; ++i) {
      set_digit(i, time[i]);
    }

    if (dc % 50 == 0) {
    ++time[5];
    }
    if (time[5] > 9) {
      time[5] = 0;
      ++time[4];
    }
    if (time[4] > 9) {
      time[4] = 0;
      ++time[3];
    }
    if (time[3] > 9) {
      time[3] = 0;
      ++time[2];
    }
    if (time[2] > 9) {
      time[2] = 0;
      ++time[1];
    }
    if (time[1] > 9) {
      time[1] = 0;
      ++time[0];
    }

    if (time[0] > 9) {
      time[0] = 0;
    }

    if (++dc > 500) {
      decimal_place += decimal_direction;
      dc = 0;
      if (decimal_place >= 5) {
        decimal_place = 5;
        decimal_direction = -1;
      }
      if (decimal_place <= 0) {
        decimal_place = 0;
        decimal_direction = 1;
      }
    }
  }

  return 0;
}
