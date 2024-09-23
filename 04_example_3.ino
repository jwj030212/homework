#define PIN_LED 13  

unsigned int count = 0;  
boolean toggle = LOW;     

void setup() {
  pinMode(PIN_LED, OUTPUT);  
  Serial.begin(115200);  
  while (!Serial) {
    ;  
  }
  Serial.println("Hello world!"); 
  digitalWrite(PIN_LED, toggle);  
}

void loop() {
  Serial.println(++count);
  Serial.println("Hello world!");
  toggle = toggle_state(toggle); 
  digitalWrite(PIN_LED, toggle); 
  delay(1000);  
}

boolean toggle_state(boolean toggle) {
  
  return !toggle;  
}
