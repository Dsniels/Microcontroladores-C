
#include <avr/io.h>
#define RETARDO 10000
#include <util/delay.h>

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
const uint8_t tabla7seg[17] = {
    0b11111110, // 0
    0b11101101, // 2
    0b00110000, // 1
    0b11111001, // 3
    0b10110011, // 4
    0b11011011, // 5
    0b11011111, // 6
    0b11110000, // 7
    0b11111111, // 8
    0b11111011, // 9
    0b11110111, // A
    0b10011111, // B
    0b11001110, // C
    0b10111101, // D
    0b11001111, // E
    0b11000111, // F
    0b11100111};

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
  display_init();
  while (1)
  { // 0bXXXXXXXX
    //     &
    // 0b00000100
    // 0b00000X00
    if (!BTNS_2_READ)
    {
      leds_on();
      display_F();
      continue;
    }
    if (!(BTNS_1_READ) && !(BTNS_0_READ))
    {
      display_P();
      leds_parpadeo();
    }
    else if (!(BTNS_1_READ) && (BTNS_0_READ))
    {
      display_1();
      leds_izquierda_derecha();
    }
    else if ((BTNS_1_READ) && !(BTNS_0_READ))
    {
      display_2();
      leds_derecha_izquierda();
    }
    else if ((BTNS_1_READ) && (BTNS_0_READ))
    {
      display_E();
      leds_centrales_exteriores();
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

void leds_on(void)
{
  LEDS_PORTX |= (1 << LEDS_1);
  LEDS_PORTX |= (1 << LEDS_2);
  LEDS_PORTX |= (1 << LEDS_3);
  LEDS_PORTX |= (1 << LEDS_4);
}

void leds_centrales_exteriores(void)
{
  _delay_ms(RETARDO);

  LEDS_PORTX &= ~(1 << LEDS_2);
  LEDS_PORTX &= ~(1 << LEDS_3);

  LEDS_PORTX |= (1 << LEDS_1);
  LEDS_PORTX |= (1 << LEDS_4);
  _delay_ms(RETARDO);
  LEDS_PORTX &= ~(1 << LEDS_1);
  LEDS_PORTX &= ~(1 << LEDS_4);
  _delay_ms(RETARDO);

  LEDS_PORTX |= (1 << LEDS_2);
  LEDS_PORTX |= (1 << LEDS_3);
  _delay_ms(RETARDO);
}

void leds_izquierda_derecha(void)
{
  for (int i = 0; i <= 4; i++)
  {
    LEDS_PORTX = (1 << i);
    _delay_ms(RETARDO);
  }
}

void leds_derecha_izquierda(void)
{
  for (int i = 4; i >= 0; i--)
  {
    LEDS_PORTX = (1 << i);
    _delay_ms(RETARDO);
  }
}

void leds_parpadeo(void)
{
  _delay_ms(RETARDO);
  LEDS_PORTX |= (1 << LEDS_1);
  LEDS_PORTX |= (1 << LEDS_2);
  LEDS_PORTX |= (1 << LEDS_3);
  LEDS_PORTX |= (1 << LEDS_4);
  _delay_ms(RETARDO);
  LEDS_PORTX &= ~(1 << LEDS_1);
  LEDS_PORTX &= ~(1 << LEDS_2);
  LEDS_PORTX &= ~(1 << LEDS_3);
  LEDS_PORTX &= ~(1 << LEDS_4);
  _delay_ms(RETARDO);
}

void display_init(void)
{
  DISPLAY_DDRX = 0b01111111;
}
void display_E(void)
{
  DISPLAY_PORTX = tabla7seg[14];
}

void display_1(void)
{
  DISPLAY_PORTX = tabla7seg[1];
}
void display_2(void)
{

  DISPLAY_PORTX = tabla7seg[2];
}
void display_P(void)
{
  DISPLAY_PORTX = tabla7seg[16];
}
void display_F(void)
{
  DISPLAY_PORTX = tabla7seg[15];
}