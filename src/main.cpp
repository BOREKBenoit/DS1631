/* 
  Capteur de température DS1631 en I2C
  
  Programme basique sur Arduino Mega
  IDE Arduino
  
  Constituants :
  -  Capteur de température DS1631

  Version 1 : 19/11/2024

  Benoit BOREK
  */

#include <Arduino.h>
#include <Wire.h> // Inclus la librairie Wire pour intéragir avec le capteur
#define Addr 0x90 >> 1 // Définit une variable Addr sur 0x90, qui est l'adresse du capteur.

void setup() {

 delay(3000); // J'ai volontairement mis une pause de 3 secondes au début du code pour éviter de possibles bug

 Serial.begin(9600); // Initialisation Arduino <-----> PC sur 9600 bauds.

 Serial.println("//////////////////////////////"); // Texte avec une petite décoration
 Serial.println();
 Serial.println("DS1680 CAPTEUR DE TEMPERATURE");
 Serial.println();


 delay(1000); // La pause d'une seconde est facultative et peut-être supprimé.


 Wire.begin(); // L'arduino ce met en mode maitre 
 Wire.beginTransmission(Addr); // L'arduino définit l'adresse 0x90 en tant qu'esclave et initialise une communication.
 Wire.write(0x22); // L'arduino va envoyer 0x22 au capteur de température qui va arrêter une possible conversion du capteur de température en cours.
 int error = Wire.endTransmission(); // Cette fonction arrête la transmission de l'arduino avec le capteur et permet de trouver un possible code erreur et mettre la valeur de ce code dans une variable "error".
 // Cette fonction renvoie 0 si il n'y a aucune erreur.
 if(error != 0){ // Le if vérifie si il y a un code erreur, si il n'y en a pas alors la configuration du capteur continuera.

  Serial.println("Oups... Une erreur est survenue..."); // Si jamais il y a une erreur, on va afficher dans le moniteur série le code d'erreur.
  Serial.println();
  Serial.print("Code erreur ");
  Serial.println(error); // Affichage du code erreur dans le moniteur série.
  } else {
  Wire.beginTransmission(Addr); // On recommence une initialisation avec le capteur.
  Wire.write(0xAC); // L'arduino envoi 0xAC qui permet de lire le registre de configuration.
  Wire.endTransmission(); // On arrête la transmission avec le capteur.

  Wire.requestFrom(Addr, 1); // Va envoyer une requête au capteur pour pouvoir lire le registre de configuration sur un bit.
  Wire.available(); // Test si le bit du registre de configuration est diponnible.
  int ancienne_config = Wire.read(); // Lis le bit du registre de configuration et le place dans "ancienne_config".

  Serial.println("Ancienne config en 0x : "); 
  Serial.println(ancienne_config, HEX); // Affiche le bit du registre de configuration en héxadécimal.
  Serial.println();
  Serial.println("//////////////////////////////");
  delay(1000); // Pause d'une seconde.

  Wire.beginTransmission(Addr);
  Wire.write(0xAC); // L'arduino envoi 0xAC qui permet d'écrire dans le registre de configuration.
  Wire.write(0x0C); // L'arduino envoie 0x0C qui permet mettre la configuration que je veux.
  Wire.endTransmission();

  Wire.requestFrom(Addr, 1); // Va envoyer une requête au capteur pour pouvoir lire le registre de configuration sur un bit.
  Wire.available(); // Test si le bit du registre de configuration est diponnible.
  int nouvelle_config = Wire.read(); // Lis le bit du registre de configuration et le place dans "nouvelle_config".

  Serial.println("Nouvelle config en 0x : ");
  Serial.println(nouvelle_config, HEX); // Affiche le bit du nouveau registre de configuration en héxadécimal.
  Serial.println();
  Serial.println("//////////////////////////////");
  delay(1000);



  Wire.beginTransmission(Addr);
  Wire.write(0xA2); // Accède à TL pour écrire sa nouvelle valeur sur 2 Bits.
  Wire.write(0x1A);
  Wire.write(0x00);
  Wire.endTransmission();




 Wire.requestFrom(Addr, 1);
  Wire.available();
  int tl = Wire.read();

  Serial.println("TL 0x : ");
  Serial.println(tl, HEX); // Affiche la valeur de TL en héxadécimal.
  Serial.println();
  Serial.println("//////////////////////////////");
  delay(1000);


  Wire.beginTransmission(Addr);
  Wire.write(0xA1); // Accède à TH pour écrire sa nouvelle valeur sur 2 Bits.
  Wire.write(0x1B);
  Wire.write(0x00);
  Wire.endTransmission();

   Wire.requestFrom(Addr, 1);
  Wire.available();
  int th = Wire.read();

  Serial.println("TH 0x : ");
  Serial.println(th, HEX); // Affiche la valeur de TH en héxadécimal.
  Serial.println();
  Serial.println("//////////////////////////////");
  delay(1000);



  Wire.beginTransmission(Addr);
  Wire.write(0x51); // On demande au capteur de commencer à convertir la température.
  Wire.endTransmission();
  
  }
}

void loop() {
  Serial.println("--------------------------------");
  Wire.beginTransmission(Addr);
  Wire.write(0xAA); // On demande au capteur de lire la température.
  Wire.endTransmission();

  Wire.requestFrom(Addr, 2);  // On envoi une requête pour lire la température sur 2 Bits.
  Serial.print("octets en attente de lecture: ");
  Serial.println(Wire.available());
  int T_MSB = Wire.read(); // Le premier Bit sera enregistrer dans "T_MSB".
  int T_LSB = Wire.read(); // Le deuxième Bit sera enregistrer dans "T_LSB".

  Serial.println("=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!="); // Affiche la valeur binaire des deux bits de température .
  Serial.println("Valeurs en binaires : ");
  Serial.print("T_MSB : ");
  Serial.println(T_MSB, BIN); 
  Serial.print("T_LSB : ");
  Serial.println(T_LSB, BIN);


// Partie du code qui sert à convertir la température du binaire vers les décimales.
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

  Serial.print((T_MSB & 0b01111111) + partie_decimale, 4); // Affichage de la température en décimale.
  Serial.println();

  delay(2500); // Pause de 2 secondes et demi.
}
  
