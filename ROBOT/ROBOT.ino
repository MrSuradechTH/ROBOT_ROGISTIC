#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

//motor
uint8_t motor[3][3] = {{44, 45, 8},{46, 47, 9},{48, 49, 10}}; // มอรเตอร์ซ้าย,มอเตอร์ขวา,เซอร์โว
uint16_t motor_speed_default[] = {1400,1450,1400,1450}; //แก้ตรงนี้ให้เป็นค่าที่หุ่นเดินตรง
uint8_t speed_down[] = {600,600,600,600};

//line
uint8_t line_sensor[] = {22,23,24,25,26,27}; //กลาง 4 ตัวแรก 2 ตัวหลังอ่านข้าง ไล่จากซ้ายไปขวา
String line_status;
uint8_t stack;

//arm
uint8_t lm[] = {32,34,35};
uint8_t arm_level_now = 3;
uint8_t state_lm[] = {0,0,0};

//box
uint8_t ir = 33;
uint8_t box_success,shelf_success;
bool box_check = false;
bool box_state[] = {true,true,true,true,true,true,true,true};
bool shelf_state[] = {true,true,true,true,true,true,true,true};
uint8_t shelf_stack,shelf_now,shelf_floor;
//bool shelf_color[] = {"red","green","red","green","red","green","red","green"}; //รอเข้าชั้นให้ได้ สีเรียงจากบนลงล่าง ซ้ายไปขวา

//map
uint8_t x = 8,y = 4;
uint8_t x_now,y_now,x_go,y_go;
uint8_t y_put_box = 5,x_min = 0,x_max = 4,x_mid = 2;


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

void robot_backward() {
  robot_motor(0,1,0,1,motor_speed_default[0],motor_speed_default[1],0);
}

void robot_stop() {
  robot_motor(0,0,0,0,0,0,0);
}

void robot_left() {
  robot_motor(0,1,1,0,motor_speed_default[2],motor_speed_default[3] + speed_down[2],0);

  while(digitalRead(line_sensor[0]) == 0) {
  
  }
  
  while(digitalRead(line_sensor[2]) == 0) {
    
  }
  
  get_out_line();
  robot_stop();
}

void robot_right() {
  robot_motor(1,0,0,1,motor_speed_default[2] + speed_down[3],motor_speed_default[3],0);

  while(digitalRead(line_sensor[3]) == 0) {
  
  }
  
  while(digitalRead(line_sensor[1]) == 0) {
    
  }

  get_out_line();
  robot_stop();
}

void turn_around() {
  robot_motor(0,0,0,1,motor_speed_default[2],motor_speed_default[3],0);
  while(digitalRead(line_sensor[5]) == 0) {
    
  }

  robot_motor(1,0,0,0,motor_speed_default[2],motor_speed_default[3],0);
  while(digitalRead(line_sensor[4]) == 0) {
    
  }
  
  robot_motor(1,0,0,1,motor_speed_default[2],motor_speed_default[3],0);
  while(digitalRead(line_sensor[1]) == 0) {
    
  }
  robot_stop();
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
  if ((digitalRead(line_sensor[0]) == 1 && line_status == "11" && digitalRead(line_sensor[3]) == 1) || (digitalRead(line_sensor[0]) == 0 && line_status == "00" && digitalRead(line_sensor[3]) == 0)) {
    robot_forward();
  }else if (digitalRead(line_sensor[0]) == 1 && line_status == "00" && digitalRead(line_sensor[3]) == 0) {
    while(digitalRead(line_sensor[2]) == 0) {
      robot_motor(1,0,1,0,0,motor_speed_default[1] + speed_down[0],0);
      if ((digitalRead(line_sensor[4]) == 1 || digitalRead(line_sensor[5]) == 1) || (digitalRead(line_sensor[0]) == 0 && line_status == "00" && digitalRead(line_sensor[3]) == 0)) {
        robot_stop();
        break;
      }
    }
  }else if (digitalRead(line_sensor[0]) == 0 && line_status == "00" && digitalRead(line_sensor[3]) == 1) {
    while(digitalRead(line_sensor[1]) == 0) {
      robot_motor(1,0,1,0,motor_speed_default[0] + speed_down[0],0,0);
      if ((digitalRead(line_sensor[4]) == 1 || digitalRead(line_sensor[5]) == 1) || (digitalRead(line_sensor[0]) == 0 && line_status == "00" && digitalRead(line_sensor[3]) == 0)) {
        robot_stop();
        break;
      }
    }
  }else if (digitalRead(line_sensor[0]) == 1 && line_status == "11" && digitalRead(line_sensor[3]) == 0) {
//      robot_motor(1,0,1,0,0,motor_speed_default[1] + speed_down[0],0);
  robot_forward();
  }else if (digitalRead(line_sensor[0]) == 1 && line_status == "10" && digitalRead(line_sensor[3]) == 0) {
//      robot_motor(1,0,1,0,0,motor_speed_default[1] + speed_down[0],0);
  robot_forward();
  }else if (digitalRead(line_sensor[0]) == 0 && line_status == "11" && digitalRead(line_sensor[3]) == 1) {
//      robot_motor(1,0,1,0,motor_speed_default[0] + speed_down[0],0,0);
  robot_forward();
  }else if (digitalRead(line_sensor[0]) == 0 && line_status == "01" && digitalRead(line_sensor[3]) == 1) {
//      robot_motor(1,0,1,0,motor_speed_default[0] + speed_down[0],0,0);
  robot_forward();
  }else if (line_status == "00" || line_status == "11") {
      robot_forward();
  }else if (line_status == "10") {
      robot_motor(1,0,1,0,0,motor_speed_default[1] - speed_down[1],0);
  }else if (line_status == "01") {
      robot_motor(1,0,1,0,motor_speed_default[0] - speed_down[0],0,0);
  }else {
    robot_forward();
  }
}

//void balance_backward() {
//  line_check();
//  if (digitalRead(line_sensor[0]) == 1 && line_status == "11" && digitalRead(line_sensor[3]) == 1) {
//    robot_backward();
//  }else if (digitalRead(line_sensor[0]) == 1 && line_status == "00" && digitalRead(line_sensor[3]) == 0) {
//    while(digitalRead(line_sensor[2]) == 0) {
//      robot_motor(0,1,0,1,motor_speed_default[0] + speed_down[0],0,0);
//    }
//  }else if (digitalRead(line_sensor[0]) == 0 && line_status == "00" && digitalRead(line_sensor[3]) == 1) {
//    while(digitalRead(line_sensor[1]) == 0) {
//      robot_motor(0,1,0,1,0,motor_speed_default[1] + speed_down[0],0);
//    }
//  }else if (digitalRead(line_sensor[0]) == 1 && line_status == "11" && digitalRead(line_sensor[3]) == 0) {
////      robot_motor(1,0,1,0,0,motor_speed_default[1] + speed_down[0],0);
//  robot_backward();
//  }else if (digitalRead(line_sensor[0]) == 0 && line_status == "11" && digitalRead(line_sensor[3]) == 1) {
////      robot_motor(1,0,1,0,motor_speed_default[0] + speed_down[0],0,0);
//  robot_backward();
//  }else if (line_status == "00" || line_status == "11") {
//      robot_backward();
//  }else if (line_status == "10") {
//      robot_motor(0,1,0,1,motor_speed_default[0] + speed_down[2],motor_speed_default[1] - speed_down[3],0);
//  }else if (line_status == "01") {
//      robot_motor(0,1,0,1,motor_speed_default[0] - speed_down[2],motor_speed_default[0] + speed_down[3],0);
//  }
//}

void get_stack(uint8_t stack_count) {
  stack_count = stack_count;
  while(stack != stack_count) {
    Serial.println(stack);
    balance();
    if (box_check == true && digitalRead(line_sensor[0]) == 1 && digitalRead(line_sensor[3]) == 1 && y_now == y_put_box) {
      stack++;
    }else if (digitalRead(ir) == 0 && box_check == false) {
        stack++;
    }
    
//    else if (digitalRead(line_sensor[4]) == 1 || digitalRead(line_sensor[5]) == 1) {
//      get_out_line_forward();
//      stack++;  
//    }
  }
  stack = 0;
  robot_stop();
}

//void get_stack_backward(uint8_t stack_count) {  //only 1 stack
//  stack_count = stack_count;
//  while(stack != stack_count) {
////    Serial.println(stack);
//    robot_backward();
//    while ((digitalRead(line_sensor[0]) == 0 || digitalRead(line_sensor[1]) == 0 || digitalRead(line_sensor[2]) == 0 || digitalRead(line_sensor[3]) == 0)) {
//      
//    }
//    robot_stop();
//    delay(500);
//    
//    robot_forward();
//    while (digitalRead(line_sensor[4]) == 0 || digitalRead(line_sensor[5]) == 0) {
//      if (digitalRead(line_sensor[4]) == 1) {
//        robot_motor(0,0,1,0,motor_speed_default[2],motor_speed_default[3],0);
//      }else if (digitalRead(line_sensor[5]) == 1) {
//        robot_motor(1,0,0,0,motor_speed_default[2],motor_speed_default[3],0);
//      }
//    }
//    stack++;
//  }
//  stack = 0;
//  robot_stop();
//}

void get_stack_left(uint8_t stack_count) {
  stack_count = stack_count;
  while(stack != stack_count) {
//    Serial.println(stack);
    balance();
    
    if (digitalRead(line_sensor[4]) == 1) {
      stack++;
      get_out_line_forward();
    }
  }
  stack = 0;
  robot_stop();
}

void get_stack_right(uint8_t stack_count) {
  stack_count = stack_count;
  while(stack != stack_count) {
//    Serial.println(stack);
    balance();
    
    if (digitalRead(line_sensor[5]) == 1) {
      stack++;
      get_out_line_forward();
    }
  }
  stack = 0;
  robot_stop();
}

void get_out_line() {
  while(digitalRead(line_sensor[4]) == 1 || digitalRead(line_sensor[5]) == 1) {
    if(digitalRead(line_sensor[4]) == 1 && digitalRead(line_sensor[5]) == 1) {
      balance();
    }else if (digitalRead(line_sensor[4]) == 1 && digitalRead(line_sensor[5]) == 0) {
      robot_motor(1,0,0,0,motor_speed_default[2],0,0);
    }else if (digitalRead(line_sensor[4]) == 0 && digitalRead(line_sensor[5]) == 1) {
      robot_motor(0,0,1,0,0,motor_speed_default[3],0);
    }  
  }
}

void get_out_line_forward() {
  while(digitalRead(line_sensor[4]) == 1 || digitalRead(line_sensor[5]) == 1) {
    robot_forward();
  }
}

//void get_out_line_backward() {
//  while(digitalRead(line_sensor[4]) == 1 || digitalRead(line_sensor[5]) == 1) {
//    robot_backward();
//  }
//}

//arm
int angle(int angles)
{ 
  int pulse_wide, analog_value;
  pulse_wide = map(angles, 0, 180, 650, 2350);
  analog_value = int(float(pulse_wide) / 1000000 * 50 * 4096);
  return analog_value;
}

void open_arm() {
  pwm.setPWM(0, 0, angle(110));
  pwm.setPWM(1, 0, angle(30));
}

void open_arms() {
  pwm.setPWM(0, 0, angle(65));
  pwm.setPWM(1, 0, angle(75));
}

void close_arm() {
  pwm.setPWM(0, 0, angle(60));
  pwm.setPWM(1, 0, angle(80));
}

void arm_state() {
  for(uint8_t i = 0;i < 3;i++) {
    if (i == 1) {
      state_lm[i] = !digitalRead(lm[i]);
    }else {
      state_lm[i] = digitalRead(lm[i]);
    }
    
  }
}

void arm_set(uint8_t level) {
//  Serial.println(String(digitalRead(lm[0])) + " : " + String(digitalRead(lm[1])) + " : " + String(digitalRead(lm[2])));
  arm_state();
  if (level != arm_level_now) {
    if (level > arm_level_now) {
      while(state_lm[level - 1] == HIGH) {
        arm_state();
        Serial.println(String(state_lm[0]) + " : " + String(state_lm[1]) + " : " + String(state_lm[2]));
        digitalWrite(motor[2][0], 0);
        digitalWrite(motor[2][1], 1);
        pwm.setPWM(motor[2][2], 0, 1200);
      }
      digitalWrite(motor[2][0], 0);
      digitalWrite(motor[2][1], 0);
      pwm.setPWM(motor[2][2], 0, 0);
      arm_level_now = level;
    }else if (level < arm_level_now) {
      while(state_lm[level - 1] == HIGH) {
        arm_state();
        Serial.println(String(state_lm[0]) + " : " + String(state_lm[1]) + " : " + String(state_lm[2]));
        digitalWrite(motor[2][0], 1);
        digitalWrite(motor[2][1], 0);
        pwm.setPWM(motor[2][2], 0, 1200);
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
  robot_stop();
  arm_set(1);
  delay(500);
  close_arm();
  delay(500);
  box_check = true;
  delay(2000);
}

void put_box() {
  robot_stop();
  open_arm();
  delay(500);
  arm_set(3);
  robot_right();
  robot_right();
  box_check = false;
  delay(500);
  get_out_line();
  delay(2000);
}

//map
void goto_xy(uint8_t x_need,uint8_t y_need) {
  if (box_check == true) {
    arm_set(shelf_floor + 1);
    turn_around();
    
    if (x_now > x_mid) {
      get_stack_left(1);
      robot_left();
      x_now = x_mid;
    }else if (x_now < x_mid) {
      get_stack_left(1);
      robot_right();
      x_now = x_mid;
    }

    if (y_now < y_put_box) {
      get_stack_left(y_need - y_now);
      y_now = y_put_box;
    }

    if (x_need > x_mid) {
      robot_left();
      get_stack_right(shelf_stack);
      robot_right();
      x_now = x_need;
    }else {
      robot_right();
      get_stack_left(shelf_stack);
      robot_left();
      x_now = x_need;
    }
    
    get_stack_left(1);
    arm_set(shelf_floor);
    put_box();
  }else {
    robot_stop();
    if (x_now > x_mid) {     
      get_stack_left(1);
      robot_left();
      if (x_now - x_mid == 2) {
        get_stack_left(1);
        get_stack_right(1);
      }else {
        get_stack_right(1);
      }
      robot_right();
      x_now = x_mid;
    }else {
      get_stack_left(1);
      robot_right();
      if (x_mid - x_now == 2) {
        get_stack_right(1);
        get_stack_left(1);
      }else {
        get_stack_left(1);
      }
      robot_left();
      x_now = x_mid;
    }

    get_stack_left(y_now - y_need);
    y_now = y_need;

    if (x_need == x_max) {
      robot_right();
      x_now = x_max;
    }else if (x_need == x_min) {
      robot_left();
      x_now = x_min;
    }

    arm_set(1);
    get_stack(1);
    get_box();
  }
}

void goto_box() {
  box_success = 0;
  for (uint8_t g;g < 8;g++) {
    if (box_state[g] == true) {
      box_state[g] = false;
      if (g%2 == 0) {
        x_go = x_max;
      }else {
        x_go = x_min;
      }
      y_go = int(g/2) + 1;
      break;
    }else {
      box_success++;
    }
  }

  if (box_success == 8) {
    goto_shelf();
  }else {
    goto_xy(x_go,y_go);
  }
}

void goto_shelf() {
  shelf_success = 0;
  for (uint8_t h;h < 8;h++) {
    if (shelf_state[h] == true) {
      shelf_state[h] = false;
      if (h > 4) {
        shelf_floor = 2;
      }else {
        shelf_floor = 1;
      }
      
      if (h%4 > 1) {
        shelf_stack = 3 - (4 - (h%4));
        x_go = 3 - (h%4);
      }else {
        shelf_stack = 3 - ((h%4) + 1);
        x_go = 4 - (h%4);
      }
      y_go = y_put_box;
      
      break;
    }else {
      shelf_success++;
    }
  }

  //get gold box and put to end fuction
  if (shelf_success == 8) {
    
  }else {
    goto_xy(x_go,y_go);
  }
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

  pwm.begin();
  //pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
  Wire.setClock(400000);

while (digitalRead(ir) == 1) {
  Serial.println(digitalRead(ir));
}

  //get_box_1
  
  arm_set(1);
  open_arm();
  get_stack_left(1);
  robot_left();
  get_stack(1);
  get_box();
  x_now = 4,y_now = 1;
  box_state[0] = false;
}

void loop() {
  while (box_success != 8 && shelf_success != 8) {
    goto_shelf();
    goto_box();
  }
  while(true) {
    robot_stop();
  }
}
