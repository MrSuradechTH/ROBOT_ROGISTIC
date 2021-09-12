String compass[] = {"n","e","s","w"};
int8_t compass_now = 0;

void left() {
  Serial.println("left");
  compass_now -= 1;
  if (compass_now < 0) {
    compass_now = 3;
  }
}

void right() {
  Serial.println("right");
  compass_now += 1;
  if (compass_now > 3) {
    compass_now = 0;
  }
}

void opposite() {
  Serial.println("opposite");
  compass_now += 1;
  if (compass_now > 3) {
    compass_now = 0;
  }
  compass_now += 1;
  if (compass_now > 3) {
    compass_now = 0;
  }
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("[" + String(compass_now) + "][" + compass[compass_now] + "]");
  delay(1000);
  //left();
  //right();
  //opposite();
}
