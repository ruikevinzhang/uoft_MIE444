#include <NewPing.h>
#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial BTSerial (15, 14);

#define IR_R    40
#define IR_L    41

#define TRIGGER_PIN   53
#define ECHO_f     50
#define ECHO_fr    46
#define ECHO_fl    47
#define ECHO_b     24
#define ECHO_br    28
#define ECHO_bl    29
#define ECHO_bb    26
#define ECHO_top   35

#define yellow        33
#define red      32
#define green     34

#define IR_f   A1
#define IR_b   A0


#define MAX_DISTANCE 200

int d_f, d_fr, d_fl, d_br, d_bl, d_b, d_bb;
int color[2];
int dist[7];

int start =true;
bool flag=true;
bool check=true;
bool blockfound = false;
bool LZdone = false;

int block;
int emptyblock =16;
int frontdist=10;


NewPing sonar_f(TRIGGER_PIN, ECHO_f, MAX_DISTANCE);
NewPing sonar_fr(TRIGGER_PIN, ECHO_fr, MAX_DISTANCE);
NewPing sonar_fl(TRIGGER_PIN, ECHO_fl, MAX_DISTANCE);
NewPing sonar_b(TRIGGER_PIN, ECHO_b, MAX_DISTANCE);
NewPing sonar_br(TRIGGER_PIN, ECHO_br, MAX_DISTANCE);
NewPing sonar_bl(TRIGGER_PIN, ECHO_bl, MAX_DISTANCE);
NewPing sonar_bb(TRIGGER_PIN, ECHO_bb, MAX_DISTANCE);
NewPing sonar_top(TRIGGER_PIN, ECHO_top, MAX_DISTANCE);



//________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________

int* Ulsense(int *dist) {
  //sensor reading
  delay(100);
  //front sensor
  dist[0] = sonar_f.ping_cm();
  delay(100);
  //front sensor
  dist[1] = sonar_b.ping_cm();
  delay(100);
  //front right sensor
  dist[2] = sonar_fr.ping_cm();
  delay(100);
  //back right sensor
  dist[3] = sonar_br.ping_cm();
  delay(100);
  //front left sensor
  dist[4] = sonar_fl.ping_cm();
  delay(100);
  //back left sensor
  dist[5] = sonar_bl.ping_cm();
  delay(100);
  //back sensor
  dist[6] = sonar_bb.ping_cm();

  if (dist[0] == 0) {
    dist[0] = 99;
  }
  if (dist[1] == 0) {
    dist[1]  = 99;
  }
  if (dist[2] == 0) {
    dist[2]  = 99;
  }
  if (dist[3] == 0) {
    dist[3]  = 99;
  }
  if (dist[4] == 0) {
    dist[4]  = 99;
  }
  if (dist[5] == 0) {
    dist[5]  = 99;
  }
  if (dist[6] == 0) {
    dist[6]  = 99;
  }

  Serial.print(dist[0]);
  Serial.print(",");
  Serial.print(dist[1] );
  Serial.print(",");
  Serial.print(dist[2] );
  Serial.print(",");
  Serial.print(dist[3] );
  Serial.print(",");
  Serial.print(dist[4] );
  Serial.print(",");
  Serial.print(dist[5] );
  Serial.print(",");
  Serial.print(dist[6] );
  Serial.print(",");
  return dist;
}

//_______________________________________________________________________________________________________

int* Ulsense_wo(int *dist) {
  //sensor reading
  delay(100);
  //front sensor
  dist[0] = sonar_f.ping_cm();
  delay(100);
  //front sensor
  dist[1] = sonar_b.ping_cm();
  delay(100);
  //front right sensor
  dist[2] = sonar_fr.ping_cm();
  delay(100);
  //back right sensor
  dist[3] = sonar_br.ping_cm();
  delay(100);
  //front left sensor
  dist[4] = sonar_fl.ping_cm();
  delay(100);
  //back left sensor
  dist[5] = sonar_bl.ping_cm();
  delay(100);
  //back sensor
  dist[6] = sonar_bb.ping_cm();

  if (dist[0] == 0) {
    dist[0] = 99;
  }
  if (dist[1] == 0) {
    dist[1]  = 99;
  }
  if (dist[2] == 0) {
    dist[2]  = 99;
  }
  if (dist[3] == 0) {
    dist[3]  = 99;
  }
  if (dist[4] == 0) {
    dist[4]  = 99;
  }
  if (dist[5] == 0) {
    dist[5]  = 99;
  }
  if (dist[6] == 0) {
    dist[6]  = 99;
  }
  
  return dist;
}
//_____________________________________________________________________________________

int* IRsense(int *color) {
  int left_IR_state;
  int right_IR_state;
  left_IR_state = digitalRead(IR_L);
  right_IR_state = digitalRead(IR_R);
  color[0] = left_IR_state;
  color[1] = right_IR_state;

  Serial.print(color[1]);
  Serial.print(",");
  Serial.print(color[0] );
  Serial.print(",");

  return color;
}


int ir (){
  delay(1000);
  int block = analogRead(IR_f);
  //Serial.println(block);
  return block;
}

int ir_b (){
  delay(500);
  bool sense;
  int block = analogRead(IR_b);
  //Serial.println(block);
  if (block>=350){
    sense = true;
  }
 
  else{
    sense = false;
  }
  return sense;
}

//_________________________________________________________________________________________________________

void center_check(int * ptr, int clearance ) {

  straight_check(ptr, clearance*1.5);

  int dist[7];
  d_f = ptr[0];
  d_b = ptr[1];
  d_fr = ptr[2];
  d_br = ptr[3];
  d_fl = ptr[4];
  d_bl = ptr[5];
  d_bb = ptr[6];

  if (d_fr < clearance) { //右边有墙
    while (abs(d_fr - 8) > 2 & d_fr<clearance & d_f>clearance) { // 6<fr, fr>10 偏了
      if (d_fr > 10) {
        Serial.println('0');
        BTSerial.write('d');
        IRsense(color);
        Ulsense(dist);
        Serial.println('1');
        BTSerial.write('f');
        BTSerial.write('t');
      }
      else if (d_fr < 6) {
        Serial.println('0');
        BTSerial.write('a');
        IRsense(color);
        Ulsense(dist);
        Serial.println('1');
        BTSerial.write('f');
        BTSerial.write('t');
      }
      delay(1000);
      IRsense(color);
      int*ptr = Ulsense(dist);
      d_fr = ptr[2];
      d_f = ptr[0];
      straight_check(ptr, clearance);
    }
  }
  else if (d_fl < clearance) { //右边没墙 左边有墙
    while (abs(d_fl - 8) > 2 & d_fr<clearance & d_f>clearance) { // 6<fl<10 偏了
      if (d_fl >= 11) {
        Serial.println('0');
        BTSerial.write('a');
        IRsense(color);
        Ulsense(dist);
        Serial.println('1');
        BTSerial.write('f');
        BTSerial.write('t');

      }
      else if (d_fr < 5) {
        Serial.println('0');
        BTSerial.write('d');
        IRsense(color);
        Ulsense(dist);
        Serial.println('1');
        BTSerial.write('f');
        BTSerial.write('t');
      }
      delay(1000);
      IRsense(color);
      int*ptr = Ulsense(dist);
      d_fl = ptr[4];
      d_f = ptr[0];
      straight_check(ptr, clearance);
    }
  }
}

//________________________________________________________________________________________________________

void straight_check(int * ptr, int clearance) {
  int diff, diff_new;

  int dist[7];
  d_f = ptr[0];
  d_b = ptr[1];
  d_fr = ptr[2];
  d_br = ptr[3];
  d_fl = ptr[4];
  d_bl = ptr[5];
  d_bb = ptr[6];

  if (d_fr < clearance & d_br < clearance) { //右边有墙
    while (abs(d_fr - d_br) >= 2) { //右边有墙 前后差>2
      if (d_fr > d_br) {
        Serial.println('0');
        BTSerial.write('d');
        BTSerial.write('t');
      }
      else if (d_br > d_fr) {
        Serial.println('0');
        BTSerial.write('a');
        BTSerial.write('t');
      }
      delay(500);
      IRsense(color);
      int*ptr = Ulsense(dist);
      d_f = ptr[0];
      d_b = ptr[1];
      d_fr = ptr[2];
      d_br = ptr[3];
      d_fl = ptr[4];
      d_bl = ptr[5];
    }
  }

  else if (d_fl < clearance & d_bl < clearance) { //右边没有  左边有
    while (abs(d_fl - d_bl) >= 2) {

      if (d_fl > d_bl) {
        BTSerial.write('a');
        BTSerial.write('t');
        Serial.println('0');
      }
      else if (d_bl > d_fl) {
        BTSerial.write('d');
        BTSerial.write('t');
        Serial.println('0');
      }
      delay(500);
      IRsense(color);
      int*ptr = Ulsense(dist);
      d_f = ptr[0];
      d_b = ptr[1];
      d_fr = ptr[2];
      d_br = ptr[3];
      d_fl = ptr[4];
      d_bl = ptr[5];
    }
  }
  else if (d_f<=emptyblock*3 & d_b<=emptyblock*3){
    bool dir = true;
    while ((abs(d_f - d_b) >= 3) & (!(d_fr < clearance & d_br < clearance) | !(d_fl < clearance & d_bl < clearance)) ) {
      diff = abs(d_f - d_b);
      if ((d_f > d_b & dir) | (d_b > d_f & !dir)) {
        BTSerial.write('a');
        BTSerial.write('t');
        Serial.println('0');
      }
      else if ((d_f > d_b & !dir) | (d_b > d_f & dir)) {
        BTSerial.write('d');
        BTSerial.write('t');
        Serial.println('0');
      }
      delay(500);

      int d_fr_old = d_fr;
      
      IRsense(color);
      int*ptr = Ulsense(dist);
      d_f = ptr[0];
      d_b = ptr[1];
      d_fr = ptr[2];
      d_br = ptr[3];
      d_fl = ptr[4];
      d_bl = ptr[5];

      //fr距离变大，说明方向错了，fr应该变小，因为由不垂直变垂直？
      if (d_fr >= d_fr_old & dir == true) {
        dir = false;
      }
    }
  }

}

//_______________________________________________________________________________________________________________________

void straight_check_wo(int * ptr, int clearance) {
  int diff, diff_new;

  int dist[7];
  d_f = ptr[0];
  d_b = ptr[1];
  d_fr = ptr[2];
  d_br = ptr[3];
  d_fl = ptr[4];
  d_bl = ptr[5];
  d_bb = ptr[6];

  if (d_fr < clearance & d_br < clearance) { //右边有墙
    while (abs(d_fr - d_br) >= 2) { //右边有墙 前后差>2
      if (d_fr > d_br) {
        //Serial.println('0');
        BTSerial.write('d');
        BTSerial.write('t');
      }
      else if (d_br > d_fr) {
        //Serial.println('0');
        BTSerial.write('a');
        BTSerial.write('t');
      }
      delay(500);
      //IRsense(color);
      int*ptr = Ulsense_wo(dist);
      d_f = ptr[0];
      d_b = ptr[1];
      d_fr = ptr[2];
      d_br = ptr[3];
      d_fl = ptr[4];
      d_bl = ptr[5];
    }
  }

  else if (d_fl < clearance & d_bl < clearance) { //右边没有  左边有
    while (abs(d_fl - d_bl) >= 2) {

      if (d_fl > d_bl) {
        BTSerial.write('a');
        BTSerial.write('t');
        //Serial.println('0');
      }
      else if (d_bl > d_fl) {
        BTSerial.write('d');
        BTSerial.write('t');
        //Serial.println('0');
      }
      delay(500);
      //IRsense(color);
      int*ptr = Ulsense_wo(dist);
      d_f = ptr[0];
      d_b = ptr[1];
      d_fr = ptr[2];
      d_br = ptr[3];
      d_fl = ptr[4];
      d_bl = ptr[5];
    }
  }
  else if(d_f<=emptyblock*4 & d_b<=emptyblock*4){
    bool dir = true;
    while (abs(d_f - d_b) >= 3) {
      diff = abs(d_f - d_b);
      if ((d_f > d_b & dir) | (d_b > d_f & !dir)) {
        BTSerial.write('a');
        BTSerial.write('t');
        //Serial.println('0');
      }
      else if ((d_f > d_b & !dir) | (d_b > d_f & dir)) {
        BTSerial.write('d');
        BTSerial.write('t');
        //Serial.println('0');
      }
      delay(500);

      int d_fr_old = d_fr;
      
      //IRsense(color);
      int*ptr = Ulsense_wo(dist);
      d_f = ptr[0];
      d_b = ptr[1];
      d_fr = ptr[2];
      d_br = ptr[3];
      d_fl = ptr[4];
      d_bl = ptr[5];

      //fr距离变大，说明方向错了，fr应该变小，因为由不垂直变垂直？
      if (d_fr >= d_fr_old & dir == true) {
        dir = false;
      }
    }
  }
  else if (d_f<=emptyblock*3 & d_b<=emptyblock*3){
    bool dir = true;
    while ((abs(d_f - d_b) >= 3) & (!(d_fr < clearance & d_br < clearance) | !(d_fl < clearance & d_bl < clearance)) ) {
      diff = abs(d_f - d_b);
      if ((d_f > d_b & dir) | (d_b > d_f & !dir)) {
        BTSerial.write('a');
        BTSerial.write('t');
        Serial.println('0');
      }
      else if ((d_f > d_b & !dir) | (d_b > d_f & dir)) {
        BTSerial.write('d');
        BTSerial.write('t');
        Serial.println('0');
      }
      delay(500);

      int d_fr_old = d_fr;
      
      IRsense(color);
      int*ptr = Ulsense(dist);
      d_f = ptr[0];
      d_b = ptr[1];
      d_fr = ptr[2];
      d_br = ptr[3];
      d_fl = ptr[4];
      d_bl = ptr[5];

      //fr距离变大，说明方向错了，fr应该变小，因为由不垂直变垂直？
      if (d_fr >= d_fr_old & dir == true) {
        dir = false;
      }
    }
  }
}


//_______________________________________________________________________________________________

void initial_check(int * ptr, int clearance) {

 int count = 0;
  int dist[7];
  d_f = ptr[0];
  d_b = ptr[1];
  d_fr = ptr[2];
  d_br = ptr[3];
  d_fl = ptr[4];
  d_bl = ptr[5];
  d_bb = ptr[6];
  
    if (abs(d_fr - d_br) < 2){
      count=count+1;
    }
    if (abs(d_fl - d_bl) < 2){
      count=count+1;
    }
    if (abs(d_f - d_b) <=2){
      count=count+1;
    }   
  
  while (count<2){
    Serial.println('0');
    BTSerial.write('d');
    BTSerial.write('t');
    delay(500);
    
    IRsense(color);
    int*ptr = Ulsense(dist);
    d_f = ptr[0];
    d_b = ptr[1];
    d_fr = ptr[2];
    d_br = ptr[3];
    d_fl = ptr[4];
    d_bl = ptr[5];
    d_bb = ptr[6];

    count = 0;
    
    if (abs(d_fr - d_br) < 2){
      count=count+1;
    }
    if (abs(d_fl - d_bl) < 2){
      count=count+1;
    }
    if (abs(d_f - d_b) <=2){
      count=count+1;
    }    
       
  }
}

//________________________________________________________________________________________________________________

void align_check(int * ptr, int clearance) {

 int count = 0;
 int dist[7];
  d_f = ptr[0];
  d_b = ptr[1];
  d_fr = ptr[2];
  d_br = ptr[3];
  d_fl = ptr[4];
  d_bl = ptr[5];
  d_bb = ptr[6];
  

    if (abs(d_f - d_b) <=2){
      count=count+1;
      if (abs(d_fr - d_br) < 2){
        count=count+1;
      }
      if (abs(d_fl - d_bl) < 2){
        count=count+1;
      }
    }   
  
  while (count<2){
    //Serial.println('0');
    BTSerial.write('d');
    BTSerial.write('t');
    delay(500);
    //Serial.println("adjustng");
    
    int*ptr = Ulsense_wo(dist);
    d_f = ptr[0];
    d_b = ptr[1];
    d_fr = ptr[2];
    d_br = ptr[3];
    d_fl = ptr[4];
    d_bl = ptr[5];
    d_bb = ptr[6];

    count = 0;
    
    if (abs(d_f - d_b) <=2){
      count=count+1;
      if (abs(d_fr - d_br) < 2){
        count=count+1;
      }
      if (abs(d_fl - d_bl) < 2){
        count=count+1;
      }
    }       
       
  }

}


//______________________________________________________________________________________________________________________

void searching (){
  bool turnonce = false;
  bool uonce = false;
  bool backonce = false;
  int counter=0;
  
  int*ptr = Ulsense_wo(dist);
  d_f = ptr[0];
  d_b = ptr[1];
  d_fr = ptr[2];
  d_br = ptr[3];
  d_fl = ptr[4];
  d_bl = ptr[5];
  d_bb = ptr[6];
  
  block = ir();
  if (block>235 & d_f>12 & d_b>12){
    //Serial.println("found initial");
    blockfound = true;
    BTSerial.write('d');
    BTSerial.write('t');
    delay(100);
    return;
  }
  if (!blockfound){

    while (d_br<=emptyblock){
      BTSerial.write('f');
      BTSerial.write('t');
      //Serial.println("forward to center");
      delay(500);
      ptr = Ulsense_wo(dist);
      d_br = ptr[3];
      straight_check_wo(ptr, emptyblock*2);
    }
    BTSerial.write('f');
    BTSerial.write('t');
    //Serial.println("forward to center");
    delay(500);
    ptr = Ulsense_wo(dist);
    straight_check_wo(ptr, emptyblock*2);
    BTSerial.write('w');
    BTSerial.write('t');
    delay(200);
    ptr = Ulsense_wo(dist);
    straight_check_wo(ptr, emptyblock*2);

    
    block = ir();
    if (block>235& d_f>12 & d_b>12){
      //Serial.println("found forward to center");
      blockfound = true;
      BTSerial.write('d');
      BTSerial.write('t');
      //Serial.println("right");
      delay(100);
      return;
    }
  }
  
  if (d_fr<emptyblock&!blockfound){
      uonce=true;
  }
   
  while (!blockfound){
    
      straight_check_wo(ptr, emptyblock*2);
     
      if(((d_fl<=emptyblock&d_f>=emptyblock)|d_bb<=emptyblock)&!turnonce){
          while (counter<=12){
            BTSerial.write('d');
            BTSerial.write('t');
            //Serial.println("right");
            delay(100);
            
            block = ir();
            if (block>235& d_f>12 & d_b>12){
              //Serial.println("found rightturn");
              blockfound = true;
              BTSerial.write('d');
              BTSerial.write('t');
              //Serial.println("right");
              delay(100);
              return;
            }       
                 
            int*ptr = Ulsense_wo(dist);
            d_f = ptr[0];
            d_b = ptr[1];
            d_fr = ptr[2];
            d_br = ptr[3];
            d_fl = ptr[4];
            d_bl = ptr[5];
            d_bb = ptr[6];
            counter=counter+1;
          }
          turnonce=true;   
      }
      else if((d_f<=frontdist|(d_f>=30&d_fl>=30&d_bb>=30))& (!uonce)){
          BTSerial.write('u');
          BTSerial.write('t');
          //Serial.println("uturn");
          delay(3000);
          int*ptr = Ulsense_wo(dist);
          d_f = ptr[0];
          d_b = ptr[1];
          d_fr = ptr[2];
          d_br = ptr[3];
          d_fl = ptr[4];
          d_bl = ptr[5];
          d_bb = ptr[6];         
          uonce=true;
      }
      else{
        while(d_f>=frontdist&!backonce){
            straight_check_wo(ptr, emptyblock);
            if (d_f<15){
              BTSerial.write('w');
              delay(100);
              block = ir();
              if (block>235 & d_f>12 & d_b>12){
                blockfound = true;
                BTSerial.write('d');
                BTSerial.write('t');
                //Serial.println("right");
                delay(100);
                return;
              } 
            }
            else{
              BTSerial.write('f');
              delay(100);
              block = ir();
              if (block>235 & d_f>12 & d_b>12){ 
                blockfound = true;
                BTSerial.write('d');
                BTSerial.write('t');
                //Serial.println("right");
                delay(100);
                return;
              } 
            }
            for (int i=1;i<=5;i++){
              BTSerial.write('a');
              BTSerial.write('t');
              delay(100);
              block = ir();
              if (block>235 & d_f>12 & d_b>12){
                //Serial.println("found forward");
                blockfound = true;
                BTSerial.write('d');
                BTSerial.write('t');
                //Serial.println("right");
                delay(100);
                return;
              } 
            }
            for (int i=1;i<=5;i++){
              BTSerial.write('d');
              BTSerial.write('t');
            }             
            
            
            
            //Serial.println("forward");
            delay(1000);

            block = ir();
            if (block>235 & d_f>12 & d_b>12){
              //Serial.println("found forward");
              blockfound = true;
              BTSerial.write('d');
              BTSerial.write('t');
              //Serial.println("right");
              delay(100);
              return;
            } 
            
            int*ptr = Ulsense_wo(dist);
            d_f = ptr[0];
            d_b = ptr[1];
            d_fr = ptr[2];
            d_br = ptr[3];
            d_fl = ptr[4];
            d_bl = ptr[5];
            d_bb = ptr[6];            
            
            while(d_fl<=emptyblock & d_fr<=emptyblock){
                straight_check_wo(ptr, emptyblock);
                BTSerial.write('b');
                BTSerial.write('t');
                //Serial.println("backward");
                delay(500);
                int*ptr = Ulsense_wo(dist);
                d_f = ptr[0];
                d_b = ptr[1];
                d_fr = ptr[2];
                d_br = ptr[3];
                d_fl = ptr[4];
                d_bl = ptr[5];
                d_bb = ptr[6];
                backonce=true;
            }
         }
        turnonce=false;
        uonce=false;
        backonce=false;
        counter=0;
      }     
 
   }
}

//_____________________________________________________________________________________________________________________________


void pickup(){
  int d_top_w;
  bool pickup=false;
  int ad_count = 1;
  bool pickup_sense = false;
  int block;

  while(!pickup){
    BTSerial.write('w');
    d_top_w = sonar_top.ping_cm();
    //Serial.println(d_top_w);
    ad_count=ad_count+1;
    if (d_top_w<=18){
      while (!pickup_sense){
        //Serial.println("top");
        BTSerial.write('w');
        pickup=true;
        BTSerial.write('m');
        delay(4000);
        BTSerial.write('q');
        pickup_sense = ir_b();
      }
    }
    else if (ad_count==3){
      while (!pickup_sense){
        //Serial.println("count");
        BTSerial.write('w');
        pickup=true;
        BTSerial.write('m');
        delay(4000);
        BTSerial.write('q');
        pickup_sense = ir_b();
      }
    }
  }
  //Serial.println("done");
}


//_______________________________________________________________________________________________________________________

void exitLZ(){
  bool arrive = false;

  int*ptr = Ulsense_wo(dist);
  d_f = ptr[0];
  d_b = ptr[1];
  d_fr = ptr[2];
  d_br = ptr[3];
  d_fl = ptr[4];
  d_bl = ptr[5];
  d_bb = ptr[6];

  align_check(ptr,2*emptyblock); //align with two walls

  ptr = Ulsense_wo(dist);
  d_f = ptr[0];
  d_b = ptr[1];
  d_fr = ptr[2];
  d_br = ptr[3];
  d_fl = ptr[4];
  d_bl = ptr[5];
  d_bb = ptr[6];
  
  while(!(d_f<(frontdist)|d_b<(frontdist)) & !(d_fl<emptyblock & d_fr<emptyblock)){  //go straight blindly until in front of a wall
    straight_check_wo(ptr,2*emptyblock);
    BTSerial.write('f');
    BTSerial.write('t');
    //Serial.println("keep forward");
    int*ptr = Ulsense_wo(dist);
    d_f = ptr[0];
    d_b = ptr[1];
    d_fr = ptr[2];
    d_br = ptr[3];
    d_fl = ptr[4];
    d_bl = ptr[5];
    d_bb = ptr[6];
  }

  ptr = Ulsense_wo(dist);
  d_f = ptr[0];
  d_b = ptr[1];
  d_fr = ptr[2];
  d_br = ptr[3];
  d_fl = ptr[4];
  d_bl = ptr[5];
  d_bb = ptr[6];
  
  if(d_fl<emptyblock & d_fr<emptyblock){  //if left the loading zone, either at correct location or have to move backward
    if(d_br<emptyblock){ //correct final location
      BTSerial.write('w');
      BTSerial.write('t');
      //Serial.println("we are here");
    }else{
      while(d_fr<emptyblock){ //move backward into the loading zone
        straight_check_wo(ptr,2*emptyblock);
        BTSerial.write('b');
        BTSerial.write('t');
       // Serial.println("go back to loading zone,wrong exit");
        
        ptr = Ulsense_wo(dist);
        d_f = ptr[0];
        d_b = ptr[1];
        d_fr = ptr[2];
        d_br = ptr[3];
        d_fl = ptr[4];
        d_bl = ptr[5];
        d_bb = ptr[6];
      }
      straight_check_wo(ptr,2*emptyblock);
      BTSerial.write('l');  //not quite sure.. assuming the robot aligns itself facing 0 degree, turn left to face the wall
      BTSerial.write('t');
      //Serial.println("turn left");
      delay(1500);
      ptr = Ulsense_wo(dist);
      d_f = ptr[0];
      d_b = ptr[1];
      d_fr = ptr[2];
      d_br = ptr[3];
      d_fl = ptr[4];
      d_bl = ptr[5];
      d_bb = ptr[6];
    }
  }else{        //somewhere inside LZ
    while((d_fl>emptyblock | d_fr>emptyblock) & !arrive){ //while not at final location, always left turn at a wall
      while(d_f>(frontdist-2) & !arrive){
        straight_check_wo(ptr,2*emptyblock);
        BTSerial.write('f');
        BTSerial.write('t');
        //Serial.println("go straight");
        delay(500);
        ptr = Ulsense_wo(dist);
        d_f = ptr[0];
        d_b = ptr[1];
        d_fr = ptr[2];
        d_br = ptr[3];
        d_fl = ptr[4];
        d_bl = ptr[5];
        d_bb = ptr[6];
        if(d_fl<emptyblock & d_fr<emptyblock){
          arrive = true;
        }
        
      }
      if (!arrive){
        straight_check_wo(ptr,2*emptyblock);
        BTSerial.write('l');
        BTSerial.write('t');
        //Serial.println("turn left");
        delay(2000);
        ptr = Ulsense_wo(dist);
        d_f = ptr[0];
        d_b = ptr[1];
        d_fr = ptr[2];
        d_br = ptr[3];
        d_fl = ptr[4];
        d_bl = ptr[5];
        d_bb = ptr[6];
      }
      
    }
  }
  ptr = Ulsense_wo(dist);
  d_fl = ptr[4];
  while (d_fl<=emptyblock){
    BTSerial.write('s');
    BTSerial.write('t');
    delay(200);
    ptr = Ulsense_wo(dist);
    d_fl = ptr[4];
  }
  BTSerial.write('s');
  BTSerial.write('t');
  delay(200);
  BTSerial.write('s');
  BTSerial.write('t');
  delay(200);
  BTSerial.write('s');
  BTSerial.write('t');
  delay(200);
  BTSerial.write('s');
  BTSerial.write('t');
  delay(200);
  //Serial.println("we are here");
}

//______________________________________________________________________________________________________________


void setup() {
  Serial.begin(9600);
  BTSerial.begin (9600);

  pinMode(red, OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(yellow,OUTPUT);

  digitalWrite(red,LOW);
  digitalWrite(green,LOW);
  digitalWrite(yellow,LOW);
  
  delay(5000);
}
//________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________

void loop() {
  
  if (Serial.available()> 0){
    int ave =Serial.available();
    //Serial.print(ave);
    //Serial.print(" ");
    
    int state = Serial.read();
    //Serial.print(state);
  
    flag = false;
    
 
    
    if (state=='9'){//localized{
      digitalWrite(red,HIGH);
      delay(3000);
      digitalWrite(red,LOW);
      IRsense(color);
      Ulsense(dist);
      Serial.println('9');
    }
    
    else if (state=='8'){ //loading zone 
      digitalWrite(yellow,HIGH);
      //delay(2000);
      //IRsense(color);
      //Ulsense(dist);
      //Serial.println('8');
    }
    
    else if (state=='7'){ //offloading zone
      digitalWrite(green,HIGH);
      int* ptr = Ulsense_wo(dist);
      d_bb = ptr[6];
      d_fr = ptr[2];
      d_fl = ptr[4];
      
      if (d_fr>=d_fl){
        while(d_bb<emptyblock){
          BTSerial.write('r');
          BTSerial.write('t');
          delay(500);
          int* ptr = Ulsense_wo(dist);
          d_bb = ptr[6];
          straight_check_wo(ptr, emptyblock*2);
        }
      }
      else{
        while(d_bb<emptyblock){
          BTSerial.write('l');
          BTSerial.write('t');
          delay(500);
          int* ptr = Ulsense_wo(dist);
          d_bb = ptr[6];
          straight_check_wo(ptr, emptyblock*2);
        }
      } 
        
      
      for (int i=1;i<=5;i++){
        BTSerial.write('s');
        BTSerial.write('t');
      }
      
      BTSerial.write('g');
      delay(10000);
      BTSerial.write('q');
      
      digitalWrite(red,HIGH);
      digitalWrite(yellow,HIGH);

      exit(0);
      //BTSerial.write('b');
      //IRsense(color);
      //Ulsense(dist);
      //Serial.println('7');
    }

    else if (state=='z'){ 
      searching();
      digitalWrite(yellow,LOW);
      delay(1000);
      digitalWrite(yellow,HIGH);
      delay(1000);
      digitalWrite(yellow,LOW);
      delay(1000);
      digitalWrite(yellow,HIGH);
      delay(1000);
      digitalWrite(yellow,LOW);
      delay(1000);
      digitalWrite(yellow,HIGH);
      delay(1000);
      
      //Serial.println("Ready to pickup");
      pickup();
      digitalWrite(yellow,LOW);
      delay(1000);
      digitalWrite(yellow,HIGH);
      delay(1000);
      digitalWrite(yellow,LOW);
      delay(1000);
      digitalWrite(yellow,HIGH);
      delay(1000);
      digitalWrite(yellow,LOW);
      delay(1000);
      digitalWrite(yellow,HIGH);
      delay(1000);
      
      //Serial.println("Going to LZ exit now");
      exitLZ();
      //Serial.println("Ready to exit LZ");
      digitalWrite(yellow,LOW);
      
      digitalWrite(green,HIGH);
      delay(1000);
      digitalWrite(green,LOW);
      delay(1000);
      digitalWrite(green,HIGH);
      delay(1000);
      digitalWrite(green,LOW);
      delay(1000);
      digitalWrite(green,HIGH);
      delay(1000);
      digitalWrite(green,LOW);
      delay(1000);
      
      IRsense(color);
      Ulsense(dist);
      Serial.println('7');
    }
    
    else if (state=='1'){
        IRsense(color);
        int*ptr = Ulsense(dist);
        d_f = ptr[0];
        d_b = ptr[1];
        d_fr = ptr[2];
        d_br = ptr[3];
        d_fl = ptr[4];
        d_bl = ptr[5];
        d_bb = ptr[6];     
        center_check(ptr, emptyblock );
        if (d_f >= emptyblock) {
          BTSerial.write('f');
          BTSerial.write('t');
          Serial.println('1');
          delay(1000);
          //IRsense(color);
          //Ulsense(dist);
        } 
        else if (d_f>=frontdist){
          BTSerial.write('w');
          BTSerial.write('t');
          Serial.println('4'); //not able to go forward
        }
        else {
          Serial.println('5');
        }
    }
    else if (state=='2'){
        int*ptr = Ulsense_wo(dist);
        d_f = ptr[0];   
        while (d_f>frontdist & d_f<=emptyblock){
          BTSerial.write('w');
          BTSerial.write('t');
          delay(100);
          ptr = Ulsense_wo(dist);
          d_f = ptr[0];
        }
        delay(500);
        IRsense(color);
        ptr = Ulsense(dist);
        d_f = ptr[0];
        d_b = ptr[1];
        d_fr = ptr[2];
        d_br = ptr[3];
        d_fl = ptr[4];
        d_bl = ptr[5];
        d_bb = ptr[6];     
        BTSerial.write('r');
        BTSerial.write('t');
        Serial.println('2');
        delay(3000);
    }
    else if (state=='3'){
        int*ptr = Ulsense_wo(dist);
        d_f = ptr[0];   
        while (d_f>frontdist & d_f<=emptyblock){
          BTSerial.write('w');
          BTSerial.write('t');
          delay(100);
          ptr = Ulsense_wo(dist);
          d_f = ptr[0];
        }
        
        IRsense(color);
        ptr = Ulsense(dist);
        d_f = ptr[0];
        d_b = ptr[1];
        d_fr = ptr[2];
        d_br = ptr[3];
        d_fl = ptr[4];
        d_bl = ptr[5];
        d_bb = ptr[6];     
        BTSerial.write('l');
        BTSerial.write('t');
        Serial.println('3');
        delay(3000);
    }
    
    else if (state=='6'){
        IRsense(color);
        int*ptr = Ulsense(dist);
        d_f = ptr[0];
        d_b = ptr[1];
        d_fr = ptr[2];
        d_br = ptr[3];
        d_fl = ptr[4];
        d_bl = ptr[5];
        d_bb = ptr[6];     
        BTSerial.write('u');
        BTSerial.write('t');
        delay(3000);
        Serial.println('6');//uturn
    }
    
   
   
  } 
    
    
   


  
  
  if (flag==true){
    IRsense(color);
    int*ptr = Ulsense(dist);
    d_f = ptr[0];
    d_b = ptr[1];
    d_fr = ptr[2];
    d_br = ptr[3];
    d_fl = ptr[4];
    d_bl = ptr[5];
    d_bb = ptr[6];
  
   if (start){
    
    initial_check(ptr,emptyblock);
    start=false;
   }
   
   if (d_f <= emptyblock) {
    //________________________________________________________________________________________________
    //Case 1: turn left
    if (d_fr <= emptyblock) {
      while (d_f >= frontdist) {
        //move forward until there is less than 5 cm
        straight_check(ptr, emptyblock);
        
        BTSerial.write('w'); //forward adjust
        Serial.println('4');
        delay(500);
        IRsense(color);
        int*ptr = Ulsense(dist);
        d_f = ptr[0];
      }
      bool keepright=false;
      while (d_f <= emptyblock) {
        //turn left until forward path is clear (>15 cm) //3sides block
        if (d_fl < 7 | keepright) {
          BTSerial.write('r');
          BTSerial.write('t');//left 90
          Serial.println('2');
          
          keepright=true;
        }
        else {
          BTSerial.write('l');
          BTSerial.write('t');//left 90
          Serial.println('3');
        }
        delay(3000);

        IRsense(color);
        int*ptr = Ulsense(dist);
        d_f = ptr[0];

      }

    }
    //________________________________________________________________________________________________
    //Case 2: turn right
    else {
      while (d_f >= frontdist) {
        //move forward until there is less than 5 cm
        straight_check(ptr, emptyblock);
        BTSerial.write('w'); //forward adjust
        Serial.println('4');
        delay(500);
        IRsense(color);
        int*ptr = Ulsense(dist);
        d_f = ptr[0];
      }
      BTSerial.write('r');
      BTSerial.write('t');//right 90
      Serial.println('2');
      IRsense(color);
      Ulsense(dist);

      while (d_fr >= emptyblock) {
        BTSerial.write('w');
        Serial.println('4');//forward adjust
        delay(1000);
        IRsense(color);
        int*ptr = Ulsense(dist);
        d_fr = ptr[2];
      }
    }
  }
  //________________________________________________________________________________________________
  //case 3: move forward
  else if (d_f >= emptyblock & d_fr <= emptyblock ) {

    center_check(ptr, emptyblock );
    if (d_f >= emptyblock) {
      BTSerial.write('f');
      BTSerial.write('t');
      Serial.println('1');
      delay(1000);
      IRsense(color);
      Ulsense(dist);
    }
  }
  else if (d_f >= emptyblock & d_fr >= emptyblock ) {
    //Case 4: turn right
    int adjust = 0;
    while (d_br < emptyblock) {
      //straight_check(ptr, emptyblock);
      BTSerial.write('w'); //forward adjust
      delay(500);
      Serial.println('4');
      IRsense(color);
      int*ptr = Ulsense(dist);
      d_br = ptr[3];

      adjust = 1;
    }
    if (adjust == 1) {
      //straight_check(ptr, emptyblock);
      BTSerial.write('f');
      Serial.println('1');
      delay(500);
      IRsense(color);
      Ulsense(dist);
      BTSerial.write('w');
      Serial.println('4');
      delay(500);
      IRsense(color);
      Ulsense(dist);
    }

    BTSerial.write('r');
    BTSerial.write('t');
    Serial.println('2');
    IRsense(color);
    Ulsense(dist);

    while (d_fr > emptyblock) {
      //straight_check(ptr, emptyblock);
      BTSerial.write('w');
      Serial.println('4');//forward adjust
      delay(1000);
      IRsense(color);
      int*ptr = Ulsense(dist);
      d_fr = ptr[2];
    }

  }
  Serial.println('0');
  }
}
