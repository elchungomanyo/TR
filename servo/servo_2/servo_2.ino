#include <Servo.h>
#include <SPI.h>
#include <SD.h>

Servo servo1;
File dataFile;

const int fsrAnalogPin = 0;
const int servoPin = 9;
const int minPulse = 1000;
const int maxPulse = 2900;
const int LEDpin = 8;

int fsrReading;
int servoAngle;
int firstTime;

void setup(void) {
  servo1.attach(servoPin, minPulse, maxPulse);
  Serial.begin(9600);
  digitalWrite(LEDpin, LOW);
  while (!Serial) {
    ;
  }

Serial.print("inicializando SD card...");

if (!SD.begin(4)) {
  Serial.println("fallo en inicializacion.");
  return;
}
Serial.println("inicializacion completada.");
dataFile = SD.open("data.txt", FILE_WRITE);

if (dataFile)
{
  Serial.print("Escribiendo en data.txt...");
  dataFile.println("testing...");
  dataFile.close();
  Serial.println("Completado");
}
else 
{
  Serial.println("Error al abrir data.txt");
}

}

void loop(void) {
  fsrReading = analogRead(fsrAnalogPin);
  Serial.println(fsrReading);


  if(fsrReading > 0 && firstTime == 0)
  {
    delay(1000);
    firstTime = 1;
  }
  else
  {
  if(fsrReading >= 100 && fsrReading < 300)  //Se impone una condición. Si es cumplida se realizarán las acciones siguientes.
  {
    if(servoAngle > 0)
    {
      servo1.write(0);
    }
    digitalWrite(LEDpin, HIGH);
    dataFile = SD.open("data.txt", FILE_WRITE);
    dataFile.println(fsrReading);
    dataFile.close();
    delay (1000);
  }
  else if(fsrReading >= 300)
  { 
    digitalWrite(LEDpin, LOW);
    servo1.write(servoAngle);
    servoAngle = 180;
    dataFile = SD.open("data.txt", FILE_WRITE);
    dataFile.println(fsrReading);
    dataFile.close();
    delay(700);
    
  }
  else if(fsrReading > 0 && fsrReading < 100)
  {
   digitalWrite(LEDpin, LOW);
   dataFile = SD.open("data.txt", FILE_WRITE);
   dataFile.println(fsrReading);
   dataFile.close();
   servoAngle = 180;
   servo1.write(servoAngle);
   delay(1000);
  }
  else if(!fsrReading)
  {
   digitalWrite(LEDpin, LOW);
   servoAngle = 0;
   servo1.write(servoAngle);
   delay(200);
  }
  firstTime = 0;
  }

  
  
  delay(20);  //Concedemos un espacio de tiempo entre acciones de 20 ms.

}
