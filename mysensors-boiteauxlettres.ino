#define MY_RF24_PA_LEVEL RF24_PA_LOW
#define MY_RADIO_NRF24

#include <SPI.h>
#include <MySensors.h>

#define CHILD_ID 0

// 1er couple.
#define PHOTODIODE_1 7
#define LEDIR_1 A5

// 2ème couple.
#define PHOTODIODE_2 6
#define LEDIR_2 A4

// 3ème couple.
#define PHOTODIODE_3 5
#define LEDIR_3 A3

// 4ème couple.
#define PHOTODIODE_4 4
#define LEDIR_4 A2

// 5ème couple.
#define PHOTODIODE_5 3
#define LEDIR_5 A1

// 6ème couple.
#define PHOTODIODE_6 2
#define LEDIR_6 A0

#define CAPTEUR A6
#define SEUIL_PRESENCE_COURRIER 60

unsigned long SLEEP_TIME = 600000;

MyMessage msg(CHILD_ID, V_STATUS);

void before() {
  pinMode(PHOTODIODE_1, INPUT);
  pinMode(LEDIR_1, OUTPUT);
  
  pinMode(PHOTODIODE_2, INPUT);
  pinMode(LEDIR_2, OUTPUT);
  
  pinMode(PHOTODIODE_3, INPUT);
  pinMode(LEDIR_3, OUTPUT);
  
  pinMode(PHOTODIODE_4, INPUT);
  pinMode(LEDIR_4, OUTPUT);
  
  pinMode(PHOTODIODE_5, INPUT);
  pinMode(LEDIR_5, OUTPUT);
  
  pinMode(PHOTODIODE_6, INPUT);
  pinMode(LEDIR_6, OUTPUT);
}

void presentation()  {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Capteur boite aux lettres", "1.0");

  // Register all sensors to gateway (they will be created as child devices)
  present(CHILD_ID, S_BINARY);
}

void loop()      
{
  bool courrierPresent = Mesurer(PHOTODIODE_1, LEDIR_1);

  if (!courrierPresent)
  {
    courrierPresent = Mesurer(PHOTODIODE_2, LEDIR_2);
  }
  
  if (!courrierPresent)
  {
    courrierPresent = Mesurer(PHOTODIODE_3, LEDIR_3);
  }
  
  if (!courrierPresent)
  {
    courrierPresent = Mesurer(PHOTODIODE_4, LEDIR_4);
  }
  
  if (!courrierPresent)
  {
    courrierPresent = Mesurer(PHOTODIODE_5, LEDIR_5);
  }
  
  if (!courrierPresent)
  {
    courrierPresent = Mesurer(PHOTODIODE_6, LEDIR_6);
  }
  
  // Envoie de la valeur à la gateway.
  send(msg.set(courrierPresent ? 1 : 0));
  
  // Mise en veille du traitement.
  sleep(SLEEP_TIME);
}

// Effectue une mesure d'un couple de capteur.
// Renvoi :
//  - Vrai si du courrier est détecté pour ce capteur.
//  - Faux sinon.
bool Mesurer(uint8_t photodiode, uint8_t ledIr)
{
  // Alimentation de la photodiode.
  pinMode(photodiode, OUTPUT);
  digitalWrite(photodiode, HIGH);
  
  // Alimentation de la led IR.
  digitalWrite(ledIr, HIGH);

  // Lecture de la valeur.
  int mesure = analogRead(CAPTEUR);

  // Arrêt de la led IR.
  digitalWrite(ledIr, LOW);
  
  // Arrêt de la photodiode.
  digitalWrite(photodiode, LOW);
  pinMode(photodiode, INPUT);

  sleep(20);

  return mesure < SEUIL_PRESENCE_COURRIER;
}
