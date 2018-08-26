int Step = 4;
int dir = 5;
bool state = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(Step, OUTPUT);
  pinMode(dir, OUTPUT);
  digitalWrite(dir, LOW);
  Serial.begin(9600);
}

int start_del = 80;
int del = start_del;
int max_count = 1000;
int count = max_count;
int dir_ = 0;
int m_speed = 80;
bool last_state = false;



void loop() {
  if (Serial.available() > 0) {
    String com = Serial.readString();
    int str_l = com.length();
    Serial.println(com);
    

    if (com.substring(0, 4) == "com:") {
      int str_l = com.length();
      int last_separator=com.lastIndexOf(':');
    long freq = com.substring(com.lastIndexOf(':')+1,com.length()).toInt();   
    com = com.substring(0,com.lastIndexOf(':'));
    last_separator=com.lastIndexOf(':');
    int low_speed = com.substring(com.lastIndexOf(':')+1,com.length()).toInt();
    com = com.substring(0,com.lastIndexOf(':'));
    last_separator=com.lastIndexOf(':');
    int high_speed = com.substring(com.lastIndexOf(':')+1,com.length()).toInt();
    if(high_speed<5)high_speed=5;
    Serial.print("freq - ");
    Serial.println(freq);
    Serial.print("Low_speed - ");
    Serial.println(low_speed);
    Serial.print("high_speed - ");
    Serial.println(high_speed);
      max_count = freq;
      
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
    }
    digitalWrite(Step, HIGH);
    delayMicroseconds(del);
    digitalWrite(Step, LOW);
    delayMicroseconds(del);

    if (count < 0)count = max_count;
  }
}







//    if (count==0 && dir_==1){
//    del++;
//    count = max_count;
//  }
//  if(del<5)dir_=1;
//  if(del>80)dir_=0;
//
