//LCD LIBRARIES
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//LCD ADRESS
#define endereco 0x27                                  // endereços mais comum 0x27 para o CI Philips PCF8574T
//#define endereco 0x3F                                // endereços mais comum 0x3F para o CI Philips PCF8574AT

//LCD DIMENTIONS
#define colunas 16
#define linhas 2

//LCD INSTANCE
LiquidCrystal_I2C lcd(endereco, colunas, linhas);      // Criando uma estrutura com o nome "lcd" nos parâmetros indicados


//INPUT NAMES
#define POTENTIOMETER_PIN A0

//OUTPUT NAMES
#define red 2
#define green 4
#define PWM_PIN 5
const byte interruptPin = 3;


//GLOBAL VARIABLES
int potentiometer_value;
int tank_percentage;
float motor_dutycycle;
volatile byte state = LOW;

//mestre  
int num = 0;

//SETUP////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  
     //Serial Monitor
     Serial.begin(9600);
     
     //LCD
     lcd.init();
     lcd.backlight();

     //ENTRADAS
     pinMode(interruptPin, INPUT_PULLUP);
     attachInterrupt(digitalPinToInterrupt(interruptPin), stop, LOW);

     //SAÍDAS 
     pinMode(red, OUTPUT);
     pinMode(green, OUTPUT);
     pinMode(PWM_PIN, OUTPUT);
}

//LOOP/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop () {
     
     potentiometer_value = analogRead(POTENTIOMETER_PIN);
     tank_percentage = map(potentiometer_value, 0, 1023, 0 , 100);
     Serial.print(tank_percentage);
     motor_dutycycle = map(potentiometer_value, 0, 1023, 100, 0);         // Leitura do valor do potenciômetro e conversão em porcentagem
     analogWrite(PWM_PIN, motor_dutycycle);
     lcd.setCursor(0,1);
     lcd.print(tank_percentage);
     lcd.print("%      ");
     
     working();  
}

//OTHER_FUNCTIONS//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//STOP FUNCTION
void stop() {
      while(true){
         analogWrite(PWM_PIN, 0);   
         digitalWrite(red, HIGH);
         digitalWrite(green, LOW); 
      }
}
 
//WORKING FUNCTION
void working(){
                                                        // Verificação do nível do tanque
  if (tank_percentage < 90 && tank_percentage > 10) {   // Tanque com nível normal    
      lcd.setCursor(0,0);
      lcd.print("NIVEL OK        ");
      digitalWrite(green, HIGH);
      digitalWrite(red, LOW); 
  } 

  if (tank_percentage < 6) {                            // Tanque em reserva crítica
      lcd.setCursor(0,0);
      lcd.print("RESERVA         ");
      stop();
  }
    
  if (tank_percentage < 11) {                           // Tanque em reserva
      lcd.setCursor(0,0);
      lcd.print("RESERVA         ");
      digitalWrite(green, LOW);
      digitalWrite(red, HIGH);
  }
    
  if (tank_percentage > 89) {                           // Tanque saturado
      lcd.setCursor(0,0);
      lcd.print("SATURADO");
      digitalWrite(green, LOW);
      digitalWrite(red, HIGH);
  }

  if (tank_percentage > 94) {                           // Tanque saturado crítico
      lcd.setCursor(0,0);
      lcd.print("SATURADO");
      stop();
  }
}
