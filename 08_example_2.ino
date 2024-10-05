// Arduino pin assignment
#define PIN_LED  9
#define PIN_TRIG 12   
#define PIN_ECHO 8   

// configurable parameters
#define SND_VEL 346.0     
#define INTERVAL 100      
#define PULSE_DURATION 10 
#define _DIST_MIN 100.0   
#define _DIST_MAX 300.0   
#define TIMEOUT ((INTERVAL / 2) * 1000.0)
#define SCALE (0.001 * 0.5 * SND_VEL) 

unsigned long last_sampling_time;   
void setup() {
  // initialize GPIO pins
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);  
  pinMode(PIN_ECHO, INPUT);   
  digitalWrite(PIN_TRIG, LOW); 
  
  // initialize serial port
  Serial.begin(57600);
}

void loop() {
  float distance;

  // wait until next sampling time. 
  if (millis() < (last_sampling_time + INTERVAL))
    return;

  distance = USS_measure(PIN_TRIG, PIN_ECHO); 

  if ((distance == 0.0) || (distance > _DIST_MAX)) {
      distance = _DIST_MAX + 10.0;   
      analogWrite(PIN_LED, 255);      
  } else if (distance < _DIST_MIN) {
      distance = _DIST_MIN - 10.0;   
      analogWrite(PIN_LED, 255);      
  } else {    // In desired Range
      int brightness = calculateBrightness(distance); 
      analogWrite(PIN_LED, brightness);       
  }

  // output the distance to the serial port
  Serial.print("Min:");        Serial.print(_DIST_MIN);
  Serial.print(",distance:");  Serial.print(distance);
  Serial.print(",Max:");       Serial.print(_DIST_MAX);
  Serial.println("");
  
  
  
  
  // update last sampling time
  last_sampling_time += INTERVAL;
}

// calculate LED brightness based on distance (100mm ~ 300mm)
int calculateBrightness(float distance) {
  if (distance <= 100) {
    // 100mm 이하는 밝기 255 (LED OFF)
    return 255;
  } else if (distance <= 200) {
   
    return map(distance, 100, 200, 255, 0);  
  } else if (distance <= 300) {
    
    return map(distance, 200, 300, 0, 255);  
  } else {
    return 255; 
  }
}


float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // unit: mm
}
