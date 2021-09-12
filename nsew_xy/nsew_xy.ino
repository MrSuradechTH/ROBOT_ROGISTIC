uint8_t x,y;
String compass[] = {"n","e","s","w"};
int8_t compass_now = 0;

void get_stack() {
  
}

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

//control
//  robot_forward(motor_speed[0][0],motor_speed[0][1],motor_delay[0]);  //เดินหน้า
//  robot_backward(motor_speed[1][0],motor_speed[1][1],motor_delay[1]); //ถอยหลัง
//  robot_right(motor_speed[2][0],motor_speed[2][1],motor_delay[2]);    //เลี้ยวขวา
//  robot_left(motor_speed[3][0],motor_speed[3][1],motor_delay[3]);     //เลี้ยวซ้าย
//  robot_opposite(motor_speed[4][0],motor_speed[4][1],motor_delay[4]); //กลับหลัง
//  robot_stop(motor_speed[5][0],motor_speed[5][1],motor_delay[5]);     //หยุด
//  arm_up(motor_speed[2],motor_speed[3]);         //เเขนกลขึ้น
//  arm_down(motor_speed[2],motor_speed[3]);       //แขนกลลง

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
