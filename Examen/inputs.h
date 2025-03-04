#include <avr/io.h>
#include <util/delay.h>

// Etiquetas
// Inputs
#define BTNS_DDRX DDRC
#define BTNS_PINX PINC
#define BTNS_PORTX PORTC
#define BTNS_0 PINC2
#define BTNS_1 PINC3
#define BTNS_2 PINC4
// Macros
#define BTNS_0_READ (BTNS_PINX & (1 << BTNS_0))
#define BTNS_1_READ (BTNS_PINX & (1 << BTNS_1))
#define BTNS_2_READ (BTNS_PINX & (1 << BTNS_2))
