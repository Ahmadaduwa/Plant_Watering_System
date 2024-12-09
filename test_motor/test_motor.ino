
#define MOTOR_IN1 12
#define MOTOR_IN2 14



void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
}

void loop() {
    digitalWrite(MOTOR_IN1, HIGH);
    digitalWrite(MOTOR_IN2, LOW);
  delay(1000);
}