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
uint8_t box_success,shelf_success;
bool box_check = false;
bool box_state[] = {true,true,true,true,true,true,true,true};
bool shelf_state[] = {true,true,true,true,true,true,true,true};
uint8_t shelf_stack,shelf_now,shelf_floor;
//bool shelf_color[] = {"red","green","red","green","red","green","red","green"}; //รอเข้าชั้นให้ได้ สีเรียงจากบนลงล่าง ซ้ายไปขวา

//map
uint8_t x = 8,y = 4;
uint8_t x_now,y_now,x_go,y_go;
uint8_t x_get_box,y_get_box = 1,x_put_box,y_put_box = 4;


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

void balance_backward() {
  line_check();
  if (digitalRead(line_sensor[0]) == 1 && line_status == "11" && digitalRead(line_sensor[3]) == 1) {
    robot_backward();
  }else if (digitalRead(line_sensor[0]) == 1 && line_status == "00" && digitalRead(line_sensor[3]) == 0) {
    while(digitalRead(line_sensor[2]) == 0) {
      robot_motor(0,1,0,1,0,motor_speed_default[1] + speed_down[0],0);
    }
  }else if (digitalRead(line_sensor[0]) == 0 && line_status == "00" && digitalRead(line_sensor[3]) == 1) {
    while(digitalRead(line_sensor[1]) == 0) {
      robot_motor(0,1,0,1,motor_speed_default[0] + speed_down[0],0,0);
    }
  }else if (digitalRead(line_sensor[0]) == 1 && line_status == "11" && digitalRead(line_sensor[3]) == 0) {
//      robot_motor(1,0,1,0,0,motor_speed_default[1] + speed_down[0],0);
  robot_backward();
  }else if (digitalRead(line_sensor[0]) == 0 && line_status == "11" && digitalRead(line_sensor[3]) == 1) {
//      robot_motor(1,0,1,0,motor_speed_default[0] + speed_down[0],0,0);
  robot_backward();
  }else if (line_status == "00" || line_status == "11") {
      robot_backward();
  }else if (line_status == "10") {
      robot_motor(0,1,0,1,motor_speed_default[0] - speed_down[0],motor_speed_default[1] + speed_down[0],0);
  }else if (line_status == "01") {
      robot_motor(0,1,0,1,motor_speed_default[0] + speed_down[0],motor_speed_default[0] - speed_down[0],0);
  }
}

void get_stack(uint8_t stack_count) {
  while(stack != stack_count) {
//    Serial.println(stack);
    balance();
    if (box_check == true && digitalRead(line_sensor[0]) == 1 && digitalRead(line_sensor[3]) == 1 && y_now == y_put_box) {
      stack++;
    }
    
    if (digitalRead(line_sensor[4]) == 1 || digitalRead(line_sensor[5]) == 1) {
      stack++;
      get_out_line_forward();
    }else if (digitalRead(ir) == 0 && box_check == false) {
        stack++;
    }
  }
  stack = 0;
}

void get_stack_backward(uint8_t stack_count) {  //only 1 stack
  while(stack != stack_count) {
//    Serial.println(stack);
    balance_backward();
    if (digitalRead(line_sensor[4]) == 1 || digitalRead(line_sensor[5]) == 1) {
      stack++;
      get_out_line_forward();
    }
  }
  stack = 0;
}

void get_stack_left(uint8_t stack_count) {
  while(stack != stack_count) {
//    Serial.println(stack);
    balance();
    
    if (digitalRead(line_sensor[4]) == 1) {
      stack++;
      get_out_line_forward();
    }
  }
  stack = 0;
}

void get_stack_right(uint8_t stack_count) {
  while(stack != stack_count) {
//    Serial.println(stack);
    balance();
    
    if (digitalRead(line_sensor[5]) == 1) {
      stack++;
      get_out_line_forward();
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
  robot_stop();
  arm_set(1);
  delay(500);
  close_arm();
  delay(500);
  arm_set(3);
  box_check = true;
  turn_around(); 
}

void put_box() {
  robot_stop();
  robot_backward();
  get_stack_backward(1);
  box_check = false;
}

//map
void goto_xy(uint8_t x_need,uint8_t y_need) {
  if (box_check == true) {
    arm_set(shelf_floor);
    
    if (y_now < y_get_box) {
      get_stack(y_get_box - y_now);
      y_now = y_get_box;
    }
    
    if (x_now > x_need) {
      robot_right();
      get_stack(x_need - x_now);
      robot_left();
      x_now = x_need;
    }else if (x_now < x_need) {
      robot_left();
      get_stack(x_need - x_now);
      robot_right();
      x_now = x_need;
    }

    if (y_now < y_put_box) {
      get_stack(y_get_box - y_now);
      y_now = y_put_box;
    }

    if (x_now == 0) {
      robot_left();
      get_stack_right(shelf_stack);
      robot_right();
      get_stack(1);
      put_box();
      robot_right();
      get_stack_left(shelf_stack);
      robot_right();
    }else if (x_now == 8) {
      robot_right();
      get_stack_left(shelf_stack);
      robot_left();
      get_stack(1);
      put_box();
      robot_left();
      get_stack_right(shelf_stack);
      robot_left();
    }
  }else {
    if (y_now > y_get_box) {
      get_stack(y_now - y_get_box);
      y_now = y_get_box;
    }

    if (x_now > x_need) {
      robot_left();
      get_stack(x_now - x_need);
      robot_right();
      x_now = x_need;
    }else if (x_now < x_need) {
      robot_right();
      get_stack(x_need - x_now);
      robot_left();
      x_now = x_need;
    }

    get_stack(1);
    get_box();
    y_now = 0;
  }
}

void goto_box() {
  box_success = 0;
  for (uint8_t g;g < 8;g++) {
    if (box_state[g] == true) {
      box_state[g] = !box_state[g];
      x_go = 1 + g;
      y_go = 0;
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
      shelf_state[h] = !shelf_state[h];
      if (h > 4) {
        shelf_floor = 2;
      }else {
        shelf_floor = 1;
      }
      
      shelf_now = (h%4) + 1;
      if (h%4 > 1) {
        x_go = 0;
        y_go = 4;
        shelf_stack = 4 - (h%4);
      }else {
        x_go = 8;
        y_go = 4;
        shelf_stack = (h%4) + 1;
      }
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


  //get_box_1
  arm_set(1);
  open_arm();
  get_stack(1);
  robot_left();
  get_stack(1);
  robot_left();
  get_stack(1);
  get_box();
  x_now = 1,y_now = 0;
}

void loop() {
  while (box_success != 8 && shelf_success != 8) {
    goto_shelf();
    goto_box();
  }
}
