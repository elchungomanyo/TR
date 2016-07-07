#include <Servo.h>
#include <SPI.h>
#include <SD.h>

Servo servo1;
File dataFile;

const int fsrAnalogPin = 0;
const int servoPin = 11;
const int minPulse = 1000;
const int maxPulse = 2900;
const int LEDpin = 13;

int fsrReading;
int servoAngle;
int firstTime;

void setup(void) {
  servo1.attach(servoPin, minPulse, maxPulse);
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

Serial.print("inicializando SD card...");

if (!SD.begin(4)) {
  Serial.println("fallo en inicialización.");
  return;
}
Serial.println("inicialización completada.");
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
    dataFile = SD.open("data.txt", FILE_WRITE);
    dataFile.println(fsrReading);
    dataFile.close();
    digitalWrite(LEDpin, HIGH);
    delay (1000);
  }
  else if(fsrReading >= 300)
  {
    dataFile = SD.open("data.txt", FILE_WRITE);
    dataFile.println(fsrReading);
    dataFile.close();
    digitalWrite(LEDpin, LOW);
    servoAngle = 180;
    servo1.write(servoAngle);
    delay(1000);
  }
  else if(fsrReading > 0 && fsrReading < 100)
  {
   dataFile = SD.open("data.txt", FILE_WRITE);
   dataFile.println(fsrReading);
   dataFile.close();
   digitalWrite(LEDpin, LOW);
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
