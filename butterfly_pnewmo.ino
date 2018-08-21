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
int max_count = 100;
int count = max_count;
int dir_ = 0;
int m_speed = 80;
bool last_state = false;



void loop() {
  if (Serial.available() > 0) {
    String com = Serial.readString();
    Serial.println(com);
    if (com.substring(0, 4) == "com:") {
      int str_l = com.length();
      m_speed = com.substring(4, str_l).toInt();
      Serial.print(m_speed);
    }

    if (com == "ON") {
      last_state = state;
      state = true;
    }
    if (com == "OFF") {
      last_state = state;
      state = false;
    }

  }

  if (state = true) {
    if (last_state == false) {
      if (m_speed < start_del) del = start_del;
      last_state = true;
    }
    count--;



    if (count == 0 && del > m_speed) {
      del--;
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

