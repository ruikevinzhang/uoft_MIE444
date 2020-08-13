#include <NewPing.h>
#include <Wire.h>

//#define switchpin     10 

#define TRIGGER_PIN   7
#define ECHO_PIN1     6
#define ECHO_PIN2     5
#define ECHO_PIN3     4
#define ECHO_PIN4     2

#define MAX_DISTANCE 200
 
int d1f, d2r, d3l, d4b;

NewPing sonar1(TRIGGER_PIN, ECHO_PIN1, MAX_DISTANCE);
NewPing sonar2(TRIGGER_PIN, ECHO_PIN2, MAX_DISTANCE);
NewPing sonar3(TRIGGER_PIN, ECHO_PIN3, MAX_DISTANCE);
NewPing sonar4(TRIGGER_PIN, ECHO_PIN4, MAX_DISTANCE);

#define A_dir         12
#define A_brake        9
#define B_dir         13
#define B_brake        8

const int MPU = 0x68; // MPU6050 I2C address
float AccZ;
float GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float yaw;
float elapsedTime, currentTime, previousTime;


int *p;

#define TIME_PAUSE  100


int* sense(int *dist){

  //int dist[4];
  //sensor reading 
   delay(100);
   //front sensor
   dist[0] = sonar1.ping_cm();
   delay(100);
   //right sensor
   dist[1] = sonar2.ping_cm();
   delay(100);
   //left sensor
   dist[2] = sonar3.ping_cm();
   delay(100);
   //back sensor
   dist[3] = sonar4.ping_cm();

   if (dist[0]==0 | dist[0]>20 ){
    dist[0]= 99;
   }
   if (dist[1] ==0| dist[1]>20 ){
    dist[1]  = 99;
   }
   if (dist[2] ==0| dist[2]>20 ){
    dist[2]  = 99;
   }
   if (dist[3] ==0| dist[3]>20 ){
    dist[3]  = 99;
   }
   //display sensor reading
   Serial.print("d1f: ");
   Serial.print(dist[0]);
   Serial.print(" d2r: ");
   Serial.print(dist[1] );
   Serial.print(" d3l: ");
   Serial.print(dist[2] );
   Serial.print(" d4b: ");
   Serial.println(dist[3] );

   
   return dist;
}

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
  }else{
    digitalWrite(13, HIGH); //Establishes forward direction of Channel B
  }
   
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, speed);   //Spins the motor on Channel B at full speed 
}

//fullstop
void fs(){
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
}


void forward(int runtime){
   Serial.println("Command: move forward");
   Serial.println(runtime);
   runA(255, true);
   runB(255, false);
   delay(runtime);
   fs();
   delay(TIME_PAUSE);
}

void leftturn(int turntime){
  Serial.print("Command: turn left");
  runA(255, false);
  runB(255, false);
  delay(turntime);
  fs();
  delay(TIME_PAUSE);
}

void rightturn(int turntime){
  Serial.println("Command: turn right");
  runA(255, true);
  runB(255, true);
  delay(turntime);
  fs();
  delay(TIME_PAUSE);
}

void adjustright(int adjusttime){
  Serial.println("Adjust: adjust right"); 
  runA(255, true);
  runB(255, true);
  delay(adjusttime*40);
  fs();
  delay(TIME_PAUSE); 
}

void adjustleft(int adjusttime){
  Serial.println("Adjust: adjust left");
  runA(255, false);
  runB(255, false);
  delay(adjusttime*40);
  fs();
  delay(TIME_PAUSE);
}


void setup() {
  Serial.begin(9600);
  
  //setup Channel A  //left wheel
  pinMode(A_dir, OUTPUT); //Initiates the Motor Channel A pin
  pinMode(A_brake, OUTPUT); //Initiates the Brake channel A pin

  //Setup Channel B //right wheel
  pinMode(B_dir, OUTPUT); //Initiates Motor Channel B pin
  pinMode(B_brake, OUTPUT);  //Initiates Brake Channel B pin
  
  //pinMode(switchpin, INPUT);  

  //Compass setup
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
  
}

int start=1;
 
void loop() {
  // int switchVal;
  // switchVal = digitalRead(switchPin); 
  
  int forwardtime, pausetime, turntime,turnforwardtime,adjusttime;
  int clearance, diff, block,range ; 
  int storeright = 6;
  int adjustdiff = 2;
  int temp = 0;
  int dist[4];

  int*ptr= sense(dist);
  d1f=ptr[0];
  d2r=ptr[1];
  d3l=ptr[2];
  d4b=ptr[3];
  Serial.println(d1f);
  Serial.println(d2r);
  Serial.println(d3l);
  Serial.println(d4b);
  
  turntime = 625;
  turnforwardtime = 1050;
  forwardtime = 200*2; 

    
  clearance = 16; //cm
  block   = 7 ;//cm
  range   = 10;
  diff =  7;//cm
   
   if (start == 1){
     temp = 0;
     start = 0;
   }

     
   //tuning section 
   //need to check compass direction to ensure 90 degree turn
   if (d1f<=(clearance+2)){
    //________________________________________________________________________________________________
    //Case 1: turn left
    if (d2r<=clearance+1){


      while (d1f>=(block)){
        //move forward until there is less than 5 cm
         forward(forwardtime/2);
         int*ptr= sense(dist);
         d1f=ptr[0];
         d2r=ptr[1];
         d3l=ptr[2];
         d4b=ptr[3];         
      }
      while (d1f<=clearance){
        //turn left until forward path is clear (>15 cm)
        leftturn(turntime);
        int*ptr= sense(dist);
        d1f=ptr[0];
        d2r=ptr[1];
        d3l=ptr[2];
        d4b=ptr[3];
      }
      if (d1f>= clearance*2 & d2r<clearance & d3l<clearance){
        forward(turnforwardtime);
      }
      else {
        forward(forwardtime);
      }
    }
    //________________________________________________________________________________________________
    //Case 2: turn right
    else {
      while (d1f>=block){
         //move forward until there is less than 5 cm
         forward(forwardtime/2);
         int*ptr= sense(dist);
         d1f=ptr[0];
         d2r=ptr[1];
         d3l=ptr[2];
         d4b=ptr[3];
      }
      rightturn(turntime);
      int*ptr= sense(dist);
      d1f=ptr[0];
      d2r=ptr[1];
      d3l=ptr[2];
      d4b=ptr[3];
      if (d1f>= clearance*2-5){
        forward(turnforwardtime);
      }
      else {
        forward(forwardtime);
      }
    }
   }
   //________________________________________________________________________________________________
   //case 3: move forward
   else if (d1f>=clearance & d2r <= clearance ){  
      //adjust the position to the right

      if (d2r<block-2 & d2r<storeright){
        adjustdiff = abs(storeright-d2r);
        if (adjustdiff<3){
          adjustdiff = 3 ;
        }
        if (d2r < 3){
          adjustdiff = 3;
          temp = 1;
        }
        adjustleft(adjustdiff);
      }
      else if (d2r>range & d2r>storeright){
        adjustdiff = abs(storeright-d2r);
        if (adjustdiff<3){
          adjustdiff = 2.5 ;
        }
        if (d2r < 3){
          adjustdiff = 2.5;
          temp = 1;
        }
        adjustright(adjustdiff);
      }
      storeright = d2r;
      int* ptr= sense(dist);
      d1f=ptr[0];
      d2r=ptr[1];
      d3l=ptr[2];
      d4b=ptr[3];
      if (temp == 1){
        forward(forwardtime/2);
        temp =0;
      }
      else if (d1f>= clearance){
        forward(forwardtime);
      }
      
      
      
    }
   //________________________________________________________________________________________________
   //Case 4: turn right
   else if (d1f>=(clearance+2) & d2r >= clearance ){
    
    forward(forwardtime*1.5);
    rightturn(turntime);
    int* ptr= sense(dist);
    d1f=ptr[0];
    d2r=ptr[1];
    d3l=ptr[2];
    d4b=ptr[3];
    if (d1f>= clearance*2-5){
      forward(turnforwardtime);
    }   
    else {
        forward(forwardtime);
    }
   }
}
