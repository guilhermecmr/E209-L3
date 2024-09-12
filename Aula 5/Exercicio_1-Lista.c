#define MOTOR (1 << PD7)  // Motor conectado ao PD7
#define ALARME (1 << PD6) // Alarme conectado ao PD6
#define LIGA (1 << PD5)   // Botão LIGA (NA) conectado ao PD5
#define DESLIGA (1 << PD4) // Botão DESLIGA (NF) conectado ao PD4
#define S1 (1 << PD3)     // Sensor de amontoamento de peças conectado ao PD3

int main() {
    // Definindo os pinos como entrada ou saída
    DDRD |= MOTOR | ALARME; // PD7 e PD6 como saídas

    // Habilitar pull-ups nos botões
    PORTD |= LIGA | DESLIGA;

    while (1) {
        if (PIND & S1 != 0) {  // Amontoamento de peças detectado
            PORTD &= ~MOTOR;  // Para a esteira
            PORTD |= ALARME;  // Liga o alarme
        }
        if ((!(PIND & LIGA) == 0) && (!(PIND & S1) == 0)) {  // Verifica se o botão LIGA foi pressionado e não há amontoamento de peças
              PORTD |= MOTOR;  // Liga a esteira (MOTOR = HIGH)
              PORTD &= ~ALARME; // Desliga o alarme
        }
        if (!(PIND & DESLIGA) == 0) {  // Verifica se o botão DESLIGA foi pressionado
            PORTD &= ~MOTOR;  // Desliga a esteira (MOTOR = LOW)
        }
    }
}
