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

void get_color() {
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
  Serial.println("[" + color + "]");
  return color;
}

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  Serial.begin(9600);
  delay(5000);
  Serial.println("strated");
  for (uint8_t a = 0;a < color_all;a++) {
    set_color(a);
  }
}
void loop() {
  get_color();
}
