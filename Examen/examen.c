
#include <avr/io.h>
#define RETARDO 100
#include <util/delay.h>
#define F_CPU 16000000UL

// Inputs
#define BTNS_DDRX DDRC
#define BTNS_PINX PINC
#define BTNS_PORTX PORTC
#define BTNS_0 PINC2
#define BTNS_1 PINC3
#define BTNS_2 PINC4
// Motor
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

const uint8_t tabla7seg[] = {
    0b01111110, // 0
    0b00110000, // 1
    0b01101101, // 2
    0b01111001, // 3
    0b11001111, // E
    0b11000111, // F
    0b01100111,//P
};

#define LEDS_DDRX DDRB
#define LEDS_PORTX PORTB
#define LEDS_1 PORTB2
#define LEDS_2 PORTB3
#define LEDS_3 PORTB4
#define LEDS_4 PORTB5
#define MOTOR_2 PORTB1

// botones
void btns_init(void);
// leds
void leds_init(void);
void leds_centrales_exteriores(void);
void leds_izquierda_derecha(void);
void leds_derecha_izquierda(void);
void leds_parpadeo(void);
void leds_on(void);
// motor
void motor_init(void);
void motor_off(void);
void motor_izquierda(void);
void motor_derecha(void);
void motor_free_wheel(void);
void motor_fast_stop(void);
// display
void display_init(void);
void display_E(void);
void display_1(void);
void display_2(void);
void display_P(void);
void display_F(void);
// Constante

int main(void)
{
  btns_init();
  motor_init();
  leds_init();
  display_init();
  while (1)
  {
    if (!BTNS_2_READ)
    {
      leds_on();
      display_F();
            motor_fast_stop();

      continue;
    }
    if (!(BTNS_1_READ) && !(BTNS_0_READ))
    {
      display_P();
      leds_parpadeo();
      motor_free_wheel();
    }
    else if (!(BTNS_1_READ) && (BTNS_0_READ))
    {
      display_2();

      leds_izquierda_derecha();
      motor_derecha();
    }
    else if ((BTNS_1_READ) && !(BTNS_0_READ))
    {
      display_1();

      leds_derecha_izquierda();
      motor_izquierda();
    }
    else if ((BTNS_1_READ) && (BTNS_0_READ))
    {
      display_E();
      leds_centrales_exteriores();
      motor_off();
    }
  }
}

void btns_init(void)
{
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
  for (int i = LEDS_1; i <= LEDS_4; i++)
  {
    LEDS_PORTX &= ~(0b00111100);
    LEDS_PORTX |= (1 << i);
    _delay_ms(RETARDO);
  }
}

void leds_derecha_izquierda(void)
{
  for (int i= LEDS_4; i >= LEDS_1; i--)
  { 
    LEDS_PORTX &= ~(0b00111100);
    LEDS_PORTX |= (1 << i);
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
  DISPLAY_DDRX = 0b11111111;
}
void display_E(void)
{
  DISPLAY_PORTX = tabla7seg[4];
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
  DISPLAY_PORTX = tabla7seg[6];
}
void display_F(void)
{
  DISPLAY_PORTX = tabla7seg[5];
}

void motor_init(void)
{
  BTNS_DDRX |= (1 << MOTOR_0);
  BTNS_DDRX |= (1 << MOTOR_1);
  LEDS_DDRX |= (1 << MOTOR_2);
  
}
void motor_off(void)
{
  BTNS_PORTX &= ~(1 << MOTOR_0);
  BTNS_PORTX &= ~(1 << MOTOR_1);
  LEDS_PORTX |= (1 << MOTOR_2);
}
void motor_izquierda(void)
{
  BTNS_PORTX |= (1 << MOTOR_1);
  BTNS_PORTX &= ~(1 << MOTOR_0);
  LEDS_PORTX |= (1 << MOTOR_2);
}
void motor_derecha(void)
{

  BTNS_PORTX |= (1 << MOTOR_0);
  BTNS_PORTX &= ~(1 << MOTOR_1);
  LEDS_PORTX |= (1 << MOTOR_2);
}
void motor_free_wheel(void)
{
  LEDS_PORTX &= ~(1 << MOTOR_2);
}
void motor_fast_stop(void)
{
  BTNS_PORTX &= ~(1 << MOTOR_1);
  BTNS_PORTX &= ~(1 << MOTOR_0);
  LEDS_PORTX |= (1 << MOTOR_2);
}