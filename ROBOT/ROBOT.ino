#include <QMC5883LCompass.h>

QMC5883LCompass compass;

uint8_t motor[2][3] = {{36, 37, 8},{41, 40, 7}}; // มอรเตอร์ซ้าย,มอเตอร์ขวา ถ้าไม่ตรงให้แก้
uint8_t motor_speed_default[] = {50,50,60,60}; //แก้ตรงนี้ให้เป็นค่าที่หุ่นเดินตรง **********
uint8_t speed_down[] = {15,7};
uint8_t line_sensor[] = {27,28,29,30,31,32}; //กลาง 4 ตัวแรก 2 ตัวหลังอ่านข้าง แยกกัน
String line_status; //line_status_old ยังไม่มีการใช้งาน
int stack;

//String all_compass[] = {"n","e","s","w"}; 
int compass_now = 0;
int degree,degree_set;

void robot_motor(uint8_t digital_a,uint8_t digital_b,uint8_t digital_c,uint8_t digital_d,uint8_t analog_a,uint8_t analog_b,uint16_t int_a) {
  digitalWrite(motor[0][0], digital_a);
  digitalWrite(motor[0][1], digital_b);
  digitalWrite(motor[1][0], digital_c);
  digitalWrite(motor[1][1], digital_d);
  analogWrite(motor[0][2], analog_a);
  analogWrite(motor[1][2], analog_b);
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
  set_degree();
  degree_set -= degree_value;
  if  (degree_set < 0) {
    degree_set = 360 - abs(degree_set);
  }
  robot_motor(0,1,1,0,motor_speed_default[2],motor_speed_default[3],0);
  while(!degree_stack_check()) {
    get_degree();
    Serial.println(String(degree) + " " + String(degree_set));
  }
  get_out_line();
  robot_stop();
}

void robot_right(int degree_value) {
  set_degree();
  degree_set += degree_value;
  if  (degree_set > 360) {
    degree_set = degree_set - 360;
  }
  robot_motor(1,0,0,1,motor_speed_default[2],motor_speed_default[3],0);
  while(!degree_stack_check()) {
    get_degree();
    Serial.println(String(degree) + " " + String(degree_set));
  }
  get_out_line();
  robot_stop();
}

void line_check() {
  line_status = "";
  for(uint8_t c = 0;c < 2;c++) {
    line_status += String(digitalRead(line_sensor[c + 1]));
  }
//  Serial.println(line_status);
}

void balance() {
  line_check();
  if (digitalRead(27) == 1 && digitalRead(30) == 0 && line_status == "00") {
    while(digitalRead(28) == 0) {
      robot_motor(1,0,1,0,0,motor_speed_default[1] + speed_down[0],0);
    }
  }else if (digitalRead(27) == 0 && digitalRead(30) == 1 && line_status == "00") {
    while(digitalRead(28) == 0) {
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
    if (digitalRead(31) == 1 || digitalRead(32) == 1) {
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
  while(digitalRead(31) == 1 || digitalRead(32) == 1) {
    balance();
  }
}

void setup() {
  Serial.begin(9600);
  compass.init();
  compass.setCalibration(-1702, 512, -1911, 281, -2048, 0);
  for(uint8_t a = 0;a < 2;a++) {
    for(uint8_t b = 0;b < 3;b++) {
      pinMode(motor[a][b], OUTPUT);
    }
  }
  
  for(uint8_t c = 0;c < 6;c++) {
    pinMode(line_sensor[c], INPUT);
  }
  get_degree();
}

void loop() {
//  get_stack(1);
//  robot_left(90);
//  delay(5000);
//  get_stack(1);
//  robot_left(180);
//  delay(5000);
//  get_stack(1);
//  robot_right(90);
//  delay(5000);
//  robot_forward();
//  delay(1000);
//  robot_right(180);
//  delay(5000);
  
//  get_stack(3);
//  robot_left(180);
//  get_stack(3);
//  robot_left(180);
//  while(true) {
//    robot_stop();
//  }
}
