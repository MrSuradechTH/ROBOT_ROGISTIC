#include <QMC5883LCompass.h>
QMC5883LCompass compass;

#include <Servo.h>
Servo ser_l,ser_r;

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

uint8_t motor[3][3] = {{44, 45, 8},{46, 47, 9},{48, 49, 10}}; // มอรเตอร์ซ้าย,มอเตอร์ขวา,เซอร์โว
int motor_speed_default[] = {1200,1200,1200,1200}; //แก้ตรงนี้ให้เป็นค่าที่หุ่นเดินตรง
uint8_t speed_down[] = {500,500};
uint8_t line_sensor[] = {22,23,24,25,26,27}; //กลาง 4 ตัวแรก 2 ตัวหลังอ่านข้าง ไล่จากซ้ายไปขวา
String line_status;
int stack;
int lm[] = {32,34,35};

int degree,degree_set;
byte arm_level_now = 3;

void robot_motor(uint8_t digital_a,uint8_t digital_b,uint8_t digital_c,uint8_t digital_d,int analog_a,int analog_b,uint16_t int_a) {
  digitalWrite(motor[0][0], digital_a);
  digitalWrite(motor[0][1], digital_b);
  digitalWrite(motor[1][0], digital_c);
  digitalWrite(motor[1][1], digital_d);
  pwm.setPWM(motor[0][2], 0, analog_a);
  pwm.setPWM(motor[1][2], 0, analog_b);
  if (int_a != 0) {
    delay(int_a);
  }
}

void robot_forward() {
  robot_motor(1,0,1,0,motor_speed_default[0],motor_speed_default[1],0);
}

void robot_stop() {
  robot_motor(0,0,0,0,0,0,0);
}

void robot_left(int degree_value) {
//  set_degree();
//  degree_set -= degree_value;
//  if  (degree_set < 0) {
//    degree_set = 360 - abs(degree_set);
//  }
  robot_motor(0,1,1,0,motor_speed_default[2],motor_speed_default[3],0);
//  while(!degree_stack_check()) {
//    get_degree();
//    Serial.println(String(degree) + " " + String(degree_set));
//  }

  while(digitalRead(line_sensor[0]) == LOW) {
  
  }
  while(digitalRead(line_sensor[2]) == LOW) {
    
  }
  get_out_line();
  robot_stop();
}

void robot_right(int degree_value) {
//  set_degree();
//  degree_set += degree_value;
//  if  (degree_set > 360) {
//    degree_set = degree_set - 360;
//  }
  robot_motor(1,0,0,1,motor_speed_default[2],motor_speed_default[3],0);
//  while(!degree_stack_check()) {
//    get_degree();
//    Serial.println(String(degree) + " " + String(degree_set));
//  }

  while(digitalRead(line_sensor[3]) == LOW) {
  
  }
  while(digitalRead(line_sensor[1]) == LOW) {
    
  }

  get_out_line();
  robot_stop();
}

void line_check() {
  line_status = "";
  for(uint8_t c = 0;c < 2;c++) {
    line_status += String(digitalRead(line_sensor[c + 1]));
  }

//String  line_statuss = "";
//  line_status += String(digitalRead(line_sensor[4]));
//  for(uint8_t c = 0;c < 4;c++) {
//    line_statuss += String(digitalRead(line_sensor[c]));
//  }
//  line_statuss += String(digitalRead(line_sensor[5]));
//  Serial.println(line_statuss);
}

void balance() {
  line_check();
  if (digitalRead(line_sensor[0]) == 1 && digitalRead(line_sensor[3]) == 0 && line_status == "00") {
    while(digitalRead(line_sensor[1]) == 0) {
      robot_motor(1,0,1,0,0,motor_speed_default[1] + speed_down[0],0);
    }
  }else if (digitalRead(line_sensor[0]) == 0 && digitalRead(line_sensor[3]) == 1 && line_status == "00") {
    while(digitalRead(line_sensor[1]) == 0) {
      robot_motor(1,0,1,0,motor_speed_default[0] + speed_down[0],0,0);
    }
  }else if (line_status == "00" || line_status == "11") {
      robot_forward();
  }else if (line_status == "10") {
      robot_motor(1,0,1,0,motor_speed_default[0] - speed_down[0],motor_speed_default[1] + speed_down[0],0);
  }else if (line_status == "01") {
      robot_motor(1,0,1,0,motor_speed_default[0] + speed_down[0],motor_speed_default[0] - speed_down[0],0);
  }
  
}

void get_degree() {
  compass.read();
  degree = compass.getAzimuth();
//  Serial.println(degree);
}

void set_degree() {
  get_degree();
  degree_set = degree;
}

bool degree_stack_check() {
  bool st = false;
    if (degree == degree_set - 1 || degree == degree_set || degree == degree_set + 1) {
//       && digitalRead(29) == 1
      st = true;
    }
  return st;
}

void get_stack(int stack_count) {
  while(stack != stack_count) {
    balance();
    if (digitalRead(line_sensor[4]) == 1 || digitalRead(line_sensor[5]) == 1) {
      stack++;
      Serial.println(stack);
      get_out_line();
    }
  }
  reset_stack();
}

void reset_stack() {
  stack = 0;
}

void get_out_line() {
  while(digitalRead(line_sensor[4]) == 1 || digitalRead(line_sensor[5]) == 1) {
    if(digitalRead(line_sensor[4]) == 1 && digitalRead(line_sensor[5]) == 1) {
      balance();
    }else if (digitalRead(line_sensor[4]) == 1 && digitalRead(line_sensor[5]) == 0) {
      robot_motor(1,0,0,1,motor_speed_default[2],motor_speed_default[3],0);
    }else if (digitalRead(line_sensor[4]) == 0 && digitalRead(line_sensor[5]) == 1) {
      robot_motor(0,1,1,0,motor_speed_default[2],motor_speed_default[3],0);
    }  
  }
}

void open_arm() {
  ser_l.write(40);
  ser_r.write(140);
}

void close_arm() {
  ser_l.write(90);
  ser_r.write(90);
}

void arm_set(byte level) {
  if (level != arm_level_now) {
    if (level > arm_level_now) {
      while(digitalRead(lm[level - 1]) == HIGH) {
        digitalWrite(motor[2][0], 0);
        digitalWrite(motor[2][1], 1);
        pwm.setPWM(motor[2][2], 0, 4095);
      }
      digitalWrite(motor[2][0], 0);
      digitalWrite(motor[2][1], 0);
      pwm.setPWM(motor[2][2], 0, 0);
      arm_level_now = level;
    }else if (level < arm_level_now) {
      while(digitalRead(lm[level - 1]) == HIGH) {
        digitalWrite(motor[2][0], 1);
        digitalWrite(motor[2][1], 0);
        pwm.setPWM(motor[2][2], 0, 4095);
      }
      digitalWrite(motor[2][0], 0);
      digitalWrite(motor[2][1], 0);
      pwm.setPWM(motor[2][2], 0, 0);
      arm_level_now = level;
    }
  }
}

void setup() {
  Serial.begin(9600);
  compass.init();
  compass.setCalibration(-1702, 512, -1911, 281, -2048, 0);
  for(uint8_t a = 0;a < 3;a++) {
    for(uint8_t b = 0;b < 2;b++) {
      pinMode(motor[a][b], OUTPUT);
    }
  }
  
  for(uint8_t c = 0;c < 6;c++) {
    pinMode(line_sensor[c], INPUT);
  }

  for(uint8_t d = 0;d < 3;d++) {
    pinMode(lm[d], INPUT);
  }

  ser_l.attach(10);
  ser_r.attach(11);
  get_degree();
  
  arm_set(1);
  open_arm();

  pwm.begin();
  //pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(1000);
  Wire.setClock(400000);
  
//  get_stack(1);
//  robot_left(90);
}

void loop() {
//  Serial.println(digitalRead(lm));

//  robot_forward();
//  delay(5000);
//  robot_motor(0,1,1,0,motor_speed_default[2],motor_speed_default[3],0);
//  delay(5000);
//  robot_motor(1,0,0,1,motor_speed_default[2],motor_speed_default[3],0);
//  delay(5000);

//  arm_set(1);
//  close_arm();
//  delay(1000);
//  arm_set(2);
//  delay(1000);
//  get_stack(1);
//  robot_left(90);
//  get_stack(1);
//  robot_left(90);
//  get_stack(1);
//  close_arm();
//  get_stack(1);
//  robot_right(90);
//  while(true) {
//    robot_stop();
//  }

//line_check();
//balance();
      
arm_set(1);
arm_set(3);
Serial.println(String(digitalRead(lm[0])) + " : " + String(digitalRead(lm[1])) + " : " + String(digitalRead(lm[2])));
//get_stack(1);
//while(true) {
//    robot_stop();
//  }
}
