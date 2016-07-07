#include <avr/io.h>
#include <util/delay.h>

int main(void) {
  DDRB = 1 << 3;
  for(;;){
    for(int i = 0; i < 10; i++){
      _delay_ms(30);
    }
    PORTB ^= 1 << 3;
  }
  return 0;
}
