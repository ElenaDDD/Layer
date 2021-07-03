#include <TimerOne.h>

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
      //Serial.print("_charbuf is");Serial.println(_charbuf);
      _display.DisplayText(_charbuf, 1);
    };

    void showInt(unsigned int value)
    {
      sprintf(_charbuf,"%u", value);
      //Serial.print("_charbuf is");Serial.println(_charbuf);
      _display.DisplayText(_charbuf, 1);
    }

    void Clear()
    {
      _display.Clear();
    };

    void ClearMeters()
    {
       _display.DisplayText("    ", 1); //Right justified
    }



    void Begin()
    {
      _display.Begin();
    };
};
