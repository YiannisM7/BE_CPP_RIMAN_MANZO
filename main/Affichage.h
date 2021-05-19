#include <U8x8lib.h>

class Affichage:public U8X8_SH1107_SEEED_128X128_SW_I2C{
  public:
  // ***** Constructeur *****
  Affichage():U8X8_SH1107_SEEED_128X128_SW_I2C (/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE){}
  // Destructeur
  ~Affichage(){};
  // ***** Méthodes *****
  // Fonction d'initialisation de l'affichage
  void Affichage_Init()
    {
      this->begin();
      this->setPowerSave(0);
      this->setFont(u8x8_font_chroma48medium8_r);
      this->setCursor(0,0);
      this->print("TEMP:");
      this->setCursor(0,3);
      this->print("HUM:");
      this->setCursor(0,6);
      this->print("Consigne:");
      this->setCursor(0,7);
      this->print("temp:");
      this->setCursor(0,10);
      this->print("hum");
      //Font pour affichage dynamique
      this->setFont(u8x8_font_px437wyse700a_2x2_r);    
    }
    // Fonctions d'affichage des consignes en T° et Hum
    void Affichage_TempHum(float temp, float hum)
    {
       this->setCursor(0,1);
       this->print(temp);          
       this->setCursor(0,4);
       this->print(hum); 
    }
    void Affichage_Cons_Temp(float temp)
    {
      this->setCursor(0,8);    
      this->print(temp);
    }
    void Affichage_Cons_Hum(float hum)
    {
      this->setCursor(0,11);    
      this->print(hum);
    }
  
};
