


#include"timer-api.h"


#include <SoftwareSerial.h>
#include <Nextion.h>


#define RF1ON 266325
#define RF1OFF 266321
#define PERIOD 146;
int s1 = 0;
int s2 = 0;

int strt_s;
int s1_pin = 11;
int s2_pin = 9;
int b1 = 10;
int b2 = 8;
int b3 = 7;

int b1_st = 0;
int b2_st = 0;
int b3_st = 0;

bool start_st = true;

int period = 150;

long current_time = 0;
int pressure = 0;
int freq = 0;
int Time = 0;

int slid1 = 0;
int slid2 = 0;

long i = 50;
int dir = 12;

long n = 800;

bool motor_on = true;


//подключаем дисплей через софтварный сериальник

SoftwareSerial nextion(2, 3);// Nextion TX to pin 2 and RX to pin 3 of Arduino
//SoftwareSerial mySerial(10,11);

Nextion myNextion(nextion, 9600); //create a Nextion object named myNextion using the nextion serial port @ 9600bps
//Nextion myNextion(Serial, 9600); //create a Nextion object named myNextion using the nextion serial port @ 9600bps
void setup() {
  Serial.begin(9600);
//  mySerial.begin(9600);
//  timer_init_ISR_20KHz(TIMER_DEFAULT);    //starts timer


//повышаем частоту шим до максимальной (чтобы было больше похоже на аналоговый сигнал,нужно для более корректной работы двигателя(который создает давление воздуха)
  TCCR1A = TCCR1A & 0xe0 | 1;
  TCCR1B = TCCR1B & 0xe0 | 0x09;
  myNextion.init();
  pinMode(dir, OUTPUT);
  pinMode(s1_pin, OUTPUT);
  pinMode(s2_pin, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(b2, OUTPUT);
  pinMode(b3, OUTPUT);
  digitalWrite(b1, b1_st);
  digitalWrite(b2, b2_st);
  digitalWrite(b3, b3_st);
  digitalWrite(dir, LOW);



}

void loop() {
  //если кнопка старт нажата ( в интерфейсе дисплея) то подаем на драйвер шим в противном случае выкл шим
//  if (start_st) {
////    analogWrite(s2_pin, s2);
//    analogWrite(s1_pin, s1);
////    Serial.println("ON");
//  }
//  else {
////    analogWrite(s2_pin, 0);
//    analogWrite(s1_pin, 0);
////    Serial.println("OFF");
//  }





/// дисплей по нажатию на кнопки в интерфейсе(и при движении всяких ползунков и тд) отправляет сообщения ардуине/ мы смотрим что за сообщение и делаем соотв действие ..  тут просто тестовые кнопки вкл.выкл
  String message = myNextion.listen(); //check for message
  //  Serial.println(message);
  if (message != "") { // if a message is received...
//    Serial.println(message); //...print it out
  }
  if (message == "65 4 5 1 ffff ffff ffff") {
    b1_st = !b1_st;
    digitalWrite(b1, b1_st);
    //    Serial.println(b1_st);
  }


// тут ловим сообщение при тыкании в кнопку старт и получив сообщение проверяем состояние кнопки через методы библиотеки дисплея
  if (message == "65 4 9 1 ffff ffff ffff") {
    strt_s = myNextion.getComponentValue("page4.bt3");
    if (strt_s == 1) {
      start_st = true;
      motor_on = true;
      long freq = myNextion.getComponentValue("page4.n0");
      int high_speed = myNextion.getComponentValue("page4.n2");
      int low_speed = myNextion.getComponentValue("page4.n3");
      s2=myNextion.getComponentValue("page4.n1");
      analogWrite(s2_pin, s2);
      i = 50;

      n = 100;
      String com = "com:";
      com = com+high_speed+':'+low_speed+':'+freq;
      
//      Serial.print("freq - ");
//      Serial.println(freq);
//
//      Serial.print("high_speed - ");
//      Serial.println(high_speed);
//
//      Serial.print("low_speed - ");
//      Serial.println(low_speed);

      Serial.println(com);

      
//      Serial.print("ON\n");
    }
    else {
      analogWrite(s2_pin, 0);
      start_st = false;
      Serial.print("OFF\n");
    }

//    Serial.println(start_st);
  }

//тут получаем значения со слайдеров
  if (message == "65 3 7 1 ffff ffff ffff" || message == "65 3 4 1 ffff ffff ffff") {
    pressure = myNextion.getComponentValue("page3.n0");
//    Serial.print("pressure = ");
//    Serial.println(pressure);
  }


  if (message == "65 3 8 1 ffff ffff ffff" || message == "65 3 5 1 ffff ffff ffff") {
    freq = myNextion.getComponentValue("page3.n1");
//    Serial.print("com:");
//    Serial.println(freq);
//    Serial.print("frequency = ");
//    Serial.println(freq);
  }


  if (message == "65 3 6 1 ffff ffff ffff" || message == "65 3 9 1 ffff ffff ffff") {
    Time = myNextion.getComponentValue("page3.n2");
//    Serial.print("time = ");
//    Serial.println(Time);
  }



  if (millis() - current_time > period) {
    slid1 = myNextion.getComponentValue("page4.n0");
    current_time = millis();
    if (s1 != slid1) {
      s1 = slid1;
      analogWrite(s1_pin, s1);
      //      analogWrite(s1_pin, s1);
//      Serial.print("s1 = ");
//      Serial.println(s1);

    }



    slid2 = myNextion.getComponentValue("page4.n1");
    if (s2 != slid2) {
      s2 = slid2;
      //      analogWrite(s2_pin, s2);
//      Serial.print("s2 = ");
//      Serial.println(s2);
    }
  }


  //65 4 5 1 ffff ffff ffff
  //65 4 5 0 ffff ffff ffff

  //65 4 3 1 ffff ffff ffff
  //65 4 3 0 ffff ffff ffff




  // int slid1 = myNextion.getComponentValue("page4.h0");
}


// это фция для того чтобы крутить шаговым двигателем по таймеру( в сетапе таймер закоменчен)
void timer_handle_interrupts(int timer) {
  if (motor_on) {
    n -- ;
    if (n < 0) {
      n = 8100;
      if (i > 9) i--;
    }
    PORTB = B0000;
    delayMicroseconds(i);
    PORTB = B1000;
    delayMicroseconds(i);
  }
}
