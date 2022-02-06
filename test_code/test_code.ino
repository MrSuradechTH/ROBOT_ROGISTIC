#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

//motor
uint8_t motor[3][3] = {{44, 45, 8},{46, 47, 9},{48, 49, 10}}; // มอรเตอร์ซ้าย,มอเตอร์ขวา,เซอร์โว
uint16_t motor_speed_default[] = {300,350,450,500}; //แก้ตรงนี้ให้เป็นค่าที่หุ่นเดินตรง
uint8_t speed_down[] = {100,100};

//line
uint8_t line_sensor[] = {22,23,24,25,26,27}; //กลาง 4 ตัวแรก 2 ตัวหลังอ่านข้าง ไล่จากซ้ายไปขวา
String line_status;
uint8_t stack;

//arm
uint8_t lm[] = {32,34,35};
uint8_t arm_level_now = 3;

//box
uint8_t ir = 33;
uint8_t picked;
bool box_check = false;
bool box_state[] = {true,true,true,true,true,true,true,true};
bool shelf_state[] = {true,true,true,true,true,true,true,true};
//bool shelf_color[] = {"red","green","red","green","red","green","red","green"}; //รอเข้าชั้นให้ได้ สีเรียงจากบนลงล่าง ซ้ายไปขวา

//map
uint8_t x = 9,y = 5;
uint8_t x_now,y_now,x_go,y_go;
uint8_t y_get_box,y_get_box,x_put_box,y_put_box;


//motor
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

void robot_left() {
  robot_motor(0,1,1,0,motor_speed_default[2],motor_speed_default[3],0);

  while(digitalRead(line_sensor[0]) == LOW) {
  
  }
  
  while(digitalRead(line_sensor[2]) == LOW) {
    
  }
  
  get_out_line();
  robot_stop();
}

void robot_right() {
  robot_motor(1,0,0,1,motor_speed_default[2],motor_speed_default[3],0);

  while(digitalRead(line_sensor[3]) == LOW) {
  
  }
  
  while(digitalRead(line_sensor[1]) == LOW) {
    
  }

  get_out_line();
  robot_stop();
}

void turn_around() {
  while(digitalRead(line_sensor[5]) == LOW) {
    robot_motor(0,0,0,1,motor_speed_default[2],motor_speed_default[3],0);
  }
  
  while(digitalRead(line_sensor[4]) == LOW) {
    robot_motor(1,0,0,0,motor_speed_default[2],motor_speed_default[3],0);
  }
  
  robot_motor(1,0,0,1,motor_speed_default[2],motor_speed_default[3],0);
  while(digitalRead(line_sensor[1]) == LOW) {

  }
}

//line
void line_check() {
  line_status = "";
  for(uint8_t f = 0;f < 2;f++) {
    line_status += String(digitalRead(line_sensor[f + 1]));
  }

//  String  line_statuss = "";
//  line_status += String(digitalRead(line_sensor[4]));
//  for(uint8_t c = 0;c < 4;c++) {
//    line_statuss += String(digitalRead(line_sensor[c]));
//  }
//  line_statuss += String(digitalRead(line_sensor[5]));
//  Serial.println(line_statuss);
}

void balance() {
  line_check();
  if (digitalRead(line_sensor[0]) == 1 && line_status == "11" && digitalRead(line_sensor[3]) == 1) {
    robot_forward();
  }else if (digitalRead(line_sensor[0]) == 1 && line_status == "00" && digitalRead(line_sensor[3]) == 0) {
    while(digitalRead(line_sensor[2]) == 0) {
      robot_motor(1,0,1,0,0,motor_speed_default[1] + speed_down[0],0);
    }
  }else if (digitalRead(line_sensor[0]) == 0 && line_status == "00" && digitalRead(line_sensor[3]) == 1) {
    while(digitalRead(line_sensor[1]) == 0) {
      robot_motor(1,0,1,0,motor_speed_default[0] + speed_down[0],0,0);
    }
  }else if (digitalRead(line_sensor[0]) == 1 && line_status == "11" && digitalRead(line_sensor[3]) == 0) {
//      robot_motor(1,0,1,0,0,motor_speed_default[1] + speed_down[0],0);
  robot_forward();
  }else if (digitalRead(line_sensor[0]) == 0 && line_status == "11" && digitalRead(line_sensor[3]) == 1) {
//      robot_motor(1,0,1,0,motor_speed_default[0] + speed_down[0],0,0);
  robot_forward();
  }else if (line_status == "00" || line_status == "11") {
      robot_forward();
  }else if (line_status == "10") {
      robot_motor(1,0,1,0,motor_speed_default[0] - speed_down[0],motor_speed_default[1] + speed_down[0],0);
  }else if (line_status == "01") {
      robot_motor(1,0,1,0,motor_speed_default[0] + speed_down[0],motor_speed_default[0] - speed_down[0],0);
  }
}

void get_stack(uint8_t stack_count) {
  while(stack != stack_count) {
//    Serial.println(stack);
    balance();
    if (digitalRead(line_sensor[4]) == 1 || digitalRead(line_sensor[5]) == 1) {
      stack++;
      Serial.println(stack);
      get_out_line_forward();
    }else if (digitalRead(ir) == 0 && box_check == false) {
        stack++;
    }
  }
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

void get_out_line_forward() {
  while(digitalRead(line_sensor[4]) == 1 || digitalRead(line_sensor[5]) == 1) {
    balance();
  }
}

//arm
uint8_t angle(uint8_t angles)
{ 
  int pulse_wide, analog_value;
  pulse_wide = map(angles, 0, 180, 650, 2350);
  analog_value = int(float(pulse_wide) / 1000000 * 50 * 4096);
  return analog_value;
}

void open_arm() {
  pwm.setPWM(0, 0, angle(70));
  pwm.setPWM(1, 0, angle(90));
}

void close_arm() {
  pwm.setPWM(0, 0, angle(130));
  pwm.setPWM(1, 0, angle(30));
}

void arm_set(uint8_t level) {
  robot_stop();
//  Serial.println(String(digitalRead(lm[0])) + " : " + String(digitalRead(lm[1])) + " : " + String(digitalRead(lm[2])));
  if (level != arm_level_now) {
    if (level > arm_level_now) {
      while(digitalRead(lm[level - 1]) == HIGH) {
        digitalWrite(motor[2][0], 0);
        digitalWrite(motor[2][1], 1);
        pwm.setPWM(motor[2][2], 0, 800);
      }
      digitalWrite(motor[2][0], 0);
      digitalWrite(motor[2][1], 0);
      pwm.setPWM(motor[2][2], 0, 0);
      arm_level_now = level;
    }else if (level < arm_level_now) {
      while(digitalRead(lm[level - 1]) == HIGH) {
        digitalWrite(motor[2][0], 1);
        digitalWrite(motor[2][1], 0);
        pwm.setPWM(motor[2][2], 0, 400);
      }
      digitalWrite(motor[2][0], 0);
      digitalWrite(motor[2][1], 0);
      pwm.setPWM(motor[2][2], 0, 0);
      arm_level_now = level;
    }
  }
}

//box
void get_box() {
  arm_set(1);
  delay(500);
  close_arm();
  delay(500);
  arm_set(3);
  box_check = true;
  turn_around(); 
}

//void put_box(uint8_t level_need) {
//  box_check = false;
//}

//map
void goto_xy(uint8_t x,uint8_t y) {
  
}

void goto_box() {
  picked = 0;
  for (uint8_t g;g < 8;g++) {
    if (box_state[] == true) {
      x_go = 1 + g;
      y_go = 0;
    }else {
      picked++;
    }
  }

  //end fuction
  if (picked == 8) {
    x_go = 8;
    y_go = 4;
  }

  goto_xy(x_go,y_go);
}

void setup() {
  Serial.begin(9600);
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

  pinMode(ir, INPUT);

//  get_degree();  

  pwm.begin();
  //pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
  Wire.setClock(400000);
  
//  get_stack(1);
//  robot_left(90);

//    arm_set(1);
//    open_arm();
}

void loop() {
  String  line_statuss = "";
  line_status += String(digitalRead(line_sensor[4]));
  for(uint8_t e = 0;e < 4;e++) {
    line_statuss += String(digitalRead(line_sensor[e]));
  }
  line_statuss += String(digitalRead(line_sensor[5]));
  Serial.println(line_statuss);

//get_stack(1);
//robot_left();
//get_stack(1);
//robot_left();
//get_stack(1);
//get_box();
//get_stack(4);
//while(true) {
//    robot_stop();
//}

}
