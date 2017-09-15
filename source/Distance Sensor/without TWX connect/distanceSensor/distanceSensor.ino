const int trigPin = 3; 
const int echoPin = 2; 
 
int duration;
float distance;

void setup() {
  Serial.begin(9600);
  
  Serial.println("initializing arduino pins");
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = (duration*.0343)/2;
    Serial.print("Distance: ");
    Serial.println(distance);
    delay(500); // waiting for .5 sec
}
