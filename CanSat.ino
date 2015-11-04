#include <SoftwareSerial.h>-
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <Servo.h>
#include <Wire.h>

#define DHTPIN 2 //Seleccionamos el pin en el que se //conectarÃ¡ el sensor
#define DHTTYPE DHT11 //Se selecciona el DHT11 (hay //otros DHT)
#define DHT11_PIN 0      // ADC0

Servo myservo;
SoftwareSerial xbee(1,0);
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
DHT dht(DHTPIN, DHTTYPE);
boolean band = false;

void setup()  
{
  xbee.begin(9600);
  //Serial.begin(9600);
  myservo.attach(9);
  myservo.write(180);
  
  dht.begin();
  if(!bmp.begin())
  {
    while(1);
  }
  delay(1000); 
}

void loop()
{
  bool newdata = false;
  unsigned long start = millis();
  
  if(band==true)
  {
    myservo.write(0);
  }
  
  sensors_event_t event;
  bmp.getEvent(&event);
  
  if (event.pressure)
  {
    //Calcular temperatura
    float temperature;
    bmp.getTemperature(&temperature);
    
    float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
    xbee.print("ProAugeCansat|");
    if(bmp.pressureToAltitude(seaLevelPressure,event.pressure) >= 4000)
    {
      band=true;
    }
    //Serial.print(bmp.pressureToAltitude(seaLevelPressure,event.pressure));//ALTITUD
    xbee.print(bmp.pressureToAltitude(seaLevelPressure,event.pressure));//ALTITUD
    //Serial.print(",");
    xbee.print(",");
    //Serial.print(temperature);//TEMPERATURA
    xbee.print(temperature);//TEMPERATURA
    //Serial.print(",");
    xbee.print(",");
    float h = dht.readHumidity(); //Se lee la humedad
    //Se imprimen las variables
    //Serial.print(h);//HUMEDAD
    xbee.print(h);//HUMEDAD
    //Serial.print(",");
    xbee.print(",");
    //Serial.print(event.pressure);//PRESION
    xbee.print(event.pressure);//PRESION
    //Serial.print("|");//PRESION
    xbee.print("|");//PRESION
    //Serial.println("");
    xbee.println("");
  }
  else
  {
    Serial.println("Sensor error");
  }
  delay(10000);
}

