#ifndef _ACTUATOR_H_
#define _ACTUATOR_H_
#include "sensor.h"

#define Brumisateur 2 // D4

class Atomizer{
  public:
  // ***** Constructeur *****
  Atomizer(){}
  // Destructeur
  ~Atomizer(){};
  // ***** Méthodes *****
  void initBrumisateur(){
      pinMode(Brumisateur,OUTPUT);
      digitalWrite(Brumisateur,LOW);
  }
  // Régulation humidité tout ou rien
  void regulationHum(float hum_actuelle, float humVoulue){
    if(hum_actuelle<humVoulue) {
      Serial.println("Haut");
      digitalWrite(Brumisateur,HIGH);
    } else {
      Serial.println("Bas");
      digitalWrite(Brumisateur,LOW);
    }
  }
  
};
#endif 
