#define MOTOR1 (1 << PD0)  // Motor M1 (30HP) conectado ao PD0
#define MOTOR2 (1 << PD1)  // Motor M2 (50HP) conectado ao PD1
#define MOTOR3 (1 << PD2)  // Motor M3 (70HP) conectado ao PD2
#define BOTAO_A (1 << PD3) // Botão A conectado ao PD3
#define BOTAO_B (1 << PD4) // Botão B conectado ao PD4
#define BOTAO_C (1 << PD5) // Botão C conectado ao PD5

int main(void) {
    // Definir motores como saídas
    DDRD |= MOTOR1 | MOTOR2 | MOTOR3;
    // Definir botões com pull-up
    PORTD |= BOTAO_A | BOTAO_B | BOTAO_C;

    int potencia_total = 0;

    while (1) {
        // Verificar botão A (motor M1, 30HP)
        if (!(PIND & BOTAO_A)) {
            potencia_total += 30;
            PORTD |= MOTOR1;  // Liga motor M1
        }
        // Verificar botão B (motor M2, 50HP)
        if (!(PIND & BOTAO_B)) {
            potencia_total += 50;
            PORTD |= MOTOR2;  // Liga motor M2
        }
        // Verificar botão C (motor M3, 70HP)
        if (!(PIND & BOTAO_C)) {
            potencia_total += 70;
            PORTD |= MOTOR3;  // Liga motor M3
        }

        // Verificar se a potência total ultrapassa 90HP
        if (potencia_total > 90) {
            if(PIND & MOTOR1 != 0){
              PORTD &= ~MOTOR1;  // Desliga motor M1
              potencia_total -= 30;
            }
            if (potencia_total > 90 && (PIND & MOTOR2 != 0)) {
                PORTD &= ~MOTOR2;  // Desliga motor M2
                potencia_total -= 50;
            }
        }
    }
}
