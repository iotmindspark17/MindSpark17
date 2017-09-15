#include <SPI.h>
#include <Ethernet.h>

const int trigPin = 3; 
const int echoPin = 2; 
 
int duration;
float distance;

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; //Make sure your mac address is unique in your network
EthernetClient client;

char server[] = "server"; //ThingWorx server hostname. Change this to your server
int port=8080; //ThingWorx server port. Change as per your server port
char appKey[] = "<your_app-key>"; //App key you created
char thingName[] = "<DistanceSensor_<username>>"; //Thing Name Should match your thing in ThingWorx
char serviceName[] = "setDistance"; //service name should be same exist in the Thing Name you specified
char propertyName[]="distance";

int timeBetweenRefresh = 1000; //refresh interval in milliseconds
unsigned long lastConnectionTime = 0;

void setup() {
  Serial.begin(9600);
  
  Serial.println("initializing ethernet");
  Serial.println("Trying to get an IP address using DHCP");
  Ethernet.begin(mac);      //Initialize the ethernet with the provided MAC. Ethernet sheild will also get an IP thought DHCP
  Serial.print("My IP address: ");
  Serial.print(Ethernet.localIP());
  Serial.println();
  
  Serial.print("initializing arduino pins");
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  if (millis() - lastConnectionTime > timeBetweenRefresh){
    updateValues();   //uploading data to ThingWorx
  }    
  else{
    updateSensorsValue(); //updating senror data locally
  }
}
void updateSensorsValue(){
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

void updateValues()
{
  if (client.connect(server, port)) {  
      if(client.connected()){
        // Sending a header of a network packet  
        Serial.println("Sending data to ThingWorx Server...\n");  
        Serial.print("POST /Thingworx/Things/");  client.print("POST /Thingworx/Things/"); 
        Serial.print(thingName);  client.print(thingName);  
        Serial.print("/Services/");  client.print("/Services/");  
        Serial.print(serviceName);   client.print(serviceName); 
        Serial.print("?appKey=");    client.print("?appKey=");  
        Serial.print(appKey);        client.print(appKey);  
        Serial.print("&method=post&x-thingworx-session=true");  client.print("&method=post&x-thingworx-session=true"); 
        Serial.print("&");  client.print("&");  
        Serial.print(propertyName); client.print(propertyName);  
        Serial.print("=");  client.print("=");
      
        Serial.print(distance);  client.print(distance);  
        
        Serial.println(" HTTP/1.1");  client.println(" HTTP/1.1");  
        Serial.println("Accept: application/json");  client.println("Accept: application/json");  
        Serial.print("Host: ");  client.print("Host: ");  
        Serial.println(server);  client.println(server);  
        Serial.println("Content-Type: application/json"); client.println("Content-Type: application/json");  
        Serial.println();  client.println();  

        client.stop();  
        lastConnectionTime = millis();
      }
    }
  else{
    Serial.println("Connection could not be established");  
    client.stop();
   }
}
