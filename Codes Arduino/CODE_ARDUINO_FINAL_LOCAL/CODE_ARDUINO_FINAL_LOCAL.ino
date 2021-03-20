#include <Adafruit_Fingerprint.h>
#define mySerial Serial3 //capteur ADA
#define gsm Serial2 //module gsm
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,16,2); //16colonnes et 2 lignes

int pinBtn1=22; //bouton défilement
int pinBtn2=24; //bouton ok
int menu=0;

void setup() {
lcd.init(); // initialisation de l'afficheur
pinMode(pinBtn1,INPUT);
pinMode(pinBtn2,INPUT);
gsm.begin(115200); // initialisation du module GSM
delay(1000);
finger.begin(57600); // initialisation du capteur d'empreintes
}

int getFingerprintIDez() {  //dans code fingerprint
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ID trouve!");
  lcd.setCursor(0,1);
  lcd.print(finger.fingerID);
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("avec certitude");
  lcd.setCursor(0,1);
  lcd.print("de ");
  lcd.setCursor(5,1);
  lcd.print(finger.confidence);
  delay(2000);
  return finger.fingerID; 
}

uint8_t getFingerprintID() { //dans code fingerprint //on ne l'a pas utilisé car la précédente est plus rapide
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Image prise");
      break;
    case FINGERPRINT_NOFINGER:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Pas de doigt!");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Erreur de");
      lcd.setCursor(0, 1);
      lcd.print("communication");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Erreur d'image");
      return p;
    default:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error inconnue");
      return p;
  }
  // OK success!
  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Image convertie");
      break;
    case FINGERPRINT_IMAGEMESS:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Image floue");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Erreur de");
      lcd.setCursor(0, 1);
      lcd.print("communication");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Impossible de trouver");
      lcd.setCursor(0, 1);
      lcd.print("des empreintes");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Impossible de trouver");
      lcd.setCursor(0, 1);
      lcd.print("des empreintes");
      return p;
    default:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Erreur inconnue");
      return p;
  }
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enpreinte");
      lcd.setCursor(0, 1);
      lcd.print("trouvee !");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Erreur de");
      lcd.setCursor(0, 1);
      lcd.print("communication");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Empreinte");
      lcd.setCursor(0, 1);
      lcd.print("non trouvee ");
    return p;
  } else {
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Erreur inconnue");
    return p;
  }   
  // found a match!
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ID trouve");
  lcd.setCursor(0,1);
  lcd.print(finger.fingerID);
  delay(1000);
  lcd.clear();
  lcd.print("avec certitude");
  lcd.setCursor(0,1);
  lcd.print("de ");
  lcd.setCursor(5,1);
  lcd.print(finger.confidence);
  delay(1000);
  return finger.fingerID; 
}
// returns -1 if failed, otherwise returns ID #

uint8_t readnumber(void) //pour code enroll et delete //lit le nombre saisi à l'aide des 2 boutons défilement et ok 
{ 
  boolean etatBtn1 = digitalRead(pinBtn1);
  boolean etatBtn2 = digitalRead(pinBtn2);
  boolean pressb20=false, pressb21=false, pressb22=false;
  
  uint8_t num = 0;
  uint8_t u=0, d=0, c=0, u1=0, d1=0, c1=0, p=0;
  while (num == 0)
  {
    boolean etatBtn1 = digitalRead(pinBtn1);
    boolean etatBtn2 = digitalRead(pinBtn2);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("ID #");
      lcd.setCursor(5,1);
      lcd.print(c);
      lcd.setCursor(7,1);
      lcd.print(d);
      lcd.setCursor(9,1);
      lcd.print(u);
      delay(1000);

      while(p==0)
    { 
      boolean etatBtn1 = digitalRead(pinBtn1);
      //boolean etatBtn2 = digitalRead(pinBtn2);
      while(pressb20==false)
      {
      boolean etatBtn1 = digitalRead(pinBtn1);
      boolean etatBtn2 = digitalRead(pinBtn2);
      if (etatBtn2==HIGH) { pressb20=true;}
      if (etatBtn1==HIGH)
       { 
        u1++;
        if (u1==10) 
        { u1=0; 
         delay(500);
        }
        lcd.setCursor(9,1);
        lcd.print(u1);
        delay(500);
      //boolean etatBtn1 = digitalRead(pinBtn1);
      boolean etatBtn2 = digitalRead(pinBtn2);
      if (etatBtn2==HIGH) { pressb20=true;}
      delay(500);
       }
      }
          delay(500);
          u=u1;
          p++; 
          pressb20=false;
          break;
    }
    delay(500);
    while(p==1)
    { 
      boolean etatBtn1 = digitalRead(pinBtn1);
      //boolean etatBtn2 = digitalRead(pinBtn2);
      
      while(pressb21==false)
      {
      boolean etatBtn1 = digitalRead(pinBtn1);
      boolean etatBtn2 = digitalRead(pinBtn2);
      if (etatBtn2==HIGH) { pressb21=true;}
      if (etatBtn1==HIGH)
       { 
        d1++;
        if (d1==10) 
        { d1=0; 
         delay(500);
        }
        lcd.setCursor(7,1);
        lcd.print(d1);
        delay(500);
      boolean etatBtn2 = digitalRead(pinBtn2);
      if (etatBtn2==HIGH) { pressb21=true;}
      delay(500);   
         }
       }
       delay(500);
       d=d1;
       p++; 
       pressb21=false;
       break;
    }
    delay(500);
    while(p==2)
    { 
      boolean etatBtn1 = digitalRead(pinBtn1);
      //boolean etatBtn2 = digitalRead(pinBtn2);
      
      while(pressb22==false)
      {
      boolean etatBtn1 = digitalRead(pinBtn1);
      boolean etatBtn2 = digitalRead(pinBtn2);
      if (etatBtn2==HIGH) { pressb22=true;}
      if (etatBtn1==HIGH)
       { 
        c1++;
        if (c1==2) 
        { c1=0; 
         delay(500);
        }
        lcd.setCursor(5,1);
        lcd.print(c1);
        delay(500);
      boolean etatBtn2 = digitalRead(pinBtn2);
      if (etatBtn2==HIGH) { pressb22=true;}
      delay(500);   
         }
       }
       delay(500);
       c=c1;
       p++; 
       pressb22=false;
       break;
    }
  delay(1000);   
  num=u+(d*10)+(c*100);
    }
  delay(1000);
  p=0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ID saisi est#");
  lcd.setCursor(0,1);
  lcd.print(num);
  delay(2000);
  return num;
}

uint8_t getFingerprintEnroll() { //dans code enroll

  int p = -1;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Votre doigt");
  lcd.setCursor(0, 1);
  lcd.print("SVP...");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(id);
  delay(2000); 
  while (p != FINGERPRINT_OK) {
    
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Image prise");
      break;
    case FINGERPRINT_NOFINGER:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Pas de doigt!");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Erreur de");
      lcd.setCursor(0, 1);
      lcd.print("communication");
      break;
    case FINGERPRINT_IMAGEFAIL:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Erreur d'image");
      break;
    default:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Erreur inconnue");
      break;
    }
  }
  delay(2000);
  // OK success!
  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Image convertie");
      break;
    case FINGERPRINT_IMAGEMESS:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Image floue");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Erreur de");
      lcd.setCursor(0, 1);
      lcd.print("communication");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Impossible de trouver");
      lcd.setCursor(0, 1);
      lcd.print("des empreintes");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Impossible de trouver");
      lcd.setCursor(0, 1);
      lcd.print("des empreintes");
      return p;
    default:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Erreur inconnue");
      return p;
  }
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Retirez votre");
  lcd.setCursor(0, 1);
  lcd.print("doigt SVP");
  delay(2000); 
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ID ");
  lcd.setCursor(0, 1);
  lcd.print(id);
  delay(2000);
  p = -1;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Votre doigt une");
  lcd.setCursor(0, 1);
  lcd.print("2eme fois SVP");
  delay(2000); 
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Image prise");
      break;
    case FINGERPRINT_NOFINGER:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Pas de doigt!");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Erreur de");
      lcd.setCursor(0, 1);
      lcd.print("communication");
      break;
    case FINGERPRINT_IMAGEFAIL:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Erreur d'images");
      break;
    default:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Erreur inconnue");
      break;
    }
  }
  delay(2000);
  // OK success!
  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Image convertie");
      break;
    case FINGERPRINT_IMAGEMESS:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Image floue");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Erreur de");
      lcd.setCursor(0, 1);
      lcd.print("communication");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Impossible de trouver");
      lcd.setCursor(0, 1);
      lcd.print("des empreintes");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Impossible de trouver");
      lcd.setCursor(0, 1);
      lcd.print("des empreintes");
      return p;
    default:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Erreur inconnue");
      return p;
  }
  delay(2000);
  // OK converted!
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Model en cours");
  lcd.setCursor(0, 1);
  lcd.print("de creation");
  delay(2000);
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Empreinte");
    lcd.setCursor(0, 1);
    lcd.print("validee!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Erreur de");
    lcd.setCursor(0, 1);
    lcd.print("commincation");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Empreinte");
    lcd.setCursor(0, 1);
    lcd.print("non validee!");
    return p;
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Erreur inconnue");
    return p;
  }   
  delay(2000);
    lcd.clear();
    lcd.setCursor(7, 0);
    lcd.print("ID "); 
    lcd.setCursor(7, 1);
    lcd.print(id); 
    delay(2000);
  p = finger.storeModel(id);
  delay(2000);
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enregistre");
    lcd.setCursor(0, 1);
    lcd.print("avec succes");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Erreur de");
    lcd.setCursor(0, 1);
    lcd.print("communication");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Emplacement");
    lcd.setCursor(0, 1);
    lcd.print("non disponible");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Erreur de flash");
    return p;
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Erreur inconnue");
    return p;
  }   
  delay(2000);
}

uint8_t deleteFingerprint(uint8_t id) { //dans code delete
  uint8_t p = -1;
  
  p = finger.deleteModel(id); //ajouté par  
  delay(2000);

  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Supprime!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Erreur de");
    lcd.setCursor(0, 1);
    lcd.print("communication");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Emplacement");
    lcd.setCursor(0, 1);
    lcd.print("non disponible");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Erreur de flash");
    return p;
  } else {
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Erreur inconnue"); 
      lcd.setCursor(0, 1);
      lcd.print("0x");
      lcd.setCursor(5, 1);
      lcd.print(p, HEX);
    return p;
  }   
}

void loop() {
 lcd.backlight();  //lumière du lcd
boolean etatBtn1 = digitalRead(pinBtn1); //bouton défilemnt
boolean etatBtn2 = digitalRead(pinBtn2); //bouton ok
if (etatBtn1==HIGH)
{
  menu++;
  delay(200);
}
switch (menu)
{
  case 0:
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Veuillez");
  lcd.setCursor(0,1);
  lcd.print("defiler le menu");
  delay(200);
  break;
   
  case 1:
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Envoyer ID ?");
   delay(200);
   if (etatBtn2==HIGH)
   {
    lcd.clear();
    lcd.setCursor(7, 0);
    lcd.print("OK");
    delay(1000);
    //code fingerprint
     finger.getTemplateCount();
     lcd.clear();
     lcd.setCursor(0, 0);
     lcd.print("Capteur contient");
     lcd.setCursor(0, 1);
     lcd.print(finger.templateCount); lcd.print(" empreintes");
     delay(2000);
     lcd.clear();
     lcd.setCursor(0, 0);
     lcd.print("Votre doigt");
     lcd.setCursor(0, 1);
     lcd.print("SVP ..."); 
     delay(1000);
    //getFingerprintIDez();
    //delay(3000);
    int ID = getFingerprintIDez();
    delay(1000);
   if (ID !=-1)
   {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("ID =");
    lcd.setCursor(6, 0);
    lcd.print(ID);
    delay(1000);
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("ID va etre");
    lcd.setCursor(3, 1);
    lcd.print("envoye");
    
    gsm.print("AT\r");
    delay(500);
    gsm.print("AT+CMGF=1;\r"); // mettre GSM dans le mode text
    delay(500);
    gsm.print("AT+CMGS=\"54650474\";\r");
    delay(500);
    gsm.print(ID); //le texte qui va être envoyé
    gsm.write(0x1a); //CTRL+Z
    delay(6000); //le temps nécessaire pour que le message soit reçu dans le téléphonne
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ID envoye! Merci");
    lcd.setCursor(0, 1);
    lcd.print("pour l'attente!");
    delay(2000);
    }
    //fin code fingerprint
    break;
   }
   break;
   
  case 2:
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Ajouter");
    lcd.setCursor(2, 1);
    lcd.print("utilisateur ?");
   delay(200);
   if (etatBtn2==HIGH)
   {
    lcd.clear();
    lcd.setCursor(7, 0);
    lcd.print("OK");
    delay(2000);
    
    //code enroll
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Pret pour nouvel");
    lcd.setCursor(0, 1);
    lcd.print("le empreinte");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Veuillez entrer");
    lcd.setCursor(0, 1);
    lcd.print("l'ID souhaite");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("(De 1 a 162) ...");
    delay(2000);

    id = readnumber();

    if (id == 0) {// ID #0 not allowed, try again!
     return;
    }  
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ID pret!");
    lcd.setCursor(0, 1);
    lcd.print(id);
    delay(2000);
    
    //while (!  getFingerprintEnroll() );
      getFingerprintEnroll();
        //fin code enroll
   break;
   }
   break;
   
  case 3:
   lcd.clear();
   lcd.setCursor(3, 0);
   lcd.print("Supprimer");
   lcd.setCursor(2, 1);
   lcd.print("utilisateur ?");
   delay(200);
   if (etatBtn2==HIGH)
   {
   lcd.clear();
   lcd.setCursor(7, 0);
   lcd.print("OK");
   delay(2000);
    
    //code delete
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SVP entrez l'ID");
    lcd.setCursor(0, 1);
    lcd.print("a supprimer");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("(de 1 a 162) ...");
    delay(2000);
    uint8_t id = readnumber();
    if (id == 0) 
    {// ID #0 not allowed, try again!
     return;
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ID en cours de");
    lcd.setCursor(0, 1);
    lcd.print("suppression ");
    lcd.setCursor(13, 1);
    lcd.print(id);
    delay(2000);
    deleteFingerprint(id);
    delay(2000);
    //fin code delete
    break;
   }
   break;
   
}
if(menu==4)
{
  menu=0;
  delay(200);
}
}



