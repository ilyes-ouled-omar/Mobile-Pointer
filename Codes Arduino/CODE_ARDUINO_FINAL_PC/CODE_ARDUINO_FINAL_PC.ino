//AT commnands //NB: Activer "les deux NL et CR" en bas dans le moniteur série //Arduino Mega

#define gsm Serial2
/* la hardware serial est beaucoup plus mieux au niveau de l'affichage*/
//#include <SoftwareSerial.h>
//SoftwareSerial gsm(50,51); // RX, TX  

void updateSerial() {
 if (Serial.available()) {
   byte b=Serial.read();
  if (b=='*')   // En cas de Send SMS, on termine notre texte par *
   gsm.write(0x1a);   //parce que CTRL Z ne marche pas dans le moniteur série d'Arduino 
  else 
    gsm.write(b); //ce qui va être reçu par Serial, va être écrit dans le module gsm
 }
 if(gsm.available()) {
 Serial.write(gsm.read()); //ce qui va être reçu par module gsm, va être écrit sur Serial
}
}

void setup() {
Serial.begin(115200);
//Serial.println("Hello ! ");
gsm.begin(115200);
delay(2000);  // ce delay est important car le A6 prend 2sec pour boot
gsm.println("AT\r"); 
delay(500);
gsm.println("AT\r"); 
delay(500);
}

void loop() { 
 updateSerial();
}
