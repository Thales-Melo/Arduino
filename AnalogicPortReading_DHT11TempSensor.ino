/*
TEMPERATURE SENSOR

  Vamos usar o "serial monitor" para exibir a temperatura lida a partir do sensor. 
  O ThinkerCad oferece o sensor TMP36, um sensor de temperatura cuja a voltagem da saída  é proporcional a tempertatura ambiente.
   
CONEXOES

  5V, SIGNAL, and GND.
  Conectar o pino 5V em 5V no Arduino.
  Conectar o pino SIGNAL (Vout) no pino A0 (Analogico 0) no Arduino.
  Conectar o pino GND em GND no Arduino.
*/

#include "DHT.h"

#define RED_PIN 8
#define YELLOW_PIN 9
#define GREEN_PIN 10
#define BLUE_PIN 11
#define DHTPIN A1 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11

DHT dht(DHTPIN, DHTTYPE);

void setup(){
  pinMode(RED_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  dht.begin();
  
  Serial.begin(9600);
}
 
void serCor(int r, int y, int g, int b){
  digitalWrite(BLUE_PIN, b);
  digitalWrite(GREEN_PIN, g);
  digitalWrite(YELLOW_PIN, y);
  digitalWrite(RED_PIN, r);
}

void loop() {
  
  float h = dht.readHumidity();
  float celsius = dht.readTemperature();
  
  Serial.print("\nTemperatura = ");
  Serial.print(celsius);
  // Serial.print("\nUmidade = ");
  // Serial.print(h);
  
  // testa se retorno é valido, caso contrário algo está errado.
  if (isnan(celsius) || isnan(h)) {
    Serial.println("\nFailed to read from DHT");
  } 


  else {

    // AZUL
    if(celsius < 10){
          serCor(LOW, LOW, LOW, HIGH);
    }
    
    // VERDE
    else if(celsius >= 10 && celsius < 20){
          serCor(LOW, LOW, HIGH, LOW);
    }

    // AMARELO
    else if(celsius >= 20 && celsius < 30){
          serCor(LOW, HIGH, LOW, LOW);
    }

    // VERMELHO
    else{
          serCor(HIGH, LOW, LOW, LOW);
    }
  }
   
}
