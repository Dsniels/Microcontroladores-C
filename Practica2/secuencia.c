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


void autoFantasticoConFor(){

    for(int i = 0; i < 7; i++){
        PORTD = (1 << i);
        
    }
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
    DDRC = 0b11111100;  // DECLARA LAS ENTRADAS
    PORTC = 0b11111100; // ACTIVA LAS ENTRADAS
    PORTD = 0b11111111;

    while (true)
    {

        /*
        PIND contiene estado actual de los pines del puerto D
        el operador (1 << PD0) desplaza el 1 a la izquierda 0 posiciones
        resultando: 0b00000001


        Suponiendo que el boton no esta presionado PIND tendria un valor de 1
        PIND tendria todos los bits en 1 0b11111111.

        si el boton esta conectado al pin PD0 y se presiona el bit se vuelve un 0
        y el valor de PIND se convierte en 0b11111110

        se usa para crear una mascara binaria que tiene un unico bit encendido,
        el que corresponde a PD0. Es decir, si PD0 es 0, (1 << PD0)
        produce el valor 00000001.

        Luego, al aplicar la operación & con el valor leido del puerto
        , se hay que aislar ese  bit para poder verificar su estado.
        Si el resultado es 0, significa que el bit PD0 estaba apagado, y si es distinto de 0, estaba encendido.

       no presionado: 0b11111111 & 0b00000001 =  00000001

       presionado: 0b11111110 & 0b00000001 = 00000000

        if(PIND & (1<<PD0) == 0){

        }

*/
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
