
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <TimerThree.h>
#include "Timer.h"


//-----------------------------PANTALLA LCD------------------------------------------------
LiquidCrystal lcd(42, 44, 46, 48, 50, 52); //Conexion de la lcd (RS, E, D4, D5, D6, D7)
int posX = 0; //Posicion del cursor
int posY = 0;

//-----------------------------TECLADO------------------------------------------------------
const byte filas = 2;     //Num de filas 
const byte columnas = 3;  //Num de columnas

char matriz[filas][columnas] =
{
  { '1', 'A'},
  { '2', 'B'},
}; //Matriz 2x2 con configuración de filas y columnas 
//Controla frecuencia en hz y resistencia

byte pinesFilas[filas] = {22, 24};       //Pines correspondientes a las filas
byte pinesColumnas[columnas] = {26, 28}; //Pines correspondientes a las columnas
Keypad teclado = Keypad( makeKeymap(matriz), pinesFilas, pinesColumnas, filas, columnas);


//-----------------------------GENERAR SEÑALES-----------------------------------------------------
const int pwmPin=5; //Salida del pwm 

//Variables de control
const int duty=200; //En us
int frec=150; //En Hz


//Variables auxiliares para calculos
float T;  //Calculo del periodo de la frecuencia en us
float d;  //Calculo del ciclo de trabajo en 10 bits

//Variables Auxiliares Modo Burst
bool burst=false;       //Bandera Modo
long int pulseTime=250;      //En ms
int frecBurst=1000;     //En Hz
Timer tpulse;           //Timer para manipular el pulso 
bool highPulse=true;    //Bandera estado de pulso
int tEvent;             //Indetificador para eveto del timer 

// --------------------------------------------------------------------------------------
void setup() {
  //Señal
  T=1000000/frec;         //Calculo del periodo en us
  d=1023-512*(duty/T);    //Calculo para el ciclo de trabajo de 0 a 1024
  Timer3.initialize(T);   //Inicilizar el timer a periodo f
  Timer3.pwm(pwmPin, d);  //(pin, ciclo *0-1023*, periodo *us*)

  //PANTALLA
  lcd.begin(16, 2);       //Inicializamos la pantalla
  lcd.setCursor(5, 0);
  lcd.print("LAL 04");
  lcd.setCursor(6, 1);
  lcd.print("V1.0");
  delay(3000);
  lcd.clear();

  lcd.setCursor(5, 0);
  lcd.print("PULSOS");
  lcd.setCursor(1, 1);
  lcd.print(frec);
  lcd.print(" Hz");
  lcd.setCursor(9, 1);
  lcd.print(duty);
  lcd.print(" us");
  
}

void loop() {
  //Control de variables 
  char tecla = teclado.getKey();  //Almacena la tecla presionada
   if(tecla){
    switch (tecla){
      case '1':
        if(frec<150){
          frec+=10;
        }
      break;
      case '2':
        if(frec>10){
          frec-=10;
        }
      break;
      case 'A':
        burst=true;
      break;
      case 'B':
        burst=false;
      break;
      }
      Mode();
   }
   tpulse.update();   //Actualizar timer 
}


//---------------------------Cambiar modo de operación------------------------------
void Mode(){
  if(burst){
    T=1000000/frecBurst;         //Calculo del periodo en us
    d=1023-512*(duty/T);        // Calculo ciclo de trabajo
    tEvent=tpulse.every(pulseTime, senalBurst, (void*)1);   //Evento del timer cada 250 ms
    
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("BURST");
    lcd.setCursor(1, 1);
    lcd.print(frecBurst);
    lcd.print(" Hz");
    lcd.setCursor(9, 1);
    lcd.print(duty);
    lcd.print(" us");
    
  }
  else{
    tpulse.stop(tEvent);    //Detiene Evento del timer
    
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("PULSOS");
    lcd.setCursor(1, 1);
    lcd.print(frec);
    lcd.print(" Hz");
    lcd.setCursor(9, 1);
    lcd.print(duty);
    lcd.print(" us");
    
    //Señal de Pulso continuo
    T=1000000/frec;         //Calculo del periodo en us
    d=1023-512*(duty/T);    //Calculo para el ciclo de trabajo de 0 a 1023
    Timer3.pwm(pwmPin, d, T);  //(pin, ciclo *0-1023*, periodo *us*)
  }
}

//------------------------------ Generar señal de pulso burst-------------------------------------
void senalBurst(){
  if(highPulse){
    Timer3.pwm(pwmPin, d, T); // Si el pulso esta activo, desactiva el pwm y visceversa
    highPulse=false;
  }
  else{
    Timer3.disablePwm(pwmPin);
    digitalWrite(pwmPin,LOW);
    highPulse=true;    
  }  
}
