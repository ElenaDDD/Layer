#include "Sensor.h"
#include "Direction.h"


class Encoder : public Sensor{
  private:
    unsigned char encoder_A, encoder_B, encoder_A_prev;
    unsigned int _pinCLK, _pinDt;
    Direction _dir;
  public:
  void Init(unsigned int pinCLK, unsigned int pinDt)
  {
    _pinCLK = pinCLK;
    _pinDt = pinDt;
  }
   virtual void PollSensor()
   {
      //CLK подключаем к пину 3 на плате Arduino
      //DT  подключаем к пину 4 на плате Arduino
      //Считываем значения выходов энкодера
      //И сохраняем их в переменных
      encoder_A = digitalRead(_pinCLK);
      encoder_B = digitalRead(_pinDt);
    
      //Если уровень сигнала А низкий,
      //и в предидущем цикле он был высокий 
      if(!encoder_A && encoder_A_prev)
      {
        //Если уровень сигнала В высокий
        if(encoder_B)
        {
          //Значит вращение происходит по часовой стрелке
          //Здесь можно вставить операцию инкремента
          //Здесь можно вставлять какие либо свои 
          //операции по обработке данных в нужном направлении
          _dir = RIGHT;
        }
        //Если уровень сигнала В низкий
        else
        {
          //Значит вращение происходит против часовой стрелки
          //Здесь можно вставить операцию декремента
          //Здесь можно вставлять какие либо свои 
          //операции по обработке данных в нужном направлении
          _dir = LEFT;
        }
      }
      //Обязательно нужно сохранить состояние текущего уровня сигнала А
      //для использования этого значения в следующем цикле сканирования программы
      encoder_A_prev = encoder_A;
    }

    Direction GetDirection()
    {
      Direction dir = _dir;
      _dir = NONE;
      return dir;
    }

    
  
};
