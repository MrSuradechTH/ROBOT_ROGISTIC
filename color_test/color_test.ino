#define S0 13
#define S1 11
#define S2 3
#define S3 5
#define OUT 9
uint8_t color_num;
//color set 1_pink
const uint8_t rgb_stack_row = 3,rgb_stack_col = 3;
uint8_t rgb_stack[rgb_stack_row][rgb_stack_col] = {{0,0,0},{0,0,0},{0,0,0}};
uint8_t rgb[] = {0,0,0};
uint8_t color_all = 1;
const uint8_t color_row = 3,color_col = 6;
uint8_t color_calibrate[color_row][color_col] = {{11,13,32,34,22,24},{5,20,20,40,15,30},{5,20,20,40,15,30}};
void get_rgb() {
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  rgb_stack[0][0] = pulseIn(OUT, LOW);
  delay(10);
  rgb_stack[0][1] = pulseIn(OUT, LOW);
  delay(10);
  rgb_stack[0][2] = pulseIn(OUT, LOW);
  delay(10);
  rgb[0] = int((rgb_stack[0][0]+rgb_stack[0][0]+rgb_stack[0][0])/3);
//  rgb_stack[0] = map(rgb_stack[0], 25,72,255,0);
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  rgb_stack[1][0] = pulseIn(OUT, LOW);
  delay(10);
  rgb_stack[1][1] = pulseIn(OUT, LOW);
  delay(10);
  rgb_stack[1][2] = pulseIn(OUT, LOW);
  delay(10);
  rgb[1] = int((rgb_stack[1][0]+rgb_stack[1][0]+rgb_stack[1][0])/3);
//  rgb_stack[1] = map(rgb_stack[1], 30,90,255,0);
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  rgb_stack[2][0] = pulseIn(OUT, LOW);
  delay(10);
  rgb_stack[2][1] = pulseIn(OUT, LOW);
  delay(10);
  rgb_stack[2][2] = pulseIn(OUT, LOW);
  delay(10);
  rgb[2] = int((rgb_stack[2][0]+rgb_stack[2][0]+rgb_stack[2][0])/3);
//  rgb_stack[2] = map(rgb_stack[2], 25,70,255,0);
//  Serial.println("R[" + String(rgb[0]) + "]" + "G[" + String(rgb[1]) + "]" + "B[" + String(rgb[2]) + "]");
}

void set_color(uint8_t int_a) {
  Serial.println("color[" + String(int_a) + "] will set!!!");
  delay(1000);
  Serial.println("min will set!!!");
  delay(3000);
  get_rgb();
  color_calibrate[int_a][0] = rgb[0];
  color_calibrate[int_a][2] = rgb[1];
  color_calibrate[int_a][4] = rgb[2];
  Serial.println("MIN R[" + String(color_calibrate[int_a][0]) + "]" + "G[" + String(color_calibrate[int_a][2]) + "]" + "B[" + String(color_calibrate[int_a][4]) + "]");
  Serial.println("max will set!!!"); 
  delay(3000);
  get_rgb();
  color_calibrate[int_a][1] = rgb[0];
  color_calibrate[int_a][3] = rgb[1];
  color_calibrate[int_a][5] = rgb[2];
  Serial.println("MAX R[" + String(color_calibrate[int_a][1]) + "]" + "G[" + String(color_calibrate[int_a][3]) + "]" + "B[" + String(color_calibrate[int_a][5]) + "]");
  color_num = 0;
}

void get_color() {
  if (color_calibrate[0][0] < rgb[0] && rgb[0] < color_calibrate[0][1] && color_calibrate[1][0] < rgb[1] && rgb[1] < color_calibrate[1][1] && color_calibrate[2][0] < rgb[2] && rgb[2] < color_calibrate[2][1]) {
    Serial.println("[red]R[" + String(rgb[0]) + "]" + "G[" + String(rgb[1]) + "]" + "B[" + String(rgb[2]) + "]");
  }else {
    Serial.println("[unknow]R[" + String(rgb[0]) + "]" + "G[" + String(rgb[1]) + "]" + "B[" + String(rgb[2]) + "]");
  }
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
  for (uint8_t a = 0;a < color_all;a++) {
    set_color(a);
  }
}
void loop() {
  get_rgb();
  get_color();
}
