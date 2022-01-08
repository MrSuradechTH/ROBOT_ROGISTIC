

/***** เขียนสั่งมอเตอร์เปล่าๆก่อนให้เดินให้ตรง แล้วเอาค่ามาใส่ *****/


uint8_t motor[2][3] = {{28, 29, 8},{30, 31, 9}}; // มอรเตอร์ซ้าย,มอเตอร์ขวา ถ้าไม่ตรงให้แก้
uint8_t motor_speed_default[] = {37,35,47,45}; //แก้ตรงนี้ให้เป็นค่าที่หุ่นเดินตรง **********
//57,55
//30,28
int millis_now,millis_old;
uint8_t speed_down[] = {3,5};
//uint8_t motor_speed[] = {motor_speed_default[0],motor_speed_default[1]};  //อันนี้ไม่ต้องแก้

uint8_t line_sensor[] = {47,48,49,50,51,52};
String line_status; //line_status_old ยังไม่มีการใช้งาน

//uint8_t speed_down[] = {52,50}; //ค่าความเร็วที่ลดลงเมื่อจะหลุดเส้น อันแรกหลุดมาก อันสองหลุดน้อย **********
//uint8_t speed_down[] = {speed_down_list[0],speed_down_list[1]}; //int((speed_down_list[0]+speed_down_list[1])/2) ยังไม่มีการใช้งาน

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
  robot_motor(0,0,0,0,0,0,1000);
}

void robot_left(int int_a) {
  robot_motor(0,1,1,0,motor_speed_default[2],motor_speed_default[3],int_a);
  robot_stop();
}

void robot_right(int int_a) {
  robot_motor(1,0,0,1,motor_speed_default[2],motor_speed_default[3],int_a);
  robot_stop();
}

void line_check() {
  //line_status_old = line_status;
  line_status = "";
  for(uint8_t c = 0;c < 6;c++) {
    line_status += String(!digitalRead(line_sensor[c]));
  }
  Serial.println(line_status);
}

void balance() {
  line_check();
  if (line_status == "001100" || line_status == "000100" || line_status == "001000") {
    robot_forward();
  }else if (line_status == "000000") {
    robot_stop();
  }else {
    if (line_status == "010000" || line_status == "011000" || line_status == "011100") {
      robot_motor(1,0,1,0,motor_speed_default[0] - speed_down[0],motor_speed_default[1] + speed_down[0],0);
    }else if (line_status == "000010" || line_status == "000110" || line_status == "001110") {
      robot_motor(1,0,1,0,motor_speed_default[0] + speed_down[0],motor_speed_default[0] - speed_down[0],0);
    }
  }
}

void setup() {
  Serial.begin(9600);
  for(uint8_t a = 0;a < 2;a++) {
    for(uint8_t b = 0;b < 3;b++) {
      pinMode(motor[a][b], OUTPUT);
    }
  }
  for(uint8_t c = 0;c < 6;c++) {
    pinMode(line_sensor[c], INPUT);
  }
}

void loop() {
//  robot_motor(1,0,1,0,motor_speed_default[0],motor_speed_default[1],0);
  balance();
  if (!digitalRead(line_sensor[0]) == 1 || !digitalRead(line_sensor[5]) == 1) {
    Serial.println("yes");
    robot_motor(1,0,1,0,motor_speed_default[2],motor_speed_default[3],500);
    while(!digitalRead(line_sensor[4]) == 1 || !digitalRead(line_sensor[5]) == 1) {
      robot_left(0);
    }
    while(true) {
      robot_stop();
    }
  }
  delay(10);

  
//Serial.println(String(!digitalRead(line_sensor[0])) + ":" + String(!digitalRead(line_sensor[5])));
//  robot_motor(1,0,1,0,motor_speed_default[0],motor_speed_default[1],900);
//  robot_left(1200);
//  while(true) {
//      robot_stop();
//    }
//  robot_right(950);
//  while(true) {
//    robot_stop();
//  }
}
