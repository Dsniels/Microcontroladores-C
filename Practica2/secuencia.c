#include <avr/io.h>
#include <util/delay.h>
#define RETARDO 500

void apagarLeds()
{
    PORTD = 0;
    _delay_ms(RETARDO);
}

void encenderLed()
{
    PORTD = 0b01111111;
    _delay_ms(RETARDO);
}

void autoFantastico(){
    PORTD = 0b00000001; 
    _delay_ms(RETARDO);
    PORTD = 0b00000010; 
    _delay_ms(RETARDO);
    PORTD = 0b00000100;
    _delay_ms(RETARDO);
    PORTD = 0b00001000;
    _delay_ms(RETARDO);
    PORTD = 0b00010000;
    _delay_ms(RETARDO);
    PORTD = 0b00100000;
    _delay_ms(RETARDO);
    PORTD = 0b01000000;
    _delay_ms(RETARDO);
    PORTD = 0b00100000;
    _delay_ms(RETARDO);
    PORTD = 0b00010000;
    _delay_ms(RETARDO);
    PORTD = 0b00001000;
    _delay_ms(RETARDO);
    PORTD = 0b00000100;
    _delay_ms(RETARDO);
    PORTD = 0b00000010;
}




void secuenciaRandom(){
    PORTD = 0b00000100;
    PORTD = 0b01000100;    
  	PORTD = 0b01010100;

    _delay_ms(RETARDO);
    PORTD = 0b00000010;
    PORTD = 0b00100010;    
  	PORTD = 0b00101010;


    _delay_ms(RETARDO);

}




int main()
{
    DDRD = 0b01111111;
    DDRC = 0b11111100;
    PORTC = 0b11111100; 
    PORTD = 0b11111111;

    while (true)
    {

 switch ((((PINC & (1 << PC1)) ? 1 : 0) << 1) | (((PINC & (1 << PC0)) ? 1 : 0)))

        {
        case 0:
            autoFantastico();
            break;
        case 1:
            apagarLeds();
            break;
        case 2:
            secuenciaRandom();
            break;
        case 3:
            encenderLed();
            break;
        default:
            PORTD = 0xFF;
            break;
        }
    }


    return 0;
}
