#include <QMC5883LCompass.h>

QMC5883LCompass compass;

/***** เขียนสั่งมอเตอร์เปล่าๆก่อนให้เดินให้ตรง แล้วเอาค่ามาใส่ *****/


uint8_t motor[2][3] = {{36, 37, 8},{41, 40, 7}}; // มอรเตอร์ซ้าย,มอเตอร์ขวา ถ้าไม่ตรงให้แก้
uint8_t motor_speed_default[] = {45,40,48,48}; //แก้ตรงนี้ให้เป็นค่าที่หุ่นเดินตรง **********
//57,55
//30,28
uint8_t speed_down[] = {15};
uint8_t line_sensor[] = {28,29,30,31,32}; //กลาง 3 ตัวแรก 2 ตัวหลังอ่านข้าง แยกกัน
String line_status; //line_status_old ยังไม่มีการใช้งาน
int stack;

String all_compass[] = {"n","e","s","w"}; 
int compass_now = 0;
int max_min = 1,degree_min,degree_max;
int degree,degree_go,degree_set;
const int all_stack = 21; //ต้องเป็นเลขคี่
int degree_stack[all_stack];

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
  robot_stop();
}

void line_check() {
  line_status = "";
  for(uint8_t c = 0;c < 3;c++) {
    line_status += String(digitalRead(line_sensor[c]));
  }
//  Serial.println(line_status);
}

void balance() {
  line_check();
  if (line_status == "010" || line_status == "111") {
      robot_forward();
  }else if (line_status == "100" || line_status == "110") {
      robot_motor(1,0,1,0,motor_speed_default[0] - speed_down[0],motor_speed_default[1] + speed_down[0],0);
  }else if (line_status == "001" || line_status == "011") {
      robot_motor(1,0,1,0,motor_speed_default[0] + speed_down[0],motor_speed_default[0] - speed_down[0],0);
  }

  if (digitalRead(31) == 1 || digitalRead(32) == 1) {
    stack++;
    Serial.println(stack);
    while(digitalRead(31) == 1 || digitalRead(32) == 1) {
      line_check();
      if (line_status == "010" || line_status == "111") {
        robot_forward();
      }else if (line_status == "100" || line_status == "110") {
          robot_motor(1,0,1,0,motor_speed_default[0] - speed_down[0],motor_speed_default[1] + speed_down[0],0);
      }else if (line_status == "001" || line_status == "011") {
          robot_motor(1,0,1,0,motor_speed_default[0] + speed_down[0],motor_speed_default[0] - speed_down[0],0);
      }
    }
  }
}

void get_degree() {
  compass.read();
  degree = compass.getAzimuth();
  Serial.println(degree);
}

void set_degree() {
  degree_set = degree;
}

bool degree_stack_check() {
  bool st = false;
  for (int a = 0;a < all_stack;a++) {
    Serial.println(String(degree_set) + " " + String(degree) + " " + String(degree_stack[a]) + " " + String(digitalRead(29)));
    degree_stack[a] = int(degree - (all_stack / 2) + a);
    if (degree_stack[a] < 0) {
      degree_stack[a] = 360 - abs(degree_stack[a]);
    }else if (degree_stack[a] > 360) {
      degree_stack[a] = degree_stack[a] - 360;
    }

    if (degree_stack[a] == degree_set) {
      st = true;
      break;
    }
  }
  return st;
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
  for(uint8_t c = 0;c < 5;c++) {
    pinMode(line_sensor[c], INPUT);
  }
  get_degree();
}

void loop() {
  balance();
  if (stack == 5) {
    robot_right(90);
    stack = 0;
    while(true) {
      robot_stop();
    }
  }
}
