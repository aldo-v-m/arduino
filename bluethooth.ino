#include <SoftwareSerial.h> //Librería que permite establecer comunicación serie en otros pins
 
//Aquí conectamos los pins RXD,TDX del módulo Bluetooth.
SoftwareSerial BT(10,11); //10 RX, 11 TX.
 int pot=0;
void setup()
{
  BT.begin(9600); //Velocidad del puerto del módulo Bluetooth
  //Serial.begin(9600); //Abrimos la comunicación serie con el PC y establecemos velocidad
  BT.println("Conectado");
}
 
void loop()
{
  
  int ValorSensor=analogRead(A0);//Lee los datos que manda de manera analoga el sensor (potenciometro) 
  int ValorSensor1=analogRead(A1);//a traves del puerto 0 y lo almacena en la variable ValorSensor 
  int ValorSensor2=analogRead(A2);
  int Electromiograma=analogRead(A3);
 
  float Angulo1= -0.2604*ValorSensor+241.37;
  float Angulo2= 0.2568*ValorSensor1-18.541;   
  float Angulo3= 0.2489*ValorSensor2 -37.52;
  float EMG=5*Electromiograma/1024;
  
 
  BT.println(Angulo1);
  BT.println(Angulo2);
  BT.println(Angulo3);
  BT.println(EMG);
  
  delay(250); //Otorga una pausa de 0.1 segundos
}
