#include <DHT.h>

// DHT11 Temperature and Humidity Sensors Example
#include "DHT.h"         //include DHT library --> Goto Sketch--> Include Library -->
                         //Manager Library--> search for DHT (In Filter your search). 
                         //Select the DHT moreinfo and install the library if it is not installed.
#define DHTPIN 3         //define as DHTPIN the Pin 3 used to connect the Sensor
#define DHTTYPE DHT11    //define the sensor used(DHT11)
DHT dht(DHTPIN, DHTTYPE);//create an instance of DHT
/*setup*/

int delayPeriod = 3000;
void setup() {
  Serial.begin(9600);    //initialize the Serial communication
  delay(delayPeriod);           //wait 3 seconds
  Serial.println("Temperature and Humidity are");//Debug prints. See on Serial monitor. Got to Tools-->Serial Monitor in Ardiuno IDE. Select 9600
  Serial.println("T(C) \tH(%)");                   //
  dht.begin();           
}

/*loop*/
void loop() {
 
  float h = dht.readHumidity();    // reading Humidity 
  float t = dht.readTemperature(); // read Temperature as Celsius (the default)
  // check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!"); //too low values. is this an issue with the senor.
    return;
  }
  Serial.print(t, 2);    //print the temperature
  Serial.print("\t");
  Serial.println(h, 2);  //print the humidity
  delay(delayPeriod);           //wait 3 seconds
}
