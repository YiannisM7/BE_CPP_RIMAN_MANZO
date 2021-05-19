// **********************************************************************
#include <Arduino.h>
#include <Ticker.h>
#include "actuator.h"
#include "sensor.h"
#include "Cmd_R_chauf.h"
#include "Affichage.h"
// Includes utilisés pour une intéraction Web (non présentée dans le projet)
//#include "wifi.h"
//#include "server.h"



// **********************************************************************
#define F_CRYST         80000000 //Hz
#define F_SAMPLE            10000 //Hz

#define PRECISION 1023 //Pour générer PWM

#define PIN_R 16 // PORT D0
#define BP_T_PLUS 14 //PORT D5
#define BP_T_MOINS 12 //PORT D6
#define BP_H_PLUS  0//PORT D3
#define BP_H_MOINS 13//PORT D7

#define TEMPERATURE_REF 25
#define TEMP_MAX  30
#define TEMP_MIN  20

#define HUMIDITY_REF 60
#define HUM_MIN 50
#define HUM_MAX 80

//#define PIN_TEST 15 //D8


// **********************************************************************
//Variables manipulées par l'IT
volatile int interrupts; //déclarer en volatile pour ne pas etre modifié a la compile et etre utilise dans routine d'It et main
volatile int compteur_sec=0; 
volatile int compteur_pwm=0;

//Variables pour régulations température et humidité
float duty_to_set;
float temperature_ref=TEMPERATURE_REF;
float humidity_ref=HUMIDITY_REF;
float  error=0;
float  temp=0;
float  hum=0;

//Variables de controle : pour refresh affichage consignes si changement
byte refreshTemp=0;
byte refreshHum=0;


// **********************************************************************
// Instanciation des capteurs
TempSensor thermometre = TempSensor();
HumSensor hygrometre = HumSensor();

//Instanciation boutons poussoir
BP Bouton_T_moins (BP_T_MOINS);
BP Bouton_T_plus (BP_T_PLUS);
BP Bouton_H_moins (BP_H_MOINS);
BP Bouton_H_plus (BP_H_PLUS);

// Instanciation des actuateurs
cmd_pwm resistance(PIN_R); 
Atomizer monBrumisateur = Atomizer();

// Instanciation affichage
Affichage ecranLED = Affichage();

// Instantiation timer
Ticker timer;

// Instantiation objet écran LED
//U8X8_SH1107_SEEED_128X128_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE); 


// **********************************************************************
//Routine pour controle température et humidité
void ICACHE_RAM_ATTR IT_T1(){
  interrupts=1;
  // Gestion seconde
  compteur_sec++;  
  if(compteur_pwm<1023){
      compteur_pwm+=1;}
  else{ 
      compteur_pwm=0;}
  // Mise en place de la PWM
  if(compteur_pwm>=(int)((1-resistance.get_duty())*PRECISION)){
      digitalWrite(PIN_R, 0);
  }else{
      digitalWrite(PIN_R, 1);
  }
  timer1_write(F_CRYST/(16*F_SAMPLE));     
}

// Fonction d'initialisation de l'échantillonnage
void init_sampling()
{
  timer1_attachInterrupt(IT_T1);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);//Prescaler=16 (5 tick/us), TIM_LOOP= pas besoin de recharger une valeur dans le timer 
  timer1_write(F_CRYST/(16*F_SAMPLE));  
}



// **************************************************************************************************************************
// ************************ SET-UP ******************************************************************************************
// **************************************************************************************************************************
void setup() {
  Serial.begin(115200); 
  thermometre.begin();
  hygrometre.begin();
  //Affichage fixe pour perdre moins de temps dans le main
   ecranLED.Affichage_Init();  
   ecranLED.Affichage_Cons_Temp(temperature_ref);
   ecranLED.Affichage_Cons_Hum(humidity_ref);
   init_sampling();

   monBrumisateur.initBrumisateur();
   Serial.println("tt");

}



// **************************************************************************************************************************
// **************************** LOOP ****************************************************************************************
// **************************************************************************************************************************
void loop()
{
  // ***** Gestion des boutons de modification des consignes de température et d'humidité *****
  if(Bouton_T_plus.detect_front())
  {
    refreshTemp=1; //pour demander de raraichir l'affichage de consigne temperature car changement
    if( temperature_ref<TEMP_MAX)
     temperature_ref+=0.5;
     
  }
  if(Bouton_T_moins.detect_front())  
  {
    refreshTemp=1;//pour demander de raraichir l'affichage de consigne temperature car changement
    if(temperature_ref>TEMP_MIN)
     temperature_ref-=0.5;
  }
  if(Bouton_H_plus.detect_front())
  {
    refreshHum=1; //pour demander de raraichir l'affichage de consigne humidité car changement
    if(humidity_ref<HUM_MAX)
     humidity_ref=humidity_ref+0.5;
     
  }
  if(Bouton_H_moins.detect_front())  
  {
    refreshHum=1;//pour demander de raraichir l'affichage de consigne humidité car changement
    
    if(humidity_ref>HUM_MIN)
     humidity_ref-=0.5;
  }


  // ***** Appel périodique à la fonction de régulation ***** 
  if(compteur_sec%(F_SAMPLE/10) == 0)
    {
      try{
        Regul();
      }catch(int e){ // Gestion des exceptions
        switch(e){
          case 1: Serial.println("Probleme lecture temperature...");
          case 2: Serial.println("Probleme lecture humidite...");
        }
      }
    }
    

     
  // ***** Affichage *****
  // Gestion affichage toutes les secondes pour ne pas prendre trop de ressource inutilement
  // NB: Uniquement affichage dynamique
  if(compteur_sec==((F_SAMPLE)-1))
  {
    
    ecranLED.Affichage_TempHum(temp,hum);
    //RAZ compteur_sec:
    compteur_sec=0;   
  }
  if(refreshTemp)
  {
    
    ecranLED.Affichage_Cons_Temp(temperature_ref);   
    refreshTemp=0;  
  }
  if(refreshHum)
  {
    
    ecranLED.Affichage_Cons_Hum(humidity_ref);
    refreshHum=0;
  }
  
  
  

}

// **************************************************************************************************************************
// ***** Fonctions **********************************************************************************************************
// **************************************************************************************************************************
  // Fonction de régulation en température
  void Regul()
  {
    temp=thermometre.getTemperature();
    hum=hygrometre.getHumidity();
    if(temp<0 || temp > 100){throw 1;}  // Exception temp
    if(hum<0 || hum > 100){throw 2;}    // Exception hum
    error=temperature_ref-temp;
    duty_to_set=error;
    resistance.set_duty(duty_to_set);
    monBrumisateur.regulationHum(hum, humidity_ref);
  }
  
  
