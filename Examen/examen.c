#include <avr/io.h>
#include "./inputs.h"
#define F_CPU 16000000UL
#include <util/delay.h>

// Etiquetas
// Inputs
#define BTNS_DDRX DDRC
#define BTNS_PINX PINC
#define BTNS_PORTX PORTC
#define BTNS_0 PINC2
#define BTNS_1 PINC3
#define BTNS_2 PINC4

#define MOTOR_0 PORTC0
#define MOTOR_1 PORTC1

// Macros
#define BTNS_0_READ (BTNS_PINX & (1 << BTNS_0))
#define BTNS_1_READ (BTNS_PINX & (1 << BTNS_1))
#define BTNS_2_READ (BTNS_PINX & (1 << BTNS_2))

// Outputs
#define DISPLAY_DDRX DDRD
#define DISPLAY_PORTX PORTD
#define DISPLAY_1 PORTD0
#define DISPLAY_2 PORTD1
#define DISPLAY_3 PORTD2
#define DISPLAY_4 PORTD3
#define DISPLAY_5 PORTD4
#define DISPLAY_6 PORTD5
#define DISPLAY_7 PORTD6

#define LEDS_DDRX DDRB
#define LEDS_PORTX PORTB
#define LEDS_1 PORTB0
#define LEDS_2 PORTB1
#define LEDS_3 PORTB2
#define LEDS_4 PORTB3

// Macros
#define LEDS_1_ON LEDS_PORTX |= (1 << LEDS_1)
#define LEDS_1_OFF LEDS_PORTX &= ~(1 << LEDS_1)

// Declaración de funciones
// botones
void btns_init(void);

// leds
void leds_init(void);
void leds_centrales_exteriores(void);
void leds_izquierda_derecha(void);
void leds_derecha_izquierda(void);
void leds_parpadeo(void);
void leds_on(void);
// display
void display_init(void);
void display_E(void);
void display_1(void);
void display_2(void);
void display_P(void);
void display_F(void);
// Constante
#define RETARDO 100

int main(void)
{
  btns_init();
  leds_init();
  while (1)
  { // 0bXXXXXXXX
    //     &
    // 0b00000100
    // 0b00000X00
    if(BTNS_0_READ){
      leds_on();
      continue;
    }
    if (!(BTNS_1_READ) && !(BTNS_0_READ))
    {
      leds_centrales_exteriores();
    }
    else if (!(BTNS_1_READ) && (BTNS_0_READ))
    {
      leds_izquierda_derecha();
    }
    else if ((BTNS_1_READ) && !(BTNS_0_READ))
    {
      leds_derecha_izquierda();
    }
    else if ((BTNS_1_READ) && (BTNS_0_READ))
    {
      leds_parpadeo();
    }
  }
}

void btns_init(void)
{ // 0bXXXXXXXX  //76543210
  //      &
  // 0b11100011
  // 0bXXX000XX
  // BTNS_DDRX = BTNS_DDRX & 0b11100011;
  BTNS_DDRX &= 0b11100011;
}
void leds_init(void)
{ // 76543210
  LEDS_DDRX |= (1 << LEDS_1);
  LEDS_DDRX |= (1 << LEDS_2);
  LEDS_DDRX |= (1 << LEDS_3);
  LEDS_DDRX |= (1 << LEDS_4);
}

void leds_auto_fantastico(void)
{
  // 76543210
  LEDS_PORTX = (1 << LEDS_1);
  _delay_ms(RETARDO);
  LEDS_PORTX = (1 << LEDS_2);
  _delay_ms(RETARDO);
  LEDS_PORTX = (1 << LEDS_3);
  _delay_ms(RETARDO);
  LEDS_PORTX = (1 << LEDS_4);
  _delay_ms(RETARDO);
  LEDS_PORTX = (1 << LEDS_5);
  _delay_ms(RETARDO);
  LEDS_PORTX = (1 << LEDS_6);
  _delay_ms(RETARDO);
  LEDS_PORTX = (1 << LEDS_5);
  _delay_ms(RETARDO);
  LEDS_PORTX = (1 << LEDS_4);
  _delay_ms(RETARDO);
  LEDS_PORTX = (1 << LEDS_3);
  _delay_ms(RETARDO);
  LEDS_PORTX = (1 << LEDS_2);
  _delay_ms(RETARDO);
}
void leds_off(void)
{
  LEDS_PORTX &= ~(1 << LEDS_1);
  LEDS_PORTX &= ~(1 << LEDS_2);
  LEDS_PORTX &= ~(1 << LEDS_3);
  LEDS_PORTX &= ~(1 << LEDS_4);
  LEDS_PORTX &= ~(1 << LEDS_5);
  LEDS_PORTX &= ~(1 << LEDS_6);
}
void leds_free(void)
{
  // 76543210
  LEDS_PORTX |= (1 << LEDS_1); 
  _delay_ms(RETARDO);
  LEDS_PORTX |= (1 << LEDS_2);
  _delay_ms(RETARDO);
  LEDS_PORTX |= (1 << LEDS_3);
  _delay_ms(RETARDO);
  LEDS_PORTX |= (1 << LEDS_4);
  _delay_ms(RETARDO);
  LEDS_PORTX |= (1 << LEDS_5);
  _delay_ms(RETARDO);
  LEDS_PORTX |= (1 << LEDS_6);
  _delay_ms(RETARDO);
  LEDS_PORTX &= ~(1 << LEDS_6);
  _delay_ms(RETARDO);
  LEDS_PORTX &= ~(1 << LEDS_5);
  _delay_ms(RETARDO);
  LEDS_PORTX &= ~(1 << LEDS_4);
  _delay_ms(RETARDO);
  LEDS_PORTX &= ~(1 << LEDS_3);
  _delay_ms(RETARDO);
  LEDS_PORTX &= ~(1 << LEDS_2);
  _delay_ms(RETARDO);
  LEDS_PORTX &= ~(1 << LEDS_1);
  _delay_ms(RETARDO);
}

void leds_on(void)
{
  LEDS_PORTX |= (1 << LEDS_1);
  LEDS_PORTX |= (1 << LEDS_2);
  LEDS_PORTX |= (1 << LEDS_3);
  LEDS_PORTX |= (1 << LEDS_4);
}

void leds_centrales_exteriores(void){
  LEDS_PORTX |= (1 << LEDS_1);
  LEDS_PORTX |= (1 << LEDS_4);
  _delay_ms(RETARDO);
  LEDS_PORTX |= (1 << LEDS_2);
  LEDS_PORTX |= (1 << LEDS_3);
}

void leds_izquierda_derecha(void){
  for(int i = 0; i <= 4; i++){
    LEDS_PORTX = (1<<i);
    _delay_ms(RETARDO);
  }
}

void leds_derecha_izquierda(void){
  for(int i = 4; i >= 0; i--){
    LEDS_PORTX = (1>>i);
    _delay_ms(RETARDO);
  }
}

void leds_parpadeo(void){
  LEDS_PORTX |= (1 << LEDS_1);
  LEDS_PORTX |= (1 << LEDS_2);
  LEDS_PORTX |= (1 << LEDS_3);
  LEDS_PORTX |= (1 << LEDS_4);
    _delay_ms(RETARDO);
  LEDS_PORTX &= ~(1 << LEDS_1);
  LEDS_PORTX &= ~(1 << LEDS_2);
  LEDS_PORTX &= ~(1 << LEDS_3);
  LEDS_PORTX &= ~(1 << LEDS_4);
}

// display
void display_init(void);
void display_E(void);
void display_1(void);
void display_2(void);
void display_P(void);
void display_F(void);