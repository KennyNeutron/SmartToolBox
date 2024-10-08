void PSABTinterrupt_setup() {
  cli();  // disable interrupts

  // set timer0 interrupt at 2kHz
  TCCR0A = 0;
  TCCR0B = 0;
  TCNT0 = 0;
  OCR0A = 124;
  TCCR0A |= (1 << WGM01);
  TCCR0B |= (1 << CS01) | (1 << CS00);
  TIMSK0 |= (1 << OCIE0A);

  // // set timer2 interrupt at 8kHz
  // TCCR2A = 0;
  // TCCR2B = 0;
  // TCNT2 = 0;
  // OCR2A = 249;
  // TCCR2A |= (1 << WGM21);
  // TCCR2B |= (1 << CS21);
  // TIMSK2 |= (1 << OCIE2A);

  sei();  // enable interrupts
}

// Interrupt for Push Buttons

ISR(TIMER0_COMPA_vect) {
  cli();
  count++;
  if (count >= 2000) {
    count_toggle = !count_toggle;
  }
  sei();
}

// // Interrput for Time
// ISR(TIMER2_COMPA_vect) {
//   // timer2 interrupt 8kHz
//   cli();  //disable all interrupts


//   sei();  //enable all interrupts
// }