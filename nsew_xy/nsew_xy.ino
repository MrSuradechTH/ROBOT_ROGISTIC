uint8_t x,y;
uint8_t x_max = 8,y_max = 4;
String compass[] = {"n","e","s","w"};
int8_t compass_now = 0;

void get_stack() {
  
}

void xy_to_xy(uint8_t xstart,uint8_t ystart,uint8_t xstop,uint8_t ystop) {
  if (ystart != 1) {
    if (ystart > 1) {
      compass_to("s");
      get_stack(ystart-1);
    }else if (ystart < 1) {
      compass_to("n");
      get_stack(1-ystart);
    }
  }
  if (xstart < xstop) {
    
  }else if (xstart > xstop) {
    
  }
}

void compass_to(String string_a) {
  if (string_a == "n") {
    if (compass[compass_now] == "e") {
      left();
    }else if (compass[compass_now] == "s") {
      opposite();
    }else if (compass[compass_now] == "w") {
      right();
    }
  }else if (string_a == "e") {
    if (compass[compass_now] == "s") {
      left();
    }else if (compass[compass_now] == "w") {
      opposite();
    }else if (compass[compass_now] == "n") {
      right();
    }
  }else if (string_a == "s") {
    if (compass[compass_now] == "w") {
      left();
    }else if (compass[compass_now] == "n") {
      opposite();
    }else if (compass[compass_now] == "e") {
      right();
    }
  }else if (string_a == "w") {
    if (compass[compass_now] == "n") {
      left();
    }else if (compass[compass_now] == "e") {
      opposite();
    }else if (compass[compass_now] == "s") {
      right();
    }
  }
}

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
  compass_to("e");
  //left();
  //right();
  //opposite();
}
