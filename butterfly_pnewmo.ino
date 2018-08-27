#include <SoftwareSerial.h>

int Step = 11;
int dir = 12;
bool state = false;


SoftwareSerial mySerial(2, 3); // RX, TXSoftwareSerial mySerial(6, 5); // RX, TX

void setup() {
  // put your setup code here, to run once:
  pinMode(Step, OUTPUT);
  pinMode(dir, OUTPUT);
  digitalWrite(dir, LOW);
  Serial.begin(9600);
  mySerial.begin(9600);
}

int start_del = 80;
int del = start_del;
int max_count = 1000;
int count = max_count;
int dir_ = 0;
int m_speed = 80;
bool last_state = false;
int high_speed = 100;
int low_speed=80;
long freq;



void loop() {
  if (mySerial.available() > 0) {
    String com = mySerial.readString();
    int str_l = com.length();
    Serial.println(com);
    

    if (com.substring(0, 4) == "com:") {
      int str_l = com.length();
      int last_separator=com.lastIndexOf(':');
    freq = com.substring(com.lastIndexOf(':')+1,com.length()).toInt();   
    com = com.substring(0,com.lastIndexOf(':'));
    last_separator=com.lastIndexOf(':');
    low_speed = com.substring(com.lastIndexOf(':')+1,com.length()).toInt();
    com = com.substring(0,com.lastIndexOf(':'));
    last_separator=com.lastIndexOf(':');
    high_speed = com.substring(com.lastIndexOf(':')+1,com.length()).toInt();
    if(high_speed<5)high_speed=5;
    Serial.print("freq - ");
    Serial.println(freq);
    Serial.print("Low_speed - ");
    Serial.println(low_speed);
    Serial.print("high_speed - ");
    Serial.println(high_speed);
    
      Serial.println("motor ON");
      max_count = freq;
      if(start_del<low_speed)del = low_speed;
      state = true;
      
    }

    if (com == "ON\n") {
      last_state = state;
      state = true;
      Serial.println("motor ON");
    }
    if (com == "OFF\n") {
      last_state = state;
      state = false;
      Serial.println("motor OFF");
    }

  }

  if (state ) {
    if (!last_state ) {
      if (high_speed < start_del) del = start_del;
      last_state = true;
      Serial.println("delay restarted");
    }
    count--;



    if (count == 0 && del > high_speed) {
      del--;
      count = max_count;
    }

    if(count == 0 && del <= high_speed){
      del=low_speed;
      count = max_count;
//      Serial.println("reached");
    }
    digitalWrite(Step, HIGH);
    delayMicroseconds(del);
    digitalWrite(Step, LOW);
    delayMicroseconds(del);

    if (count < 0)count = max_count;
  }
}

