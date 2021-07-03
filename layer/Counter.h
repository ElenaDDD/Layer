#include "Direction.h"

#define OPTIONS_COUNT 8

class Counter{
  private:
    
    float _distance;
    unsigned int _meters;
    int _selectedIndex = 0;
    int _selectedMeters;
    int _currentTargetMeters;
    int _targetMeters[OPTIONS_COUNT] = {15,42, 172, 202, 302, 342, 402 ,1002 };
    int _signalMeters;
    bool _didIncrease = false;
  public:
    void Init(int s){
      Reset();
      _currentTargetMeters = getCurrentlySelectedTargetInList();
      _selectedIndex = 0;
      _signalMeters = s;
       bool _didIncrease = false;
    }
    
  void AddMeter()
  {
    _distance += 1000;
  }

  void Reset()
  {
    _distance = 0;
  }

  unsigned int GetMeters()
  {
    //Serial.print("current meters ");Serial.println( (int)(_distance / 1000));
    return (int)(_distance / 1000);
  }

  void AddDistance (float d)
  {
    unsigned int prevMeters = (int)(_distance / 1000);
    _distance += d;
    if ((int)(_distance / 1000) - prevMeters > 0) _didIncrease = true;
    //Serial.print("current distance ");Serial.println(_distance);
  }

  int getNextTarget(Direction dir)
  {
    if (dir == LEFT)
    {
      if (--_selectedIndex < 0 ) _selectedIndex = OPTIONS_COUNT-1;
      return _targetMeters[_selectedIndex];
    }
    else if (dir == RIGHT)
    {
      if (++_selectedIndex == OPTIONS_COUNT) _selectedIndex = 0;
      return _targetMeters[_selectedIndex];
    }
    else
    {
     // Serial.println("WARNING. trying to set target while no direction");
    }
  }

    void setTargetSelectionInList(Direction dir)
  {
    // Serial.print("direction = ");  Serial.println(dir);
    if (dir == LEFT)
    {
      if (--_selectedIndex < 0 ) _selectedIndex = OPTIONS_COUNT - 1;
      //return _targetMeters[_selectedIndex];
    }
    else if (dir == RIGHT)
    {
      if (++_selectedIndex == OPTIONS_COUNT) _selectedIndex = 0;
      //return _targetMeters[_selectedIndex];
    }
    else
    {
      //Serial.println("WARNING. trying to set target while no direction");
    }
     //Serial.println("WARNING. trying to set target while no direction");
  }

   int getCurrentlySelectedTargetInList()
  {   
      //Serial.print("_selectedIndex = ");  Serial.println(_selectedIndex);
      return _targetMeters[_selectedIndex];
  }


  void SetTargetMeters()
  {
    _currentTargetMeters = _targetMeters[_selectedIndex];
  }

  bool ShouldSignal()
  {
    //Serial.print(" _currentTargetMeters - GetMeters() ");  Serial.println( _currentTargetMeters - GetMeters() );
     //Serial.print(" _signalMeters");  Serial.println( _signalMeters );
    return GetMeters() > _currentTargetMeters || ( _currentTargetMeters - GetMeters() ) < _signalMeters ;
  }

  void IncrementTarget()
  {
    if (++_selectedIndex == OPTIONS_COUNT) _selectedIndex = 0;
  }

    void DecrementTarget()
  {
    if (--_selectedIndex < 0 ) _selectedIndex = OPTIONS_COUNT - 1;
  }

  bool DidIncrease()
  {
    bool did = _didIncrease;
    _didIncrease = false;
    return did;
  }
};
