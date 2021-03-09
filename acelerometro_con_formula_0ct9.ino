// Librerias I2C para controlar el mpu6050
// la libreria MPU6050.h necesita I2Cdev.h, I2Cdev.h necesita Wire.h
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

//Librería que permite establecer comunicación serie en otros pins
#include <SoftwareSerial.h> 

//Aquí conectamos los pins RXD,TDX del módulo Bluetooth.
SoftwareSerial BT(10,11); //10 RX, 11 TX.

// La dirección del MPU6050 puede ser 0x68 o 0x69, dependiendo 
// del estado de AD0. Si no se especifica, 0x68 estará implicito
MPU6050 sensor;

// Valores RAW (sin procesar) del acelerometro y giroscopio en los ejes x,y,z
int ax, ay, az;
//int gx, gy, gz;

const int numLecturas=3;
float lecturas_z[numLecturas];
int indice = 0;
float promedioz = 0;
float total_z = 0;

void setup() {
  Serial.begin(9600);    //Iniciando puerto serial
  Wire.begin();           //Iniciando I2C  
  sensor.initialize();    //Iniciando el sensor

  BT.begin(9600); //Velocidad del puerto del módulo Bluetooth  

  for (int lecturaActual = 0; lecturaActual < numLecturas; lecturaActual++)
    lecturas_z[lecturaActual] = 0;
}

void loop() {
  // Leer las aceleraciones y velocidades angulares
  sensor.getAcceleration(&ax, &ay, &az);

  float az_m_s2 = az * (9.81/16384.0);
    
  //Almacenamiento en el vector para promediado del sensor 1
  total_z = total_z - lecturas_z[indice];
  lecturas_z[indice] = az_m_s2;
  total_z = total_z + lecturas_z[indice];

  //promediado de ambos sensores
  indice = indice + 1;
  if(indice >= numLecturas)
    indice = 0;
  promedioz = total_z / numLecturas;
  
  //Mostrar las lecturas 
  Serial.println(promedioz);
  BT.println(promedioz);
  BT.flush();
  Serial.flush();

  delay (7);
  /*delay(10); //Frecuencia de muestreo de 100 Hz */
}
