#include <Wire.h>
#define Addr 0x90 >> 1

void setup() {
 delay(3000);
 Serial.begin(9600);
 Serial.println("//////////////////////////////");
 Serial.println();
 Serial.println("DS1680 CAPTEUR DE TEMPERATURE");
 Serial.println();
 delay(1000);
 Wire.begin();
 Wire.beginTransmission(Addr);
 Wire.write(0x22);
 int error = Wire.endTransmission();
 if(error != 0){
  Serial.println("Oups... Une erreur est survenue...");
  Serial.println();
  Serial.print("Code erreur ");
  Serial.println(error);
  } else {
  Wire.beginTransmission(Addr);
  Wire.write(0xAC);
  Wire.endTransmission();

  Wire.requestFrom(Addr, 1);
  Wire.available();
  int ancienne_config = Wire.read();

  Serial.println("Ancienne config en 0x : ");
  Serial.println(ancienne_config, HEX);
  Serial.println();
  Serial.println("//////////////////////////////");
  delay(1000);

  Wire.beginTransmission(Addr);
  Wire.write(0xAC);
  Wire.write(0x0C);
  Wire.endTransmission();

  Wire.requestFrom(Addr, 1);
  Wire.available();
  int nouvelle_config = Wire.read();

  Serial.println("Nouvelle config en 0x : ");
  Serial.println(nouvelle_config, HEX);
  Serial.println();
  Serial.println("//////////////////////////////");
  delay(1000);



  Wire.beginTransmission(Addr);
  Wire.write(0xA2); // Accès à TL
  Wire.write(0x1A);
  Wire.write(0x00);
  Wire.endTransmission();




 Wire.requestFrom(Addr, 1);
  Wire.available();
  int tl = Wire.read();

  Serial.println("TL 0x : ");
  Serial.println(tl, HEX);
  Serial.println();
  Serial.println("//////////////////////////////");
  delay(1000);


  Wire.beginTransmission(Addr);
  Wire.write(0xA1); // Accès à TH
  Wire.write(0x1B);
  Wire.write(0x00);
  Wire.endTransmission();

   Wire.requestFrom(Addr, 1);
  Wire.available();
  int th = Wire.read();

  Serial.println("TH 0x : ");
  Serial.println(th, HEX);
  Serial.println();
  Serial.println("//////////////////////////////");
  delay(1000);



  Wire.beginTransmission(Addr);
  Wire.write(0x51);
  Wire.endTransmission();
  
  }
}

void loop() {
  Serial.println("--------------------------------");
  Wire.beginTransmission(Addr);
  Wire.write(0xAA);
  Wire.endTransmission();

  Wire.requestFrom(Addr, 2);
  Serial.print("octets en attente de lecture: ");
  Serial.println(Wire.available());
  int T_MSB = Wire.read();
  int T_LSB = Wire.read();

  Serial.println("=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=");
  Serial.println("Valeurs en binaires : ");
  Serial.print("T_MSB : ");
  Serial.println(T_MSB, BIN);
  Serial.print("T_LSB : ");
  Serial.println(T_LSB, BIN);

  int partie_entiere = T_MSB & 0b01111111;

  float partie_decimale = 0.0;

  if((T_LSB & 0b10000000) == 0b10000000){
    partie_decimale = partie_decimale + 0.5;
    }
  if((T_LSB & 0b01000000) == 0b01000000){
    partie_decimale = partie_decimale + 0.25;
    }
  if((T_LSB & 0b00100000) == 0b00100000){
    partie_decimale = partie_decimale + 0.125;
    }
  if((T_LSB & 0b00010000) == 0b00010000){
    partie_decimale = partie_decimale + 0.0625;
    }

  Serial.print("Valeur décimale correspondante : T_dec = ");
  if(T_MSB >= 0x80) {
    Serial.print("-");
    }

  Serial.print((T_MSB & 0b01111111) + partie_decimale, 4);
  Serial.println();

  delay(2500);
}
  
