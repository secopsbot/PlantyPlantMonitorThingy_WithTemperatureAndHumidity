/*
  Board	          int.0	  int.1	  int.2	  int.3	  int.4	  int.5
 Uno, Ethernet	  2	  3
 Mega2560	  2	  3	  21	  20	  19	  18
 Leonardo	  3	  2	  0	  1
 Due	          (any pin, more info http://arduino.cc/en/Reference/AttachInterrupt)
 */

#include <idDHT11.h>

int idDHT11pin = 2; //Digital pin for communicating with Temperature/Humidity Device.
int idDHT11intNumber = 0; //interrupt number (must be the one that use the previus defined pin (see table above)
int led13 = 13; //Pin13 (onboard LED) used for testing
int mostureSensor = 0; //Analog Pin 0

//declaration
void dht11_wrapper(); // must be declared before the lib initialization

// Lib instantiate
idDHT11 DHT11(idDHT11pin,idDHT11intNumber,dht11_wrapper);

void setup()
{
  Serial.begin(9600);
  Serial.println("Matt's Plant Monitor");
  Serial.println("---------------");
  pinMode(led13, OUTPUT);
}
// This wrapper is in charge of calling 
// must be defined like this for the lib work
void dht11_wrapper() {
  DHT11.isrCallback();
}
void loop()
{
  Serial.print("\nRetrieving information from moisture sensor: ");
  int sensorValue = analogRead(mostureSensor);
  
  Serial.println(sensorValue);
  //Check the sensor value and perform an action when in threshold <400 = WET | >=400 <=700 OK | >700 dryish | >900 probably dead 
  if (sensorValue > 900)
   {
    //Blink RED
    digitalWrite(led13, HIGH);
   }
  else if (sensorValue <= 900 && sensorValue > 700)
   {
    //Solid RED
    digitalWrite(led13, HIGH);
   }
  else if (sensorValue >= 400 && sensorValue <= 700)
   {
    //Solid GREEN
    digitalWrite(led13, HIGH);
   }
  else if (sensorValue < 400)
   {
    //Solid BLUE
    digitalWrite(led13, HIGH);
   }
   
  delay(1000);        // delay 1 second between reads
  
  Serial.print("\nRetrieving information from sensor: ");
  Serial.print("Read sensor: ");

  DHT11.acquire();
  while (DHT11.acquiring())
    ;
  int result = DHT11.getStatus();
  switch (result)
  {
  case IDDHTLIB_OK: 
    Serial.println("OK"); 
    break;
  case IDDHTLIB_ERROR_CHECKSUM: 
    Serial.println("Error\n\r\tChecksum error"); 
    break;
  case IDDHTLIB_ERROR_ISR_TIMEOUT: 
    Serial.println("Error\n\r\tISR Time out error"); 
    break;
  case IDDHTLIB_ERROR_RESPONSE_TIMEOUT: 
    Serial.println("Error\n\r\tResponse time out error"); 
    break;
  case IDDHTLIB_ERROR_DATA_TIMEOUT: 
    Serial.println("Error\n\r\tData time out error"); 
    break;
  case IDDHTLIB_ERROR_ACQUIRING: 
    Serial.println("Error\n\r\tAcquiring"); 
    break;
  case IDDHTLIB_ERROR_DELTA: 
    Serial.println("Error\n\r\tDelta time to small"); 
    break;
  case IDDHTLIB_ERROR_NOTSTARTED: 
    Serial.println("Error\n\r\tNot started"); 
    break;
  default: 
    Serial.println("Unknown error"); 
    break;
  }
  Serial.print("Humidity (%): ");
  Serial.println(DHT11.getHumidity(), 2);

  Serial.print("Temperature (oC): ");
  Serial.println(DHT11.getCelsius(), 2);

  delay(2000);
}


