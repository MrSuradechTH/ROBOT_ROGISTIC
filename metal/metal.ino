uint8_t metal_pin = 5;
uint8_t matal_check[] = {0,0,0,0,0,0,0,0};

void check_metal() {
  metal_check[x-1] = digitalRead(metal_pin);
  if (metal_check[x-1] == 1) {
    Serial.println("have metal");
  }else if (metal_check[x-1] == 0) {
    Serial.println("don't have metal");
  }
}
void setup() {
  Serial.begin(9600);
  pinMode(metal_pin, INPUT);
}

void loop() {
  
}
