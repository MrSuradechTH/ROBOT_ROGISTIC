//nesw_xy
uint8_t x,y;
String compass[] = {"n","e","s","w"};
int8_t compass_now = 0;

//color
#define S0 13
#define S1 11
#define S2 3
#define S3 5
#define OUT 9
const uint8_t color_all = 4;
uint8_t max_min = 15;
uint8_t delay_color_read = 10;
String color_name[] = {"red","green","yellow","blue"};
String color_stack[4][3] = {{"","",""},{"","",""},{"","",""},{"","",""}}; //[color_all][check_color_loop]
String color_stack_end[color_all] = {"","","",""};
uint8_t check_color_loop = 3;
uint8_t color_stack_loop,color_stack_loop_end;
const uint8_t rgb_stack_row = 4,rgb_stack_col = 3;
uint8_t rgb_stack[rgb_stack_row][rgb_stack_col] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
uint8_t rgb[] = {0,0,0};
const uint8_t color_row = 4,color_col = 3;
uint8_t color_calibrate[color_row][color_col] = {{0,0,0,},{0,0,0,},{0,0,0,},{0,0,0,}};
String color;

//motor
const uint8_t motor_row = 4,motor_col = 3;
uint8_t motor[motor_row][motor_col] = {{32, 33, 4},{34, 35, 6},{36, 37, 8},{48, 49, 10}}; //ล้อซ้าย,ล้อขวา,แขนซ้าย,แขนขวา (มองจากด้านหลัง)
const uint8_t motor_speed_row = 6,motor_speed_col = 2;
uint8_t motor_speed_default[] = {255,255}; //แก้ตรงนี้ให้เป็นค่าที่หุ่นเดินตรง **********
uint8_t motor_speed[motor_speed_row][motor_speed_col] = {{motor_speed_default[0],motor_speed_default[1]},{motor_speed_default[0],motor_speed_default[1]},{0,255},{255,0},{255,0},{0,0},}; //เดินหน้า,ถอยหลัง,เลี้ยวซ้าย,เลี้ยวขวา,กลับหลัง,หยุด (มอเตอร์ซ้าย,มอเตอร์ขวา)
uint8_t motor_delay[] = {0,0,1000,1000,2000,0}; //เดินหน้า,ถอยหลัง,เลี้ยวซ้าย,เลี้ยวขวา,กลับหลัง,หยุด

//robot
uint8_t speed_down_list[] = {10,5}; //ค่าความเร็วที่ลดลงเมื่อจะหลุดเส้น อันแรกหลุดมาก อันสองหลุดน้อย **********
uint8_t speed_down[] = {speed_down_list[0],speed_down_list[1]}; //int((speed_down_list[0]+speed_down_list[1])/2) ยังไม่มีการใช้งาน

//line_sensor
uint8_t line_sensor[] = {19,22,17,18,15,16,14};
String line_status; //line_status_old ยังไม่มีการใช้งาน

//other
uint8_t stack;
//String direction_controling = ""; //forward,backward,right,left,opposite (not yet used)

//xy_to_shelf
//รอแก้ตอนสนามเสร็จ
void xy14_to_shelf1() {
  compass_to("n");
  get_stack(1);
}

void shelf1_to_xy14() {
  compass_to("s");
  get_stack(1);
}

void xy24_to_shelf2() {
  compass_to("e");
  get_stack(1);
  compass_to("n");
  get_stack(1);
}

void shelf2_to_xy24() {
  compass_to("s");
  get_stack(1);
  compass_to("w");
  get_stack(1);
}

void xy34_to_shelf3() {
  compass_to("e");
  get_stack(1);
  compass_to("n");
  get_stack(1);
}

void shelf3_to_xy34() {
  compass_to("s");
  get_stack(1);
  compass_to("w");
  get_stack(1);
}

void xy44_to_shelf4() {
  compass_to("e");
  get_stack(1);
  compass_to("n");
  get_stack(1);
}

void shelf4_to_xy44() {
  compass_to("s");
  get_stack(1);
  compass_to("w");
  get_stack(1);
}

//nesw_xy
void xy_to_xy(uint8_t xstart,uint8_t ystart,uint8_t xstop,uint8_t ystop) { //xy_to_xy(x,y,xstop,ystop);
  if (ystart != 1) {
    if (ystart > 1) {
      compass_to("s");
      y -= int(ystart - 1);
      get_stack(ystart - 1);
    }else if (ystart < 1) {
      compass_to("n");
      y += int(1 - ystart);
      get_stack(1 - ystart);
    }
  }
  if (xstart < xstop) {
    compass_to("w");
    x += int(xstop - xstart);
    get_stack(xstop - xstart);
  }else if (xstart > xstop) {
    compass_to("e");
    x -= int(xstart - xstop);
    get_stack(xstart - xstop);
  }

  if (ystart > ystop) {
    compass_to("s");
    y -= int(1 - ystop);
    get_stack(1 - ystop);
  }else if (ystart < ystop) {
    compass_to("n");
    y += int(ystop - 1);
    get_stack(ystop - 1);
  }
}

void compass_to(String string_a) {
  if (string_a == "n") {
    if (compass[compass_now] == "e") {
      robot_left(motor_speed[3][0],motor_speed[3][1],motor_delay[3]);
    }else if (compass[compass_now] == "s") {
      robot_opposite(motor_speed[4][0],motor_speed[4][1],motor_delay[4]);
    }else if (compass[compass_now] == "w") {
      robot_right(motor_speed[2][0],motor_speed[2][1],motor_delay[2]);
    }
  }else if (string_a == "e") {
    if (compass[compass_now] == "s") {
      robot_left(motor_speed[3][0],motor_speed[3][1],motor_delay[3]);
    }else if (compass[compass_now] == "w") {
      robot_opposite(motor_speed[4][0],motor_speed[4][1],motor_delay[4]);
    }else if (compass[compass_now] == "n") {
      robot_right(motor_speed[2][0],motor_speed[2][1],motor_delay[2]);
    }
  }else if (string_a == "s") {
    if (compass[compass_now] == "w") {
      robot_left(motor_speed[3][0],motor_speed[3][1],motor_delay[3]);
    }else if (compass[compass_now] == "n") {
      robot_opposite(motor_speed[4][0],motor_speed[4][1],motor_delay[4]);
    }else if (compass[compass_now] == "e") {
      robot_right(motor_speed[2][0],motor_speed[2][1],motor_delay[2]);
    }
  }else if (string_a == "w") {
    if (compass[compass_now] == "n") {
      robot_left(motor_speed[3][0],motor_speed[3][1],motor_delay[3]);
    }else if (compass[compass_now] == "e") {
      robot_opposite(motor_speed[4][0],motor_speed[4][1],motor_delay[4]);
    }else if (compass[compass_now] == "s") {
      robot_right(motor_speed[2][0],motor_speed[2][1],motor_delay[2]);
    }
  }
}

//color
void get_rgb() {
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  rgb_stack[0][0] = pulseIn(OUT, LOW);
  delay(delay_color_read);
  rgb_stack[0][1] = pulseIn(OUT, LOW);
  delay(delay_color_read);
  rgb_stack[0][2] = pulseIn(OUT, LOW);
  delay(delay_color_read);
  rgb[0] = int((rgb_stack[0][0]+rgb_stack[0][0]+rgb_stack[0][0])/3);
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  rgb_stack[1][0] = pulseIn(OUT, LOW);
  delay(delay_color_read);
  rgb_stack[1][1] = pulseIn(OUT, LOW);
  delay(delay_color_read);
  rgb_stack[1][2] = pulseIn(OUT, LOW);
  delay(delay_color_read);
  rgb[1] = int((rgb_stack[1][0]+rgb_stack[1][0]+rgb_stack[1][0])/3);
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  rgb_stack[2][0] = pulseIn(OUT, LOW);
  delay(delay_color_read);
  rgb_stack[2][1] = pulseIn(OUT, LOW);
  delay(delay_color_read);
  rgb_stack[2][2] = pulseIn(OUT, LOW);
  delay(delay_color_read);
  rgb[2] = int((rgb_stack[2][0]+rgb_stack[2][0]+rgb_stack[2][0])/3);
}

void set_color(uint8_t int_a) {
  Serial.println("color[" + String(int_a) + "][" + color_name[int_a] + "] will set!!!");
  delay(3000);
  get_rgb();
  color_calibrate[int_a][0] = rgb[0];
  color_calibrate[int_a][1] = rgb[1];
  color_calibrate[int_a][2] = rgb[2];
  Serial.println("COLOR[" + String(int_a) + "][" + color_name[int_a] + "] R[" + String(color_calibrate[int_a][0]) + "]" + "G[" + String(color_calibrate[int_a][1]) + "]" + "B[" + String(color_calibrate[int_a][2]) + "]");
}

String get_color() {
  for (uint8_t a = 0;a < color_all;a++) {
    get_rgb();
    for (uint8_t b = 0;b < check_color_loop;b++) {
      if (color_calibrate[a][0] - max_min < rgb[0] && rgb[0] < color_calibrate[a][0] + max_min && color_calibrate[a][1] - max_min < rgb[1] && rgb[1] < color_calibrate[a][1] + max_min && color_calibrate[a][2] - max_min < rgb[2] && rgb[2] < color_calibrate[a][2] + max_min) {
        color_stack[a][b] = color_name[a];
      }else {
        color_stack[a][b] = "unknow";
      }
    }
    color_stack_loop = 0;
    for (uint8_t c = 0;c < check_color_loop;c++) {
      if (color_stack[a][0] == color_stack[a][c]) {
          color_stack_loop += 1;
        }
    }
    if (color_stack_loop > int(check_color_loop/2) ) {
      color_stack_end[a] = color_stack[a][0];
    }else {
        color_stack_end[a] = "unknow";
    }
  }
  for (uint8_t a = 0;a < color_all;a++) {
    if (color_stack_end[a] != "unknow") {
      color = color_stack_end[a];
      break;
    }else if (a == color_all-1 && color_stack_end[a] == "unknow") {
      color = "unknow";
    }
  }
  //Serial.println("[" + color + "]");
  return color;
}

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
  Serial.println("right");
  compass_now += 1;
  if (compass_now > 3) {
    compass_now = 0;
  }
  robot_motor(1,0,0,1,analog_a,analog_b,int_a);
}

void robot_left(uint8_t analog_a,uint8_t analog_b,uint16_t int_a) {
  Serial.println("left");
  compass_now -= 1;
  if (compass_now < 0) {
    compass_now = 3;
  }
  robot_motor(0,1,1,0,analog_a,analog_b,int_a);
}

void robot_opposite(uint8_t analog_a,uint8_t analog_b,uint16_t int_a) {
  Serial.println("opposite");
  compass_now += 1;
  if (compass_now > 3) {
    compass_now = 0;
  }
  compass_now += 1;
  if (compass_now > 3) {
    compass_now = 0;
  }
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

//balance
void balance() {
  line_check();
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
  }else if (line_status == "0000000") {
    robot_stop(motor_speed[4][0],motor_speed[4][1],motor_delay[4]);
  }
//  digitalWrite(motor[0][0], 1);
//  digitalWrite(motor[0][1], 0);
//  digitalWrite(motor[1][0], 1);
//  digitalWrite(motor[1][1], 0);
//  analogWrite(motor[0][2], motor_speed[0][0]);
//  analogWrite(motor[1][2], motor_speed[0][1]);
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
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  delay(5000);
  Serial.println("strated");
  for (uint8_t a = 0;a < color_all;a++) {
    set_color(a);
  }
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
//compass
//Serial.println("[" + String(compass_now) + "][" + compass[compass_now] + "]");
  
//color read
  String color_reads = get_color();
  Serial.println(color_reads);

//  robot_forward(motor_speed[0][0],motor_speed[0][1],motor_delay[0]);
//  get_stack(1);
//line_check();
  //get_color(); //return string color
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
