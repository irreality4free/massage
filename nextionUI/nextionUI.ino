


//#include"timer-api.h"


#include <SoftwareSerial.h>
#include <Nextion.h>


#define RF1ON 266325
#define RF1OFF 266321
#define PERIOD 146;
int s1 = 0;
int s2 = 0;

int volt = 5;
int volt_gnd = 6;



int strt_s;
int s2_pin = 9;


bool start_st = true;

int period = 150;

long current_time = 0;


int slid2 = 0;

long i = 50;
int dir = 12;

long n = 800;

bool motor_on = true;

int start_del = 500;
long current_del = 0;
int last_state = 0;
int sensor = A4;



//подключаем дисплей через софтварный сериальник

SoftwareSerial nextion(2, 3);// Nextion TX to pin 2 and RX to pin 3 of Arduino

Nextion myNextion(nextion, 9600); //create a Nextion object named myNextion using the nextion serial port @ 9600bps
void setup() {
  Serial.begin(9600);


  TCCR1A = TCCR1A & 0xe0 | 1;
  TCCR1B = TCCR1B & 0xe0 | 0x09;
  myNextion.init();
  pinMode(dir, OUTPUT);
  pinMode(s2_pin, OUTPUT);
  pinMode(volt, OUTPUT);
  pinMode(volt_gnd, OUTPUT);

  digitalWrite(dir, LOW);
  digitalWrite(s2_pin, LOW);

  analogWrite(volt, 0);
  digitalWrite(volt_gnd, LOW);



}



bool start_engine = false;
int power = 0;
int pulse_power = 0;
long current_power = 0;
int power_period = 200;
int pulse_speed = 0;
float pressure= 0;

long cur_pres=0;
int pres_per =1000;
int num_tr = 1000;


void loop() {
  pressure=0;
  for(int i = 0; i<num_tr;i++){
  pressure += (50000/1024) * analogRead(sensor) ;
  }
  pressure = pressure/num_tr;

  if (millis()-cur_pres>pres_per){
  cur_pres =millis();
  Serial.print("pressure = ");
  Serial.print(pressure/1000);
  Serial.println(" kPa");
  }
  //myNextion.setComponentText("t0", String(pressure));
  

  if (start_engine && millis() - current_power > power_period) {
    current_power = millis();
    analogWrite(s2_pin, power);
    analogWrite(volt, pulse_power);
    Serial.println("ENGINE FUNC");
    Serial.println(pulse_power);
    if (power < s2)power++;
    if (power > s2)power--;
    if (pulse_power < pulse_speed)pulse_power++;
    if (pulse_power > pulse_speed)pulse_power--;
    if (power == 0 && pulse_power == 0) {
      start_engine = false;
      analogWrite(s2_pin, 0);
    analogWrite(volt, 0);
    Serial.println("ENGINE OFF");
    
      //        power = s2;
    }
    //
    //    if (pulse_power == pulse_speed) {
    ////        start_engine = false;
    ////        pulse_power = pulse_speed;
    //    }
  }



  String message = myNextion.listen(); //check for message

  if (message == "65 4 9 1 ffff ffff ffff" || millis() - current_del > start_del) {

    current_del = millis();
    strt_s = myNextion.getComponentValue("page4.bt3");

    if (last_state != strt_s) {
      last_state = strt_s;
      if (strt_s == 1) {
        start_st = true;
        motor_on = true;
        long freq = myNextion.getComponentValue("page4.n0");
        int high_speed = myNextion.getComponentValue("page4.n2");
        int low_speed = myNextion.getComponentValue("page4.n3");
        pulse_speed = myNextion.getComponentValue("page4.n2");
        Serial.println(pulse_speed);
        s2 = myNextion.getComponentValue("page4.n1");
        //      analogWrite(s2_pin, s2);
        start_engine = true;
        i = 50;
        //            digitalWrite(volt, 150);

        n = 100;
        String com = "com:";
        com = com + high_speed + ':' + low_speed + ':' + freq;

        Serial.println(com);


      }
      else {
        s2 = 0;
        pulse_speed = 0;
        //            analogWrite(s2_pin, 0);
        //             start_engine = false;
        //        power = 0;
        //            start_st = false;
        Serial.print("OFF\n");
      }

    }
  }







}
