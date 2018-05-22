




//#include <RemoteSwitch.h>
#include <SoftwareSerial.h>
#include <Nextion.h>


//#define RF1ON 266325
//#define RF1OFF 266321
//#define PERIOD 146;
int s1 = 0;
int s2 = 0;


int s1_pin = 11;
int s2_pin = 9;
int b1 = 10;
int b2 = 8;
int b3 = 7;

int b1_st = 0;
int b2_st = 0;
int b3_st = 0;

int period = 150;

long current_time = 0;
int pressure = 0;
int freq = 0;
int Time = 0;




SoftwareSerial nextion(2, 3);// Nextion TX to pin 2 and RX to pin 3 of Arduino

Nextion myNextion(nextion, 9600); //create a Nextion object named myNextion using the nextion serial port @ 9600bps

void setup() {
  Serial.begin(9600);
  
  TCCR1A = TCCR1A & 0xe0 | 1;
  TCCR1B = TCCR1B & 0xe0 | 0x09;
  myNextion.init();
//  pinMode(s1_pin, OUTPUT);
//  pinMode(s2_pin, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(b2, OUTPUT);
  pinMode(b3, OUTPUT);
  digitalWrite(b1, b1_st);
  digitalWrite(b2, b2_st);
  digitalWrite(b3, b3_st);



}

void loop() {




  

  String message = myNextion.listen(); //check for message
//  Serial.println(message);
  if (message != "") { // if a message is received...
    Serial.println(message); //...print it out
  }
  if (message == "65 4 5 1 ffff ffff ffff") {
    b1_st = !b1_st;
    digitalWrite(b1, b1_st);
    //    Serial.println(b1_st);
  }


 if (message == "65 3 7 1 ffff ffff ffff" || message == "65 3 4 1 ffff ffff ffff") {
      pressure = myNextion.getComponentValue("page3.n0");
      Serial.print("pressure = ");
      Serial.println(pressure);
  }


  if (message == "65 3 8 1 ffff ffff ffff" || message == "65 3 5 1 ffff ffff ffff") {
      freq = myNextion.getComponentValue("page3.n1");
      Serial.print("frequency = ");
      Serial.println(freq);
  }


  if (message == "65 3 6 1 ffff ffff ffff" || message == "65 3 9 1 ffff ffff ffff") {
      Time = myNextion.getComponentValue("page3.n2");
      Serial.print("time = ");
      Serial.println(Time);
  }


 
 if (millis() - current_time > period){
    int slid1 = myNextion.getComponentValue("page4.h0");
     current_time = millis();
    if (s1 != slid1) {
      s1 = slid1;
      analogWrite(s1_pin, s1);
      Serial.print("s1 = ");
      Serial.println(s1);
      
    }
  

  
    int slid2 = myNextion.getComponentValue("page4.h1");
    if (s2 != slid2) {
      s2 = slid2;
      analogWrite(s2_pin, s2);
      Serial.print("s2 = ");
      Serial.println(s2);
    }
 }
  

  //65 4 5 1 ffff ffff ffff
  //65 4 5 0 ffff ffff ffff

  //65 4 3 1 ffff ffff ffff
  //65 4 3 0 ffff ffff ffff


  

// int slid1 = myNextion.getComponentValue("page4.h0");
}






































//  if (message == "65 4 1 1 ffff ffff ffff" ||  message == "65 4 5 0 ffff ffff ffff") {
//    int slid1 = myNextion.getComponentValue("page4.h0");
//    if (s1 != slid1) {
//      s1 = slid1;
//      analogWrite(s1_pin, s1);
//      Serial.print("s1 = ");
//      Serial.println(s1);
//    }
//  }
//
//  if (message == "65 4 3 1 ffff ffff ffff" ||  message == "65 4 3 0 ffff ffff ffff") {
//    int slid2 = myNextion.getComponentValue("page4.h1");
//    if (s2 != slid2) {
//      s2 = slid2;
//      analogWrite(s2_pin, s2);
//      Serial.print("s2 = ");
//      Serial.println(s2);
//    }
//  }
