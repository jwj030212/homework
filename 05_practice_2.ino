int ledPin = 7; 

void setup() {
  pinMode(ledPin, OUTPUT);  
}

void loop() {
  
  digitalWrite(ledPin, HIGH);  
  delay(1000);  // 1초 대기
  
  
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPin, LOW);   
    delay(100);                  
    digitalWrite(ledPin, HIGH);  
    delay(100);                  
  }
}