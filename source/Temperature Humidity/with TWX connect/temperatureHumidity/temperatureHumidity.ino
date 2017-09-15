#include <DHT.h>

// DHT11 Temperature and Humidity Sensors Example
#include "DHT.h"         //include DHT library --> Goto Sketch--> Include Library -->
                         //Manager Library--> search for DHT (In Filter your search). 
                         //Select the DHT moreinfo and install the library if it is not installed.
#define DHTPIN 3         //define as DHTPIN the Pin 3 used to connect the Sensor
#define DHTTYPE DHT11    //define the sensor used(DHT11)
DHT dht(DHTPIN, DHTTYPE);//create an instance of DHT
/*setup*/

#include <Ethernet.h> 
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};  
EthernetClient client; 
char server[] = "server"; //ThingWorx server hostname. Change this to your server
int port=8080; //ThingWorx server port. Change as per your server port
char appKey[] = "<your_app-key>"; //App key you created
char thingName[] = "THThing"; //Thing Name Should match your thing in ThingWorx 
char serviceName[] = "setTH"; //service name should be same exist in the Thing Name you specified
char propertyName1[]="temperature"; 
char propertyName2[]="humidity"; 
 
int timeBetweenRefresh = 1000; //refresh interval in milliseconds
unsigned long lastConnectionTime = 0; 

int delayPeriod = 3000;
float h, t;

void setup() {
  Serial.begin(9600);    //initialize the Serial communication
  //initializing ethernet 
  Serial.println("Trying to get an IP address using DHCP"); 
  Ethernet.begin(mac);      //Initialize the ethernet with the provided MAC. Ethernet sheild will also get an IP thought DHCP 
  Serial.print("My IP address: "); 
  Serial.print(Ethernet.localIP()); 
  Serial.println(); 
  //initializing arduino pins 
  delay(delayPeriod);           //wait 3 seconds
  Serial.println("Temperature and Humidity are");//Debug prints. See on Serial monitor. Got to Tools-->Serial Monitor in Ardiuno IDE. Select 9600
  Serial.println("T(C) \tH(%)");                   //
  dht.begin(); 
}

/*loop*/
void loop() {
    if (millis() - lastConnectionTime > timeBetweenRefresh){ 
    updateValues();   //uploading data to ThingWorx 
  }     
  else{ 
    updateSensorsValue(); //updating senror data locally 
  }
}

void updateSensorsValue(){ 
  h = dht.readHumidity();    // reading Humidity 
  t = dht.readTemperature(); // read Temperature as Celsius (the default)
  // check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!"); //too low values. is this an issue with the senor.
    return;
  }
  Serial.print(t, 2);    //print the temperature
  Serial.print("\t");
  Serial.println(h, 2);  //print the humidity
  delay(delayPeriod); 
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
        Serial.print(propertyName2); client.print(propertyName2);   
        Serial.print("=");  client.print("="); 
        Serial.print(h);  client.print(h); 
        
        Serial.print("&");  client.print("&");   
        Serial.print(propertyName1); client.print(propertyName1);   
        Serial.print("=");  client.print("="); 
        Serial.print(t);  client.print(t);     
         
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
