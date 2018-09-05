


//#include"timer-api.h"


#include <SoftwareSerial.h>
#include <Nextion.h>


#define RF1ON 266325
#define RF1OFF 266321
#define PERIOD 146;
int s1 = 0;
int s2 = 0;



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

  digitalWrite(dir, LOW);
  digitalWrite(s2_pin, LOW);



}



bool start_engine = false;
int power = 0;
long current_power = 0;
int power_period = 200;

void loop() {

  if (start_engine && millis() - current_power > power_period) {
  current_power=millis();
    analogWrite(s2_pin, power);
    power++;
    if (power >= s2) {
        start_engine = false;
        power = 0;
    }
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
            s2 = myNextion.getComponentValue("page4.n1");
            //      analogWrite(s2_pin, s2);
            start_engine = true;
            i = 50;

            n = 100;
            String com = "com:";
            com = com + high_speed + ':' + low_speed + ':' + freq;

            Serial.println(com);


          }
          else {
            analogWrite(s2_pin, 0);
             start_engine = false;
        power = 0;
            start_st = false;
            Serial.print("OFF\n");
          }

        }
      }







    }




  



