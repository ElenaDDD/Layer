#include <TimerOne.h>
#include "Layer.h"

enum Menu{
  MAIN,
  SET_METERS,
  SET_ACCELERATION,
  SET_LAYER_STEPS
  };


 
class Display
{
    private:
      MAX7219 _display;
        char _charbuf[4];

    public:


    void ShowTime(unsigned int minutes){
      sprintf(_charbuf,"%i", minutes);
      //Serial.print("_charbuf is");Serial.println(_charbuf);


      _display.DisplayText(_charbuf, 0);
    };

    void ShowMeters(unsigned int meters)
    {
      sprintf(_charbuf,"%u", meters);
      _display.DisplayText(_charbuf, 1);
    };

    void ShowInt(unsigned int value)
    {
      sprintf(_charbuf,"%u", value);
      //Serial.print("_charbuf is");Serial.println(_charbuf);
      _display.DisplayText(_charbuf, RIGHT);
    }

    void Clear()
    {
      _display.Clear();
    };

    void ClearMeters()
    {
       _display.DisplayText("    ", 1); //Right justified
    }

    void ShowMenu(Menu m, Layer* layer, bool showValue)
    {
      switch (m)
      {
        case SET_METERS:
            _display.DisplayText("FO",LEFT);
            if (showValue) ShowMeters(layer->counter.getCurrentlySelectedTargetInList());
          break;
         case SET_ACCELERATION:
            _display.DisplayText("F1", LEFT);
            if (showValue)  ShowInt(layer->GetCurrentAcceleration());
            break;
         case SET_LAYER_STEPS:
            _display.DisplayText("F2", LEFT);
            if (showValue)  ShowInt(layer->GetCurrentLayerSteps());
            break;
        if (!showValue) _display.DisplayText("     ", RIGHT);
      }
    }



    void Begin()
    {
      _display.Begin();
    };
};
