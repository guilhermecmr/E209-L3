#define BUZZER PD6  // Saída sonora (buzzer)
#define C      PD0  // Nota DÓ
#define D      PD1  // Nota RÉ
#define E      PD2  // Nota MI
#define F      PD3  // Nota FÁ
#define G      PD4  // Nota SOL
#define A      PD5  // Nota LÁ
#define B      PD7  // Nota SI
// Máscara para identificar os pinos configurados para botões
#define MASCARA_BOTOES ((1 << C) | (1 << D) | (1 << E) | (1 << F) | (1 << G) | (1 << A) | (1 << B)) 

int   menor_nota = 20;                                               // Valor base para a frequência da menor nota
int   intervalo_notas = 20;                                          // Intervalo entre as notas em frequências
float maximo_adc = (255.0 - (menor_nota + intervalo_notas * 6.0));   // Calcula o fator de normalização (valor maximo de leitura) do ADC a partir da menor nota e do intervalo entre as notas
int   leitura_ADC = 0;                                               // Leitura obtida do ADC
int   nota = 0;                                                      // Nota sendo tocada (C, D, E, F, G, A ou B)
int   intensidade_buzzer = 0;                                        // Intensidade do buzzer
int   estado_buzzer = 0;                                             // Estado do buzzer (ligado/desligado)
int   ultimo_botao_pressionado = 0xFF;                               // Último botão pressionado (nenhum inicialmente)
int   estado_anterior_botoes = 0xFF;                                 // Estado anterior dos botões (todos "não pressionados")
int   estado_atual_botoes = 0xFF;                                    // Estado atual dos botões (todos "não pressionados")
int   contador_timer = 0;                                            // Contador para o timer de 1 segundo

// Inicializa o ADC
void inicializaADC() {
  ADMUX = (0 << REFS1) | (1 << REFS0);                               // Referência de tensão para 5V
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Habilita ADC com prescaler de 128
  DIDR0 = (1 << PC0);                                                // Desabilita entrada digital no pino ADC0
}

// Lê o ADC
void leADC() {
  ADMUX = (ADMUX & 0xF8) | PC0;               // Seleciona o canal ADC0
  ADCSRA |= (1 << ADSC);                      // Inicia a conversão
  while (ADCSRA & (1 << ADSC));               // Aguarda a finalização da conversão
  leitura_ADC = (ADC * maximo_adc) / 1023.0;  // Normaliza e armazena o resultado da conversão 
}

// Inicializa Timer0 no modo PWM
void inicializaTimer0_PWM() {
  TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1); // Configura Timer0 para modo PWM
  TCCR0B = (1 << CS01) | (1 << CS00);                   // Configura prescaler para 64
}

// Inicializa Timer2 no modo CTC
void inicializaTimer2_CTC() {
  TCCR2A = (1 << WGM21);           // Configura Timer2 para modo CTC
  TCCR2B = (1 << CS22);            // Configura prescaler para 64
  OCR2A = 156;                     // Define o valor de comparação para 1 segundo
  TIMSK2 = (1 << OCIE2A);          // Habilita a interrupção de comparação
}

// Configura os pinos
void inicializaPinos() {
  DDRD |= (1 << BUZZER);           // Configura PD6 como saída (BUZZER)
  DDRD &= ~MASCARA_BOTOES;         // Configura os pinos dos botões como entrada
  PORTD |= MASCARA_BOTOES;         // Habilita resistores pull-up para os botões
}

// ISR para Timer2 (controle do tempo do BUZZER)
ISR(TIMER2_COMPA_vect) {
  if (estado_buzzer) {             // Verifica se o buzzer está ligado
    contador_timer++;              // Incrementa o contador
    if (contador_timer >= 1592) {  // Verifica se 1 segundo se passou ((clock / prescaler) / (OCR2A + 1)) | ((16MHz / 64) / 157) 
      contador_timer = 0;          // Reseta o contador
      intensidade_buzzer = 0;      // Zera a intensidade do buzzer
      OCR0A = 0;                   // Zera o valor de saída PWM
      estado_buzzer = 0;           // Desliga o buzzer
    }
  }
}

// Verifica botões e controla BUZZER
void verificaBotoes() {
  estado_atual_botoes = PIND & MASCARA_BOTOES; // Lê o estado atual dos botões (invertido para considerar pull-up)
  
  /*
  // Verifica qual botão foi pressionado e ajusta a nota correspondente (opção mais segura)
  if ((PIND & (1 << C)) && !estado_buzzer) {
    nota = menor_nota;                         // Define a frequência da nota C
    ultimo_botao_pressionado = (1 << C);       // Armazena PD0 como o botão pressionado
  } else if ((PIND & (1 << D)) && !estado_buzzer) {
    nota = menor_nota + intervalo_notas;       // Define a frequência da nota D
    ultimo_botao_pressionado = (1 << D);       // Armazena PD1 como  o botão pressionado
  } else if ((PIND & (1 << E)) && !estado_buzzer) {
    nota = menor_nota + intervalo_notas * 2;   // Define a frequência da nota E
    ultimo_botao_pressionado = (1 << E);       // Armazena PD2 como o botão pressionado
  } else if ((PIND & (1 << F)) && !estado_buzzer) {
    nota = menor_nota + intervalo_notas * 3;   // Define a frequência da nota F
    ultimo_botao_pressionado = (1 << F);       // Armazena PD3 como o botão pressionado
  } else if ((PIND & (1 << G)) && !estado_buzzer) {
    nota = menor_nota + intervalo_notas * 4;   // Define a frequência da nota G
    ultimo_botao_pressionado = (1 << G);       // Armazena PD4 como o botão pressionado
  } else if ((PIND & (1 << A)) && !estado_buzzer) {
    nota = menor_nota + intervalo_notas * 5;   // Define a frequência da nota A
    ultimo_botao_pressionado = (1 << A);       // Armazena PD5 como o botão pressionado
  } else if ((PIND & (1 << B)) && !estado_buzzer) {
    nota = menor_nota + intervalo_notas * 6;   // Define a frequência da nota B
    ultimo_botao_pressionado = (1 << B);       // Armazena PD7 como o botão pressionado
  } else {
    nota = 0;                                  // Zera a nota se nenhum botão foi pressionado
    ultimo_botao_pressionado = 0xFF;           // Reseta o último botão pressionado
  }
  */

  // Verifica qual botão foi pressionado e ajusta a nota correspondente (opção mais otimizada)
  for(int i = 0; i <= 6; i++){
    if(i == 6) {i++;}                          // Caso i = 6, incrementa i para verificação de PD7 (PD6 é o pin do buzzer)
    if(!(PIND & (1 << i)) && !estado_buzzer){   // Caso o botão de PDi esteja ativado e o buzzer desativado
      ultimo_botao_pressionado = (1 << i);     // Armazena o botão pressionado
      if(i == 7) {i--;}                        // Caso i = 7, subtrai i para o incremento correto do intervalo de notas de PD7
      nota = menor_nota + intervalo_notas * i; // Define a frequência da nota 
      break;
    } else {
      nota = 0;                                // Zera a nota se nenhum botão foi pressionado
      ultimo_botao_pressionado = 0xFF;         // Reseta o último botão pressionado
    }
  }

  // Liga o buzzer se um botão foi pressionado, estado mudou e o buzzer estiver desligado
  if ((!(PIND & ultimo_botao_pressionado)) && (estado_atual_botoes != estado_anterior_botoes) && !estado_buzzer) {
    intensidade_buzzer = leitura_ADC + nota;       // Calcula a intensidade do buzzer (nota tocada + afinação definida pelo ADC)
    OCR0A = intensidade_buzzer;                    // Define a intensidade do PWM
    estado_buzzer = 1;                             // Marca buzzer como ligado
    contador_timer = 0;                            // Reseta o contador
    estado_anterior_botoes = estado_atual_botoes;  // Atualiza o estado dos botões
  } else {
    estado_anterior_botoes = estado_atual_botoes;  // Apenas atualiza o estado dos botões
  }
}

int main(void) {
  inicializaADC();         // Inicializa o ADC
  inicializaTimer0_PWM();  // Inicializa Timer0 no modo PWM
  inicializaTimer2_CTC();  // Inicializa Timer2 no modo CTC
  inicializaPinos();       // Configura os pinos
  sei();                   // Habilita interrupções globais

  while (1) {
    verificaBotoes();      // Verifica os botões
    leADC();               // Lê o valor do ADC
  }

  return 0;
}
