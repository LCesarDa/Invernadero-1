#include <LiquidCrystal.h>
#include <Servo.h>
#include <Stepper.h>
#include <DHT.h> // Incluye la librería del sensor DHT11

Servo Servomotor;

// Definir pines

//-*****Potenciómetro*****
const int pot = A2;

//***********************

//-*****Ventilador*****
const int ventilador = 13;
//***********************

//-*****Fotoresistencia*****
int fotoresistencia = 0;
const int ftr = A0;
double luz;
int fotoresistenciaCalibracion = -2;  // Ajuste para la fotoresistencia
//***********************

//-*****Temperatura*****-
const int Temperatura= A1;

//Sensor LM35 
float lm35Calibracion = -15;  // Ajuste para el sensor LM35 (en grados Celsius)

//***********************

//-*****Polea*****-
const int stepsPerRevolution = 2038;
Stepper myStepper = Stepper(stepsPerRevolution, 38, 40, 39, 41);
const int vueltas = 6;//Cuantas vueltas necesita???
bool abierto;
//***********************

//-*****Sensor de humedad y temperatura*****-

#define DHT11_PIN 52; //PIN DONDE SE CONECTA EL DHT11 
#define DHTTYPE DHT11 //Tipo de sensor DHT que se esta utilizando

DHT dht (DHTPIN, DHTTYPE); 

//***********************

//-*****Bomba de agua*****

//***********************

// Declaración de objetos
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Servo myservo;
const int inPin=0;

void setup() {
  // Inicializar objetos
  lcd.begin(16, 2);
  Serial.begin(9600);
  pinMode(ventilador, OUTPUT);
  Servomotor.attach(9);
  //pinMode(relevador1, OUTPUT);
  //pinMode(relevador2, OUTPUT);  
  DHT.begin();
}


void loop() {
  // Leer valores del potenciómetro, sensor de temperatura y fotresistencia
  
  //Potenciometro----------
  
  int potenciometro  = analogRead(pot);
  float voltaje = potenciometro * (5.0 / 1023.0);
  Serial.print("voltaje:   \n" );
  //-----------------------
  
  //Calculo De Temperatura A Centigrados

  //____ metodo 1____//
  //int value = analogRead(A1);
  //float sensorTemperatura = analogRead(A1) * 0.48875; // Convertir valor a grados Celsius
  //Serial.print(sensorTemperatura);
  //Serial.println(" C");

  //___ método 2____//
  /*int value = analogRead(A1);
  float millivolts = (value / 1023.0) * 5000;
  float celsius = (millivolts / 10)-12.5; 
  Serial.print(celsius);
  Serial.println(" C");
  delay(1000);*/

  //__ método 3 ___//

  int lm35Value = analogRead(Temperatura);
  float millivolts = (lm35Value / 1023.0) * 5000;
  float celsius = (millivolts / 10) + lm35Calibracion;  // Aplicar la calibración
  Serial.print("Temperatura:  C");
  Serial.print(celsius);
  //--------

  //Fotoresistencia-----------  
  int fotoresistenciaValue = analogRead(ftr);
  luz = (double)(fotoresistenciaValue /9.74)-fotoresistenciaCalibracion; //doble calibracion
  Serial.print(" \n Valor de la fotoresistencia: ");
  Serial.print(luz);
  //----

// Controlar servo en base a la temperatura
  if (celsius > 34) {
    Servomotor.write(90); // Activar servo
    digitalWrite (ventilador,HIGH);
    Serial.println("El ventilador se activo: ");
    Serial.print(ventilador);
    if (!abierto){
      myStepper.setSpeed(vueltas);
      myStepper.step(stepsPerRevolution);
    }
    abierto = true;
    lcd.setCursor(11,1);
    lcd.print("Open ");
  } else {
    myservo.write(0); // Desactivar servo
    digitalWrite (ventilador, LOW);
    Serial.println("El ventilador se desactivo: ");
    Serial.print(ventilador);
     if (abierto){
      myStepper.setSpeed(vueltas);
      myStepper.step(-stepsPerRevolution);
    }
    abierto = false;
    lcd.setCursor(11,1);
    lcd.print("Close");
  }

//Sensor Humedad y temperatura------------
int chk = DHT.read(DHT11_PIN);
//***********
  
// Actualizar pantalla
  lcd.setCursor(0, 0);
  lcd.print("TempLM35: ");
  lcd.print(celsius);
  lcd.print(" C ");
  
  lcd.setCursor(0, 1);
  lcd.print("Luz: ");
  lcd.print(luz);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(DHT.temperature);
  lcd.print(" C");
  lcd.setCursor(0,1);
  lcd.print("Hum: ");
  lcd.print(DHT.humidity);
  lcd.print(" %");
   delay(2000);
   
  // Esperar un breve periodo de tiempo antes de repetir el ciclo
  delay(1000);
  
  Serial.print(analogRead(A0));
  Serial.print("\n");
  delay(800);
}

 
