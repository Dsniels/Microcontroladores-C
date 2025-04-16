#include <avr/io.h>
#include <avr/interrupt.h>

#define RETARDO 500

#define BUTTONS_DDRX   DDRD
#define BUTTONS_PINX   PIND
#define BUTTONS_PORTX  PORTD
#define BUTTONS_1      PIND3    
#define BUTTONS_0      PIND2    
#define BUTTONS_0_READ (BUTTONS_PINX & (1<<BUTTONS_0))
#define BUTTONS_1_READ (BUTTONS_PINX & (1<<BUTTONS_1))

#define DISPLAY7SEG_AF_DDRX  DDRB
#define DISPLAY7SEG_G_DDRX   DDRC
#define DISPLAY7SEG_AF_PORTX PORTB
#define DISPLAY7SEG_G_PORTX  PORTC
#define DISPLAY7SEG_A        PORTB0
#define DISPLAY7SEG_B        PORTB1
#define DISPLAY7SEG_C        PORTB2
#define DISPLAY7SEG_D        PORTB3
#define DISPLAY7SEG_E        PORTB4
#define DISPLAY7SEG_F        PORTB5
#define DISPLAY7SEG_G        PORTC0

#define DISPLAY7SEG_MUX_DDRX     DDRC
#define DISPLAY7SEG_MUX_PORTX    PORTC
#define DISPLAY7SEG_MUX_MILLARES PORTC1
#define DISPLAY7SEG_MUX_CENTENAS PORTC2
#define DISPLAY7SEG_MUX_DECENAS  PORTC3
#define DISPLAY7SEG_MUX_UNIDADES PORTC4

#define BUZZER_DDR  DDRC
#define BUZZER_PORT PORTC
#define BUZZER_PIN  PORTC5

typedef enum {
    IDLE,           
    CONFIG_SEC,     
    CONFIG_MIN,     
    RUNNING,        
    PAUSED,         
    ALARM           
} State;

volatile State current_state = IDLE;
volatile uint8_t minutes = 59;
volatile uint8_t seconds = 59;
volatile uint8_t current_digit = 0;
volatile uint8_t blink_state = 0;
volatile uint16_t blink_counter = 0;
volatile uint8_t buzzer_state = 0;
volatile uint16_t buzzer_counter = 0;

volatile uint8_t button0_flag = 0;
volatile uint8_t button1_flag = 0;
volatile uint8_t debounce_counter0 = 0;
volatile uint8_t debounce_counter1 = 0;
volatile uint8_t button0_pressed = 0;
volatile uint8_t button1_pressed = 0;

const uint8_t digit_patterns[10] = {
    0b00111111,
    0b00000110,
    0b01011011,
    0b01001111,
    0b01100110,
    0b01101101,
    0b01111101,
    0b00000111,
    0b01111111,
    0b01101111
};

void set_digit(uint8_t digit) {
    DISPLAY7SEG_AF_PORTX &= ~0x3F; 
    DISPLAY7SEG_AF_PORTX |= (digit & 0x3F); 
    
    if (digit & 0x40) {
        DISPLAY7SEG_G_PORTX |= (1 << DISPLAY7SEG_G);
    } else {
        DISPLAY7SEG_G_PORTX &= ~(1 << DISPLAY7SEG_G);
    }
}

void update_display() {
    DISPLAY7SEG_MUX_PORTX &= ~((1 << DISPLAY7SEG_MUX_MILLARES) | 
                              (1 << DISPLAY7SEG_MUX_CENTENAS) | 
                              (1 << DISPLAY7SEG_MUX_DECENAS) | 
                              (1 << DISPLAY7SEG_MUX_UNIDADES));
    
    uint8_t show_sec_digits = 1;
    uint8_t show_min_digits = 1;
    
    if (current_state == CONFIG_SEC) {
        show_sec_digits = blink_state;
    }
    else if (current_state == CONFIG_MIN) {
        show_min_digits = blink_state;
    }
    
    switch(current_digit) {
        case 0: 
            if (show_min_digits) {
                set_digit(digit_patterns[minutes / 10]);
                DISPLAY7SEG_MUX_PORTX |= (1 << DISPLAY7SEG_MUX_MILLARES);
            }
            break;
            
        case 1:
            if (show_min_digits) {
                set_digit(digit_patterns[minutes % 10]);
                DISPLAY7SEG_MUX_PORTX |= (1 << DISPLAY7SEG_MUX_CENTENAS);
            }
            break;
            
        case 2:
            if (show_sec_digits) {
                set_digit(digit_patterns[seconds / 10]);
                DISPLAY7SEG_MUX_PORTX |= (1 << DISPLAY7SEG_MUX_DECENAS);
            }
            break;
            
        case 3:
            if (show_sec_digits) {
                set_digit(digit_patterns[seconds % 10]);
                DISPLAY7SEG_MUX_PORTX |= (1 << DISPLAY7SEG_MUX_UNIDADES);
            }
            break;
    }
    
    current_digit = (current_digit + 1) % 4;
    blink_counter++;
    if (blink_counter >= 20) {
        blink_state = !blink_state;
        blink_counter = 0;
    }
}
void process_button0_action() {
    switch (current_state) {
        case IDLE:
            current_state = RUNNING;
            break;
            
        case CONFIG_SEC:
            seconds = (seconds + 1) % 60;
            break;
            
        case CONFIG_MIN:
            minutes = (minutes + 1) % 60;
            break;
            
        case RUNNING:
            current_state = PAUSED;
            break;
            
        case PAUSED:
            current_state = RUNNING;
            break;
            
        case ALARM:
            current_state = IDLE;
            minutes = 59;
            seconds = 59;
            break;
    }
}

void process_button1_action() {
    switch (current_state) {
        case IDLE:
            current_state = CONFIG_SEC;
            break;
            
        case CONFIG_SEC:
            current_state = CONFIG_MIN;
            break;
            
        case CONFIG_MIN:
            current_state = IDLE;
            break;
            
        case ALARM:
            current_state = IDLE;
            minutes = 59;
            seconds = 59;
            break;
            
        default:
            break;
    }
}

void init() {
    DISPLAY7SEG_AF_DDRX |= 0x3F;
    DISPLAY7SEG_G_DDRX |= (1 << DISPLAY7SEG_G);
    
    DISPLAY7SEG_MUX_DDRX |= (1 << DISPLAY7SEG_MUX_MILLARES) | 
                          (1 << DISPLAY7SEG_MUX_CENTENAS) | 
                          (1 << DISPLAY7SEG_MUX_DECENAS) | 
                          (1 << DISPLAY7SEG_MUX_UNIDADES);
    
    BUZZER_DDR |= (1 << BUZZER_PIN);
    
    BUTTONS_DDRX &= ~((1 << BUTTONS_0) | (1 << BUTTONS_1));
    BUTTONS_PORTX |= (1 << BUTTONS_0) | (1 << BUTTONS_1);
    
    EICRA |= (1 << ISC01) | (1 << ISC11);
    EIMSK |= (1 << INT0) | (1 << INT1);
    
    TCCR0A |= (1 << WGM01);
    TCCR0B |= (1 << CS01) | (1 << CS00);  
    OCR0A = 100;  
    TIMSK0 |= (1 << OCIE0A);


    TCCR1B |= (1 << WGM12) | (1 << CS12);  
    OCR1A = 62499;  
    TIMSK1 |= (1 << OCIE1A);
    
    TCCR2A |= (1 << WGM21);
    TCCR2B |= (1 << CS22);
    OCR2A = 100;
    TIMSK2 |= (1 << OCIE2A);
    
    sei();
}

ISR(TIMER0_COMPA_vect) {
    update_display();
}

ISR(TIMER1_COMPA_vect) {
    if (current_state == RUNNING) {
        if (seconds > 0) {
            seconds--;
        } else if (minutes > 0) {
            minutes--;
            seconds = 59;
        } else {
            current_state = ALARM;
        }
    }
}

ISR(TIMER2_COMPA_vect) {
    if (current_state == ALARM) {
        buzzer_counter++;
        
        if (buzzer_counter >= 500) {
            buzzer_state = !buzzer_state;
            if (buzzer_state) {
                BUZZER_PORT |= (1 << BUZZER_PIN);
            } else {
                BUZZER_PORT &= ~(1 << BUZZER_PIN);
            }
            buzzer_counter = 0;
        }
    } else {
        BUZZER_PORT &= ~(1 << BUZZER_PIN);
        buzzer_counter = 0;
        buzzer_state = 0;
    }
    
    if (button0_flag) {
        debounce_counter0++;
        if (debounce_counter0 >= 20) {
            if (!BUTTONS_0_READ) {
                button0_pressed = 1;
            }
            button0_flag = 0;
            debounce_counter0 = 0;
        }
    }
    
    if (button1_flag) {
        debounce_counter1++;
        if (debounce_counter1 >= 20) {
            if (!BUTTONS_1_READ) {
                button1_pressed = 1;
            }
            button1_flag = 0;
            debounce_counter1 = 0;
        }
    }
}

ISR(INT0_vect) {
    button0_flag = 1;
}

ISR(INT1_vect) {
    button1_flag = 1;
}

int main(void) {
    init();
    
    while (1) {
        if (button0_pressed) {
            process_button0_action();
            button0_pressed = 0;
        }
        
        if (button1_pressed) {
            process_button1_action();
            button1_pressed = 0;
        }
    }
    
    return 0;
}
