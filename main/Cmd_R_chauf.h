#ifndef _CMD_PWM_H_
#define _CMD_PWM_H_
#define _INPUT 0
#define PULL_UP 1
#define DEFAULT_PIN 10
#define DEFAULT_FREQ 1000
#define PIN 1

//***** I/O *****
class GPIO 
{
  public: 
    GPIO(byte pin):pin(pin){} //on peut rajouter une exception pour s'assurer aue la pin soit bonne
  protected:
    byte pin;
};



//***** Bouton poussoir *****
class BP: public GPIO 
{
   public:
        // ***** Constructeur *****
        BP(byte pin, bool modePin=PULL_UP): GPIO(pin),modePin(modePin)
        
        {
          old_state=this->modePin;

          switch(modePin)
         {
           case _INPUT:
               pinMode(pin,INPUT);
               break;
               
           case PULL_UP:
                  pinMode(pin,INPUT_PULLUP);
                  break;
          // default:
                  //trhow expction
                  
          }
        }

        // ***** Méthode *****
        char detect_front()
        {

          char res=0;
          char  current_state=digitalRead(this->pin);
          if (this->modePin== PULL_UP)
          {
            if((current_state==0) && (old_state==1)) res=1; 
            else res=0;
            old_state=current_state;
          }
          else 
          {
            if((current_state==1) && (old_state==0)) res=1;
            else res=0;
            old_state=current_state;

          }
          return res;
        }
        
   protected: 
        // ***** Attributs *****
        char modePin;
        char old_state;
};



//***** PWM *****
class cmd_pwm: public GPIO // /!\ Le circuit analogique à commander a une logique inverse
{
    public:
        // ***** Constructeur *****
        cmd_pwm(byte pin=DEFAULT_PIN, int frequency=DEFAULT_FREQ):GPIO(pin), frequency(frequency)
        {
          duty_cycle=0;
          this->pin=pin;
          pinMode(pin, OUTPUT);
        }

        // ***** Méthodes *****
        // Getter
        float get_duty()const
        {
            return this->duty_cycle;
        }
        // Setter
        void set_duty (float duty_cycle)
        {
            if (duty_cycle<0) this->duty_cycle=0;
            else 
            {
              if (duty_cycle>1) this->duty_cycle=1;
              else  this->duty_cycle=duty_cycle;
            }          
        }
        void off_cmd() //correspond au temps en haut
        {
            duty_cycle=0;
            analogWrite(this->pin, 0);
        }
        // ***** Attributs *****
     protected:
        float frequency;
        float duty_cycle;
};



#endif
