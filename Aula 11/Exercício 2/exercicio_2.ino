#define LEITURA PD2      
#define LED PB0              

int leituraAnterior = 0;  
int cont = 0;          

void configuraADC() {
  ADMUX = (1 << REFS0);              
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); 
}

void iniciaConversaoADC() {
  ADCSRA |= (1 << ADSC);              
  while (ADCSRA & (1 << ADSC));       
}

int leADC() {
  int resultado = ADC;           
  return resultado;
}

void configuraInterrupcao() {
  EICRA |= (1 << ISC01);              
  EIMSK |= (1 << INT0);               
}

void configuraTimer() {
  TCCR1A = 0;                         
  OCR1A = 1953;                       
  TIMSK1 = (1 << OCIE1A);             
}

void iniciaTimer() {
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); 
}

ISR(INT0_vect) {
  iniciaConversaoADC();
  int leituraAtual = leADC();
    
  if (leituraAnterior != 0) {
    if (leituraAtual < leituraAnterior * 0.8) { 
      cont++;
      if (cont >= 10) {
        iniciaTimer();                     
      } else if (cont >= 5) {
        PORTB |= (1 << LED);              
      }
    }
  }
  leituraAnterior = leituraAtual;                
}

ISR(TIMER1_COMPA_vect) {
  PORTB ^= (1 << LED);                  
}

int main(void) {
  DDRB |= (1 << LED);                            
  PORTD |= (1 << LEITURA);              
  configuraTimer();   
  configuraADC();
  configuraInterrupcao();
  sei();                                         

  while (1) {
  }
  return 0;
}
