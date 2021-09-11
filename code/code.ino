//motor
const uint8_t motor_row = 4,motor_col = 3;
uint8_t motor[motor_row][motor_col] = {{32, 33, 4},{34, 35, 6},{36, 37, 8},{48, 49, 10}}; //ล้อซ้าย,ล้อขวา,แขนซ้าย,แขนขวา (มองจากด้านหลัง)
const uint8_t motor_speed_row = 6,motor_speed_col = 2;
uint8_t motor_speed_default = 255;
uint8_t motor_speed[motor_speed_row][motor_speed_col] = {{motor_speed_default,motor_speed_default},{motor_speed_default,motor_speed_default},{0,255},{255,0},{255,0},{0,0},}; //เดินหน้า,ถอยหลัง,เลี้ยวซ้าย,เลี้ยวขวา,กลับหลัง,หยุด (มอเตอร์ซ้าย,มอเตอร์ขวา)
uint8_t motor_delay[] = {0,0,1000,1000,2000,0}; //เดินหน้า,ถอยหลัง,เลี้ยวซ้าย,เลี้ยวขวา,กลับหลัง,หยุด

//robot
uint8_t speed_down_list[] = {50,25};
uint8_t speed_down[] = {speed_down_list[0],int((speed_down_list[0]+speed_down_list[1])/2),speed_down_list[1]};

//line_sensor
uint8_t line_sensor[] = {19,22,17,18,15,16,14};
String line_status; //,line_status_old

//other
uint8_t stack;
//String direction_controling = ""; //forward,backward,right,left,opposite (not yet used)

//robot
void robot_motor(uint8_t digital_a,uint8_t digital_b,uint8_t digital_c,uint8_t digital_d,uint8_t analog_a,uint8_t analog_b,uint16_t int_a) {
  digitalWrite(motor[0][0], digital_a);
  digitalWrite(motor[0][1], digital_b);
  digitalWrite(motor[1][0], digital_c);
  digitalWrite(motor[1][1], digital_d);
  analogWrite(motor[0][2], analog_a);
  analogWrite(motor[1][2], analog_b);
  if (int_a != 0) {
    delay(int_a);
    robot_stop(motor_speed[4][0],motor_speed[4][1],motor_delay[4]);
  }
}

void robot_forward(uint8_t analog_a,uint8_t analog_b,uint16_t int_a) {
  robot_motor(1,0,1,0,analog_a,analog_b,int_a);
}

void robot_backward(uint8_t analog_a,uint8_t analog_b,uint16_t int_a) {
  robot_motor(0,1,0,1,analog_a,analog_b,int_a);
}

void robot_right(uint8_t analog_a,uint8_t analog_b,uint16_t int_a) {
  robot_motor(1,0,0,1,analog_a,analog_b,int_a);
}

void robot_left(uint8_t analog_a,uint8_t analog_b,uint16_t int_a) {
  robot_motor(0,1,1,0,analog_a,analog_b,int_a);
}

void robot_opposite(uint8_t analog_a,uint8_t analog_b,uint16_t int_a) {
  robot_motor(1,0,0,1,analog_a,analog_b,int_a);
}

void robot_stop(uint8_t analog_a,uint8_t analog_b,uint16_t int_a) {
  robot_motor(0,0,0,0,analog_a,analog_b,int_a);
}

//arm
void arm_motor(uint8_t digital_a,uint8_t digital_b,uint8_t digital_c,uint8_t digital_d,uint8_t analog_a,uint8_t analog_b) {
  digitalWrite(motor[2][0], digital_a);
  digitalWrite(motor[2][1], digital_b);
  digitalWrite(motor[3][0], digital_c);
  digitalWrite(motor[3][1], digital_d);
  analogWrite(motor[2][2], analog_a);
  analogWrite(motor[3][2], analog_b);
}

void arm_up(uint8_t analog_a,uint8_t analog_b) {
  arm_motor(1,0,1,0,analog_a,analog_b);
}

void arm_down(uint8_t analog_a,uint8_t analog_b) {
  arm_motor(0,1,0,1,analog_a,analog_b);
}

void line_check() {
  //line_status_old = line_status;
  line_status = "";
  for(uint8_t c = 0;c < 7;c++) {
    line_status += String(!digitalRead(line_sensor[c]));
  }
  Serial.println(line_status);
}

//รอปรับแต่ง (not yet used)
//void reject_rejoin() {
//  line_check();
//  while (line_status_old == line_status) {
//    line_check();
//    if (
//  }
//  while (line_status != "000" || line_status != "000" || line_status != "000") {
//      
//    }
//}

void balance() {
  if (line_status == "0100000" || line_status == "0101000" || line_status == "0110000" || line_status == "0110000") {
    motor_speed[0][0] = motor_speed[0][0] - speed_down[0];
  }else if (line_status == "0010000" || line_status == "0010000"){
    motor_speed[0][0] = motor_speed[0][0] - speed_down[1];
  }else if (line_status == "0000010" || line_status == "0001010" || line_status == "0000110" || line_status == "0001110") {
    motor_speed[0][1] = motor_speed[0][1] - speed_down[0];
  }else if (line_status == "0000100" || line_status == "0001100"){
    motor_speed[0][1] = motor_speed[0][1] - speed_down[1];
  }else if (line_status == "0001000" || line_status == "0011100") {
    motor_speed[0][0] = motor_speed_default;
    motor_speed[0][1] = motor_speed_default;
  }
  analogWrite(motor[0][2], motor_speed[0][0]);
  analogWrite(motor[1][2], motor_speed[0][1]);
}

void get_stack(uint8_t stack_count) {
  while(true) {
    line_check();
    if (line_status == "1000001" || line_status == "0000001" || line_status == "1000000") {
      stack += 1;
    }else {
      balance();
    }
    if (stack > stack_count) {
      stack = 0;
      break;
    }
  }
}

void setup() {
  Serial.begin(9600);
  for(uint8_t a = 0;a < motor_row;a++) {
    for(uint8_t b = 0;b < motor_col;b++) {
      if(motor[a][b] != 0) {
        pinMode(motor[a][b], OUTPUT);
      }
    }
  }
  for(uint8_t c = 0;c < 7;c++) {
    pinMode(line_sensor[c], INPUT);
  }
}

void loop() {
//  robot_forward(motor_speed[0][0],motor_speed[0][1],motor_delay[0]);
//  get_stack(1);
line_check();
  
//debug
//  line_check();
//  delay(100);

//debug
//  for(uint8_t a = 0;a < motor_row;a++) {
//    for(uint8_t b = 0;b < motor_col;b++) {
//        if(motor[a][b] != 0) {
//          Serial.println("[" + String(a) + "][" + String(b) + "] = " + String(motor[a][b]));
//          delay(1000);
//        }
//    }
//  }

//control
//  robot_forward(motor_speed[0][0],motor_speed[0][1],motor_delay[0]);  //เดินหน้า
//  robot_backward(motor_speed[1][0],motor_speed[1][1],motor_delay[1]); //ถอยหลัง
//  robot_right(motor_speed[2][0],motor_speed[2][1],motor_delay[2]);    //เลี้ยวขวา
//  robot_left(motor_speed[3][0],motor_speed[3][1],motor_delay[3]);     //เลี้ยวซ้าย
//  robot_opposite(motor_speed[4][0],motor_speed[4][1],motor_delay[4]); //กลับหลัง
//  robot_stop(motor_speed[5][0],motor_speed[5][1],motor_delay[5]);     //หยุด
//  arm_up(motor_speed[2],motor_speed[3]);         //เเขนกลขึ้น
//  arm_down(motor_speed[2],motor_speed[3]);       //แขนกลลง
}







//stack ตามจำนวนเส้นตัด line sensor ตัวแรกและตัวสุดท้าย

//0100000,0101000,0110000,0111000
//0010000,0011000
//0000010,0001010,0000110,0001110
//0000100,0001100
