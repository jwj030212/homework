#include <Servo.h>

// Arduino pin assignment
#define PIN_LED   9   // LED active-low
#define PIN_TRIG  12  // sonar sensor TRIGGER
#define PIN_ECHO  8   // sonar sensor ECHO
#define PIN_SERVO 10  // servo motor

// Configurable parameters for sonar
#define SND_VEL 346.0          // sound velocity at 24 celsius degree (unit: m/sec)
#define INTERVAL 25            // sampling interval (unit: msec)
#define PULSE_DURATION 10      // ultra-sound Pulse Duration (unit: usec)
#define TIMEOUT ((INTERVAL / 2) * 1000.0)  // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL)  // coefficient to convert duration to distance

#define _EMA_ALPHA 0.2         // EMA weight of new sample (range: 0 to 1)

// Target Distance
#define _DIST_MIN 180.0        // minimum distance to be measured (unit: mm)
#define _DIST_MAX 360.0        // maximum distance to be measured (unit: mm)
#define _TARGET_LOW  180.0        // target low distance (unit: cm)
#define _TARGET_HIGH 360.0        // target high distance (unit: cm)

// Duty duration for myservo.writeMicroseconds()
#define _DUTY_MIN 500          // servo full clockwise position (0 degree)
#define _DUTY_MAX 2500         // servo full counterclockwise position (180 degree)
#define _DUTY_NEU ((_DUTY_MIN + _DUTY_MAX) / 2) // servo neutral position (90 degree)

// Global variables
float dist_ema, dist_prev;   // unit: cm
unsigned long last_sampling_time;  // unit: ms

Servo myservo;

void setup() {
  // Initialize GPIO pins
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);    // Sonar TRIGGER
  pinMode(PIN_ECHO, INPUT);     // Sonar ECHO
  digitalWrite(PIN_TRIG, LOW);  // Turn-off Sonar

  myservo.attach(PIN_SERVO);
  myservo.writeMicroseconds(_DUTY_NEU);  // Start servo in neutral position

  // Initialize USS related variables
  dist_prev = _DIST_MAX;  // Assume starting at max distance

  // Initialize serial port
  Serial.begin(57600);

  // Initialize the EMA previous value
  dist_ema = dist_prev;
}

void loop() {
  float dist_raw;

 
  if (millis() < (last_sampling_time + INTERVAL))
    return;

  dist_raw = USS_measure(PIN_TRIG, PIN_ECHO);  
  
  dist_ema = _EMA_ALPHA * dist_raw + (1 - _EMA_ALPHA) * dist_ema;

  
  if (dist_ema <= _TARGET_LOW) {
    digitalWrite(PIN_LED, HIGH);  // LED ON (active low)
    myservo.writeMicroseconds(_DUTY_MIN);  // Move servo to 0 degrees
  } else if (dist_ema > _TARGET_LOW && dist_ema <= _TARGET_HIGH) {
    digitalWrite(PIN_LED, LOW);  // LED ON
    int servoPosition = map(dist_ema, _TARGET_LOW, _TARGET_HIGH, _DUTY_MIN, _DUTY_MAX);
    myservo.writeMicroseconds(servoPosition);  // Adjust servo proportionally
  } else if (dist_ema > _TARGET_HIGH) {
    digitalWrite(PIN_LED, HIGH);  // LED ON
    myservo.writeMicroseconds(_DUTY_MAX);  // Move servo to 180 degrees
  } else {
    digitalWrite(PIN_LED, HIGH);  // LED OFF
  }

 
  Serial.print("Min:"); Serial.print(_DIST_MIN);
  Serial.print(",dist:"); Serial.print(dist_raw);
  Serial.print(",ema:"); Serial.print(dist_ema);
  Serial.print(",Servo:"); Serial.print(myservo.read());
  Serial.print(",Max:"); Serial.print(_DIST_MAX);
  Serial.println("");
}


float USS_measure(int TRIG, int ECHO) {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);

  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; 
}
