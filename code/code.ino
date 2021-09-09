//motor
const uint8_t motor_row = 6,motor_col = 3;
uint8_t motor[motor_row][motor_col] = {{32, 33, 4},{34, 35, 6},{36, 37, 8},{48, 49, 10},{50, 51, 12},{52, 53}}; //ล้อซ้าย,ล้อขวา,แขนซ้าย,แขนขวา,?,? (มองจากด้านหลัง)
uint8_t default_speed = 255;
uint8_t motor_speed[] = {default_speed,default_speed,default_speed,default_speed,default_speed,default_speed};

//robot
void robot_motor(uint8_t digital_a,uint8_t digital_b,uint8_t digital_c,uint8_t digital_d,uint8_t analog_a,uint8_t analog_b) {
  digitalWrite(motor[0][0], digital_a);
  digitalWrite(motor[0][1], digital_b);
  digitalWrite(motor[1][0], digital_c);
  digitalWrite(motor[1][1], digital_d);
  analogWrite(motor[0][2], analog_a);
  analogWrite(motor[1][2], analog_b);
}

void robot_forward(uint8_t analog_a,uint8_t analog_b) {
  robot_motor(1,0,1,0,analog_a,analog_b);
}

void robot_backward(uint8_t analog_a,uint8_t analog_b) {
  robot_motor(0,1,0,1,analog_a,analog_b);
}

void robot_right(uint8_t analog_a,uint8_t analog_b) {
  robot_motor(1,0,0,1,analog_a,analog_b);
}

void robot_left(uint8_t analog_a,uint8_t analog_b) {
  robot_motor(0,1,1,0,analog_a,analog_b);
}

void robot_opposite(uint8_t analog_a,uint8_t analog_b) {
  robot_motor(1,0,0,1,analog_a,analog_b);
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

void setup() {
  Serial.begin(9600);
  for(uint8_t a;a < motor_row;a++) {
    for(uint8_t b;b < motor_col;b++) {
      if(motor[a][b] != 0) {
        pinMode(motor[a][b], OUTPUT);
      }
    }
  }
}

void loop() {
//  robot_forward(motor_speed[0],motor_speed[1]);  //เดินหน้า
//  robot_backward(motor_speed[0],motor_speed[1]); //ถอยหลัง
//  robot_right(motor_speed[0],motor_speed[1]);    //เลี้ยวขวา
//  robot_left(motor_speed[0],motor_speed[1]);     //เลี้ยวซ้าย
//  robot_opposite(motor_speed[0],motor_speed[1]); //กลับหลัง
//  arm_up(motor_speed[2],motor_speed[3]);         //เเขนกลขึ้น
//  arm_down(motor_speed[2],motor_speed[3]);       //แขนกลลง
}
