#define MOTOR_PIN 5                  
#define STOP_PIN 6
#define IR_READ 2                    

int sec=0;
int hour=0;
bool state=true;


void setup(){
  Serial.begin(9600);
  digitalWrite(STOP_PIN,LOW);
  attachInterrupt(0,STATE,RISING);
  cli();

//set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();//allow interrupts

}


//ISR for timer interrupt
ISR(TIMER1_COMPA_vect){
sec++;
}
  

void loop(){
  //runs after every 5 min and moves motor
    if(sec==300){
      moveMotor();
      sec=0;
      Serial.println(sec);
      hour++;
    }

    /*the timer interrupts drift by 13 sec on average in 24 hour
     * this if block compensate for that drift
     */
    if(hour==24){
      sec=sec-13;
      hour=0;
    }
}
//moveMotor() moves motor till the interrupt is triggered by ir sensor and actively stops it 
void moveMotor(){
  state=true;
  while(state){
    digitalWrite(MOTOR_PIN,HIGH);
  }
  digitalWrite(MOTOR_PIN,LOW);
  digitalWrite(STOP_PIN,HIGH);
  delay(100);
  digitalWrite(STOP_PIN,LOW);
}



void STATE(){       //ISR for hardware interrupt
  state=false;
}
