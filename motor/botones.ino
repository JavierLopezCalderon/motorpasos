
void button_reset(){
  if(digitalRead(Botonreset)  == LOW){
    pausecontador= 1;
    delay(500);
  }
  while(pausecontador == 1){
    if(digitalRead(Botonreset)  == LOW){
      pausecontador= 0;
    }
    Serial.println("esperando_reset");
    if(digitalRead(BotonCiclos) == LOW){
      Serial.println("stop");
      estado_reset = 1;
      estado_inicio = 0;
      pausecontador= 0;
    }
  }
  EEPROM.update(7, estado_reset);
  EEPROM.update(8, estado_inicio);
  if(estado_reset == 1){
    digitalWrite(dirPin, HIGH);
    dir_estado = 0;
  }
  
}

void button_start(){
  if(digitalRead(Botonstart) == LOW) {
    estado_inicio = 1;
    estado_reset = 0;
    EEPROM.update(0, estado_reset);
    EEPROM.update(1, estado_reset);  
    EEPROM.update(2, estado_reset);
    EEPROM.update(3, estado_reset);
    EEPROM.update(7, estado_reset);
    EEPROM.update(8, estado_inicio);
    accion[0].byte1 = EEPROM.read(0);
    accion[0].byte2 = EEPROM.read(1);
    accion[1].byte1 = EEPROM.read(2);
    accion[1].byte2 = EEPROM.read(3);
    accion[0].count = accion[0].byte1<<8|accion[0].byte2; //Subidas
    accion[1].count = accion[1].byte1<<8|accion[1].byte2; //Bajadas
    countinicio = 0;
    Serial.print("INICIO");
    lcd.setCursor(0, 0);
    lcd.print("Subidas:");
    lcd.print("          ");
    lcd.setCursor(0, 1);
    lcd.print("Bajadas:");
    lcd.print("          ");
  }
}

void button_ciclos(){
  if(digitalRead(BotonCiclos) == LOW) {
    limiteContador = limiteContador + 1;
    if(limiteContador > 50){
      limiteContador = 1;
    lcd.setCursor(0, 2);
    lcd.print("Ciclos:");
    lcd.print("    ");
    lcd.setCursor(0, 2);
    lcd.print("Ciclos:");
    lcd.print(limiteContador*100);
    }
    EEPROM.update(5, limiteContador); 
    Serial.println(limiteContador);
    lcd.setCursor(0, 2);
    lcd.print("Ciclos:");
    lcd.print(limiteContador*100);
    delay(200);
  }
}

void button_timedelay(){  
  if(digitalRead(BotonVel) == LOW) {
    Serial.println("delay");
    timedelay = EEPROM.read(6);
    timedelay = timedelay+2;
    if(timedelay > 11){
      timedelay = 1;
    lcd.setCursor(0, 3);
    lcd.print("tiempo:");
    lcd.print("  ");
    lcd.setCursor(0, 3);
    lcd.print("tiempo:");
    lcd.print(timedelay);
    
    }
    EEPROM.update(6, timedelay); 
    Serial.println(timedelay);
    lcd.setCursor(0, 3);
    lcd.print("tiempo:");
    lcd.print(timedelay);
    delay(200);
  }
}
