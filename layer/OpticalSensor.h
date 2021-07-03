#include "Sensor.h"

#define ACTIVE_STATE 1
#define LEAF_COUNT 2
float D = 44.5;


class OpticalSensor : public Sensor{
  private:
    int _state = 0;
    int _prevState = 0;
    bool _increaseDistance = false;
    float _oneLeafDistance = 0;
    unsigned int _pin = 0;

  public:
    void Init( unsigned int pin)
    {
      _oneLeafDistance = (D*PI)/LEAF_COUNT;
      _pin = pin;
    }
    virtual void PollSensor()
    {
        int currentState = digitalRead(_pin);
        if (_prevState == 0 && currentState ==1 )
        {
          //leaf started here
          //we need only front
          _increaseDistance = true;
          
        }
        else  if (_prevState == 1 && currentState == 0 )
          {
            //leaf ended here
          }
         
        _prevState = currentState;
    }

    bool IsNewLeaf()
    {
      bool is = _increaseDistance;
      _increaseDistance = false;
      return is;
    }

    float GetDistanceToIncrease()
    {
      return _oneLeafDistance;
    }
};
