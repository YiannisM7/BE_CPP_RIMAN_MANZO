#ifndef _SENSOR_H_
#define _SENSOR_H_
#include <Arduino.h>

#include <Wire.h>
#include "SHT31.h"

// Classe fille de SHT31 pour la température 
class TempSensor:public SHT31{
  public:
    // Constructeur
    TempSensor():SHT31(){}
    // Méthode
    void afficherTemperature(){
      Serial.print("Temperature actuelle : "); 
      Serial.print(this->getTemperature()); 
      Serial.println(" C"); 
    }
};

// Classe fille de SHT31 pour l'humidité
class HumSensor:public SHT31{
  public:
  // Constructeur
  HumSensor():SHT31(){}
  // Méthode
  void afficherHumidite(){
    Serial.print("Humidite actuelle : "); 
    Serial.print(this->getHumidity());
    Serial.println(" %RH");   
  }
};
#endif 
