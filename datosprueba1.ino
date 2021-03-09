#include<Wire.h>

//----VARIABLES ACELEROMETRO----
const int MPU=0x68; 
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int V_Previo;
float Datos;
char Se_Envia[10];

//----VARIABLES BT----
int Ac[2];

//----VARIABLES EXTRAS----
int pinLed=10;
bool inicio=false; //Bandera de inicio de la prueba



void setup() {
  Serial.begin(9600); // Serial para el BT 
  pinMode(pinLed,OUTPUT);//Led indicador

  //Acelerometro
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);

}

void loop() {

  //Inicia acelerometro
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true);

  //Valores aceleracion 
  AcX=Wire.read()<<8|Wire.read();    
  AcY=Wire.read()<<8|Wire.read();  
  AcZ=Wire.read()<<8|Wire.read();  
  GyX=Wire.read()<<8|Wire.read();  
  GyY=Wire.read()<<8|Wire.read();  
  GyZ=Wire.read()<<8|Wire.read();
  
  if(Serial.available()>0)
  {
    inicio=Serial.read();
    {
      if(inicio==true)
      {
        digitalWrite(pinLed,HIGH);
     
        Serial.print(AcX); //Esta parte de la transmision no funciona.
        Serial.print(AcY);
        Serial.print(AcZ);
        
      }
      else
      if(inicio==false)
      {
        digitalWrite(pinLed,LOW);
      }
    }
  } 
}
