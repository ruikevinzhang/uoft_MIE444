#define A_dir         12
#define A_brake        9
#define B_dir         13
#define B_brake        8


#define enA        7 
#define in1        6
#define in2        5
   
    
void setup() {
  //setup Channel A  //left wheel
  pinMode(A_dir, OUTPUT); //Initiates the Motor Channel A pin
  pinMode(A_brake, OUTPUT); //Initiates the Brake channel A pin

  //Setup Channel B //right wheel
  pinMode(B_dir, OUTPUT); //Initiates Motor Channel B pin
  pinMode(B_brake, OUTPUT);  //Initiates Brake Channel B pin
  
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);

  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);  
  pinMode(in2, OUTPUT);

  digitalWrite(enA,LOW);
  
  Serial.begin(9600); 
}

//_____________________________________________________________________________________________________
//_____________________________________________________________________________________________________
//_____________________________________________________________________________________________________


void runA(int speed, boolean rev) {  
  if(rev) {
    //Establishes backward direction of Channel A
    digitalWrite(12, LOW);  
  }
  else {
    //Establishes forward direction of Channel A
    digitalWrite(12, HIGH);   
  }
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
  analogWrite(3, speed);   //Spins the motor on Channel A at full speed
}  


void runB(int speed, boolean rev) {
  if(rev) {
    digitalWrite(13, LOW); //Establishes backwards direction of Channel B
  }
  else{
    digitalWrite(13, HIGH); //Establishes forward direction of Channel B
  }
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, speed);   //Spins the motor on Channel B at full speed 
}

//fullstop
void fs(){
  
  digitalWrite(9, HIGH);
  digitalWrite(8, HIGH);
  delay(100);
  Serial.println("full stop");   
}
//_____________________________________________________________________________________________________
//_____________________________________________________________________________________________________
//_____________________________________________________________________________________________________


void forward (int runtime, int pausetime) {
  runA(255/1.6, true);
  runB(255/1.5, false);
  delay(runtime);
  fs();
  delay(pausetime);
}

void backward (int runtime, int pausetime) {
  runA(255/1.6, false);
  runB(255/1.5, true);
  delay(runtime);
  fs();
  delay(pausetime);
}

void rightturn (int runtime, int turntime, int pausetime){
  //forward (1.5*runtime, pausetime);

  runB(255/1.5, true);
  runA(255/1.5, true);
  
  delay(turntime);
  fs();
  delay(pausetime);
}


void leftturn (int runtime, int turntime, int pausetime){
  //forward (1.5*runtime, pausetime);
  runA(255/1.5, false);
  runB(255/1.5, false);
  delay(turntime);
  fs();
  delay(pausetime);
}

void uturn (int runtime, int turntime, int pausetime, bool dir){
  
  runA(255/1.5, dir);
  runB(255/1.5, dir);
  delay(turntime);
  fs();
  delay(pausetime);
}



//_____________________________________________________________________________________________________
//_____________________________________________________________________________________________________
//_____________________________________________________________________________________________________
void loop() {
  int runtime, pausetime, turntime,fullstop,test,turnforwardtime,forwardtime;
  char state = 0;  

  turntime = 960;
  turnforwardtime = 1000;
  forwardtime = 200*2; 
       
  //turntime = 100;  //90degree
  runtime = 400;   //1/4 block
  pausetime = 500;


  
  if(Serial.available() > 0){ // Checks whether data is comming from the serial port
    state = Serial.read();  


    
  }
  //normal forward movement
  if (state == 'f') {
    forward (runtime, pausetime);
    //Serial.println("forward"); 
    state=0;  
  }
  //small step forward
  else if (state == 'w') {
    forward (runtime/2, pausetime);
    //Serial.println("forward");   
    state=0;  
  }
  else if (state == 'b') {
    backward (runtime*1.5, pausetime);
    //Serial.println("forward");   
    state=0;  
  }
  else if (state == 's') { //backadjust
    backward (runtime/2, pausetime);
    //Serial.println("forward");   
    state=0;  
  }


  //normal 90 degree right turn
  else if (state == 'r') {
    rightturn (runtime, turntime, pausetime);
    //Serial.println("rightturn");
    state=0;  
  }
  //normal 90 degree left turn
  else if (state == 'l') {
    leftturn (runtime, turntime, pausetime);
    //Serial.println("leftturn");
    state=0;  
   }

  //small step right adjust (10 degree)
  else if (state == 'd') {
    rightturn (runtime, turntime/9, pausetime);
    //Serial.println("rightturn");
    state=0;  
  }
  //small step left adjust (10 degree)
  else if (state == 'a') {
    leftturn (runtime, turntime/9, pausetime);
    //Serial.println("leftturn");
    state=0;  
   }
   
  else if (state == 'u') {
    uturn (runtime, 2*turntime, pausetime,true);
    //Serial.println("leftturn");
    state=0;  
   }
   

  //control pickup
  else if (state == 'm') {
    while (state == 'm'){ 
      analogWrite (enA,255);
      digitalWrite(in1,LOW);
      digitalWrite(in2,HIGH);
      // delay(3000);
      //Serial.println("collecting");
       
      if(Serial.available() > 0){ // Checks whether data is comming from the serial port
        test = Serial.read();
          if (test=='q'){
            state=test;
            // digitalWrite (enA,LOW);
            digitalWrite(in1, LOW);
            digitalWrite(in2,LOW);
            //Serial.println("stop");
          }
      }
    }
  }

  else if (state == 'g') {
    while (state == 'g'){ 
      analogWrite (enA,255);
      digitalWrite(in1, HIGH);
      digitalWrite(in2,LOW);
      
      //delay(3000);
      
      //Serial.println("releasing");
      if(Serial.available() > 0){ // Checks whether data is comming from the serial port
        test = Serial.read();
        if (test=='q'){
          state=test;
          //digitalWrite (enA,LOW);
          digitalWrite(in1, LOW);
          digitalWrite(in2,LOW);
          //Serial.println("stop");
        }
      }
    }
  }
  
}
