#include "Sensor.h"

class Button: public Sensor{
  private:
   
    int _delayms = 0, _delayValue = 0;
    unsigned int _pin;
    unsigned int _curstate = 0, _prevstate = 0;
    
    bool _pressed = false;

     bool _delayOn = false;
     unsigned int _PRESSED_STATE;
  
  public:
  
    void Init(unsigned int pin, unsigned int delayValue, unsigned int pressedState)
    {
      _pin = pin;
      _delayValue = delayValue;
      _delayOn = false;
      _PRESSED_STATE = pressedState;
       // Serial.print ("init _delayOn = ");Serial.println(_delayOn);
       // Serial.print ("init _delayValue = ");Serial.println(_delayValue);
    }
    
    
    virtual void PollSensor ()  
    {
      //Serial.println("Poll my button");
      //Serial.print ("_delayOn = ");Serial.println(_delayOn);
      if (!_delayOn)
      {
        //Serial.println("no delay");
        _curstate= digitalRead(_pin); 
         //Serial.print("curstate is "); Serial.println(_curstate);
        if (_curstate == _PRESSED_STATE && _curstate != _prevstate)
        {
          // Serial.print("button is pressed now");
           _pressed = true;   
           _delayOn = true;
           _delayms = _delayValue;
        };
        _prevstate = _curstate;
      }
     // Serial.println("Poll my button end");
    };

    
      bool IsPressed()
      {
        bool was = _pressed;
        _pressed = false;
        return was;
      };

      
      void ProcessDelay()
      {
        //Serial.print ("ProcessDelay _delayms = ");Serial.println(_delayms);
        if (_delayOn)
        {
          
          if (--_delayms == 0)
          {
            _delayOn = false;
            
          }
        }
      }

};
