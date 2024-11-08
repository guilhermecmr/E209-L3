#define LIGA PD2 // Botão LIGA no pino PD2 (INT0)
#define DESLIGA PD3 // Botão DESLIGA no pino PD3 (INT1)
#define LED1 PB0 // LED1 no pino PB0
#define LED2 PB1 // LED2 no pino PB1
#define MOTOR PD6 // MOTOR PWM no pino PD6 (OC0A)

int motor_ativo = 0; // Motor ativo ou não
int velocidade = 0; // Velocidade do motor

void interrupcoes() {
    // Configura interrupções externas
    EIMSK |= (1 << INT0) | (1 << INT1); // Ativa INT0 e INT1
    EICRA |= (1 << ISC01) | (1 << ISC11); // Configura INT0 e INT1 para tensão de descida
    sei(); 
}

void pwm() {
    // Configura o Timer0 para Fast PWM com saída no pino OC0A (PD6)
    TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1 << WGM00); // Fast PWM
    TCCR0B |= (1 << CS01) | (1 << CS00); // Prescaler 64
    OCR0A = 0;
}

void timer1() {
    // Configura o Timer1 para gerar uma interrupção a cada 1 segundo
    TCCR1A = 0; // Modo Normal
    TCCR1B |= (1 << WGM12); // Modo CTC
    OCR1A = 15624; // Valor para 1 segundo (16 MHz com prescaler de 1024)
    TIMSK1 |= (1 << OCIE1A); // Habilita interrupção de comparação para o Timer1
}

void iniciar_motor() {
    motor_ativo = 1;
    velocidade = 0;
    OCR0A = 0;
    TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler de 1024
}

void parar_motor() {
    TCCR1B &= ~((1 << CS12) | (1 << CS10)); // Para o Timer1
    OCR0A = 0; // Para o motor
    PORTB &= ~((1 << LED1) | (1 << LED2)); // Desliga LEDs
    motor_ativo = 0;
    velocidade = 0;
}

ISR(INT0_vect) { // Interrupção do botão LIGA
    if (!motor_ativo) {
        iniciar_motor();
    }
}

ISR(INT1_vect) { // Interrupção do botão DESLIGA
    parar_motor();
}

ISR(TIMER1_COMPA_vect) { // Interrupção do Timer1 a cada 1 segundo
    if (motor_ativo) {
        if (velocidade < 100) {
            velocidade += 10; // Incrementa 10% a cada segundo
            OCR0A = (velocidade * 255) / 100; // Atualiza PWM
            
            if (velocidade >= 50 && velocidade < 100) {
                PORTB |= (1 << LED1); // Acende LED1
                PORTB &= ~(1 << LED2); // Apaga LED2
            } else if (velocidade == 100) {
                PORTB |= (1 << LED2); // Acende LED2
                PORTB &= ~(1 << LED1); // Apaga LED1
            }
        }
    }
}

int main(void) {
    DDRB |= (1 << LED1) | (1 << LED2); // Configura os LEDs como saída
    DDRD |= (1 << MOTOR); // Configura o motor como saída
    interrupcoes();
    pwm();
    timer1();
    
    while (1) {
    }
    return 0;
}
