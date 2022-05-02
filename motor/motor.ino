//v1.0
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#define dirPin        2
#define stepPin       3
#define Botonreset    4 
#define fin_carrera1  5
#define fin_carrera2  6 
#define Botonstart    7
#define BotonCiclos   8
#define BotonVel      9
#define Relevador     10

class accesos {
  public:
    int count, limit, valor;
    byte byte1, byte2;
};

int pasos = 1200, timedelay = 2;
int limiteContador = 1;
int fin, dir_estado, estado_reset, cambio_giro, estado_inicio;
int countinicio;
int pausecontador;

accesos accion[2];
LiquidCrystal_I2C lcd(0x27, 20, 4);  //

void setup() {

  Serial.begin(9600);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(Relevador, OUTPUT);
  pinMode(Botonreset, INPUT_PULLUP);
  pinMode(fin_carrera1, INPUT_PULLUP);
  pinMode(fin_carrera2, INPUT_PULLUP);
  pinMode(Botonstart, INPUT_PULLUP);
  pinMode(BotonCiclos, INPUT_PULLUP);
  pinMode(BotonVel, INPUT_PULLUP);
  
  
  lcd.init();
  lcd.backlight();
  lcd.print("INICIANDO...");
  delay(1000);
  lcd.clear();
  accion[0].byte1 = EEPROM.read(0);
  accion[0].byte2 = EEPROM.read(1);
  accion[1].byte1 = EEPROM.read(2);
  accion[1].byte2 = EEPROM.read(3);
  accion[0].count = accion[0].byte1<<8|accion[0].byte2; //Subidas
  accion[1].count = accion[1].byte1<<8|accion[1].byte2; //Bajadas
  dir_estado = EEPROM.read(4);
  limiteContador = EEPROM.read(5);
  timedelay = EEPROM.read(6);
  estado_reset = EEPROM.read(7);
  estado_inicio = EEPROM.read(8);
  lcd_contador();
}

void loop() {
 // lcd_contador();
 // EEPROM.update(6, 1);
 digitalWrite(Relevador, LOW);
  while(( accion[1].count < (limiteContador*100)) && estado_inicio == 1 && estado_reset == 0){ 
    digitalWrite(Relevador, HIGH);
    if(dir_estado == 0){
      digitalWrite(dirPin, LOW);   
    }else{
      digitalWrite(dirPin, HIGH);
    }
    button_timedelay();
    ciclopaso();
    lcd_contador();
    accion[dir_estado].byte1 = accion[dir_estado].count >> 8;
    accion[dir_estado].byte2 = accion[dir_estado].count & 255;
    accion[dir_estado].valor = accion[dir_estado].byte1<<8|accion[dir_estado].byte2;
    
    /*Serial.print("byte 1 :");
    Serial.println(accion[dir_estado].byte1);
    Serial.print("byte 2 :"); 
    Serial.println(accion[dir_estado].byte2);
    Serial.print("valor :"); 
    Serial.println(accion[dir_estado].valor);
    Serial.println(dir_estado);*/

    if(dir_estado == 0){
      EEPROM.update(0, accion[dir_estado].byte1);
      EEPROM.update(1, accion[dir_estado].byte2);  
    }else{
      EEPROM.update(2, accion[dir_estado].byte1);
      EEPROM.update(3, accion[dir_estado].byte2);
    }
    //dir_estado = !dir_estado;
    lcd_contador();
  }
  estado_inicio =0;
  button_start();
  button_ciclos();
  button_reset();
  button_timedelay();
}

void ciclopaso(){
  while(cambio_giro == 0){
    button_timedelay();
    
    digitalWrite(stepPin, HIGH);
    delay(timedelay);
    digitalWrite(stepPin, LOW);
    //delay(1);
    delay(timedelay);
   if(digitalRead(fin_carrera1)==LOW && estado_reset == 0 && dir_estado == 1){
     Serial.println(digitalRead(fin_carrera1));
      if(countinicio == 1){
        accion[dir_estado].count++;
      }      
      dir_estado = 0;
      cambio_giro = 1;  
      delay(200);  
    }else if(digitalRead(fin_carrera2)==LOW&& estado_reset == 0 && dir_estado == 0){
      // EEPROM.update(4, dir_estado);
       accion[dir_estado].count++;
       countinicio = 1;
      dir_estado = 1;
      cambio_giro = 1;
      delay(200);
    }else if(digitalRead(fin_carrera1)==LOW&& estado_reset == 1 ){
      dir_estado = 0;
      cambio_giro = 1;
      digitalWrite(stepPin, LOW);
    }
    button_reset();
    //Serial.println(estado_reset);
    EEPROM.update(4, dir_estado);
   // accion[dir_estado].count++;
  }
  if(estado_reset == 0){
    cambio_giro = 0;
  }
}

void lcd_contador(){
  lcd.setCursor(0, 0);
  lcd.print("Subidas:");
  lcd.print(accion[0].count);
  lcd.setCursor(0, 1);
  lcd.print("Bajadas:");
  lcd.print(accion[1].count);
  lcd.setCursor(0, 2);
  lcd.print("Ciclos:");
  lcd.print(limiteContador*100);
  lcd.setCursor(0, 3);
  lcd.print("tiempo:");
  lcd.print(timedelay);
}
