#include <Stepper.h>

#include <max7219.h>
#include "Display.h"
#include "Button.h"
#include "Counter.h"
#include "OpticalSensor.h"
#include "Time.h"
#include "Layer.h"
#include "Encoder.h"

#ifndef uint 
  #define uint unsigned int
#endif

//MAX7219
#define MAX_CLK 10
#define MAX_CS 11
#define MAX_DIN 12


//driver
#define DRIVER_N1_PIN 3
#define DRIVER_N2_PIN 4
#define DRIVER_N3_PIN 5
#define DRIVER_N4_PIN 6

//encoder
#define ENCODERBUTTON_PIN 7
#define ENCODECLK_PIN 9
#define ENCODERDT_PIN 8
#define ENCODERBUTTON_PRESSED_STATE 0

#define OPTICALSENSOR_COUNTER_PIN 20
#define OPTICALSENSOR_PARKING_PIN 21

#define START_STOP_BUTTON_PIN 22
#define START_STOP_BUTTON_PRESSED_STATE 1

#define SIGNAL_METERS 10

#define MINDEXPERIMENT


Display display;
Time time;



#define sensorNumbers  5
#define  buttonsNumber  3

Sensor* sensorsArray[sensorNumbers];
Button* buttonsArray[buttonsNumber];

//Counter counter;

Button *resetButton, *encoderButton, *driverOnOffButton;
Encoder *encoder;
OpticalSensor *opticalSensor_counter, *opticalSensor_parking;

volatile Menu currentMenu;
 bool needsRefresh = false;

volatile int refresh = 1;

bool showNumWhileBlink = true;


unsigned char encoder_A, encoder_B, encoder_A_prev;

unsigned long ONE_SECOND = 1000000;
unsigned long TIMER_PERIOD  = 100000;
unsigned long BUTTON_DELAY = 500000;
unsigned long STEPS_PER_REVOLUTION = 1000;

int timerCounter = 0;


Stepper stepper1 = Stepper(STEPS_PER_REVOLUTION, DRIVER_N1_PIN, DRIVER_N2_PIN, DRIVER_N3_PIN, DRIVER_N4_PIN); //on wiring change places of pins 2 & 3
Layer  layer;

#define ENCODER_DELAY 5;
int encoderDelay = ENCODER_DELAY;

bool ProcessEncoderDelay()
{
  return  (encoderDelay == 0) ;
}


 void ShowMenu(Menu m)
{
  if (needsRefresh == true )
    {
        //Serial.print("ShowMenu");
        needsRefresh = false;
        display.Clear();
      if (m == MAIN)
      {   
        //Serial.println("Current menu is MAIN");
        display.ShowMeters(layer.counter.GetMeters());
        display.ShowTime(time.GetTotalMinutes());
      }
      else if (m == SET_SPOOL_DEPTH)
      {
         Serial.print("currently selected spool length in mm =  ");Serial.println(layer.GetCurrentSpoolDepth());
           display.ShowMenu(m, &layer, showNumWhileBlink);
      }
      else if (m == SET_METERS)
      {
          Serial.print("currently selected meters =  ");Serial.println(layer.counter.getCurrentlySelectedTargetInList());
           display.ShowMenu(m, &layer, showNumWhileBlink);
      }
      else if (m == SET_ACCELERATION)
      {
          Serial.print("show while blink  = "); Serial.println(showNumWhileBlink);         
           display.ShowMenu(m, &layer, showNumWhileBlink);
      }
      else if (m == SET_LAYER_STEPS)
      {
          Serial.print("currently layer steps =  ");Serial.println(layer.GetCurrentLayerSteps());
           //display.ShowInt(layer.GetCurrentLayerSteps());
           display.ShowMenu(m, &layer, showNumWhileBlink);
      }
      else
      {
        Serial.print("Undefined menu");
      }
    }
};


void TimerProc(void)
{
    timerCounter ++;
    
    showNumWhileBlink =  (timerCounter %2 == 0)  ? true: false;
    
    for (byte i = 0; i < sensorNumbers; i++) {

      
      sensorsArray[i]->PollSensor();
    }

    for (byte i = 0; i < (sizeof(buttonsArray) / sizeof(buttonsArray[0])); i++) 
    {    
      buttonsArray[i]->ProcessDelay();
    }
    
    //check buttons
    if (resetButton->IsPressed())
    {
      Serial.println("reset pressed");
      time.ResetTimer();
      layer.counter.Reset();
      Serial.println("needsRefresh 160");
      needsRefresh = true;
    }
    
    if (encoderButton->IsPressed())
    {
       Serial.println("encoder pressed");
       //check current menu we're in
       //if we're in main, than go to choose target meters
        if (currentMenu == MAIN)
        {
          Serial.println("Current menu is MAIN, set to SET_SPOOL_LENGTH");
          currentMenu = SET_SPOOL_DEPTH;
        }
        else if (currentMenu == SET_SPOOL_DEPTH)
        {
           Serial.println("Current menu is SET_SPOOL_LENGTH, set to SET_METERS");
           layer.SetSpoolLength();
          currentMenu = SET_METERS;
        }
        else if (currentMenu == SET_METERS){
        //if we're in target meters menu 
        //save target
        // goto main
        Serial.println("Current menu is SET_METERS, set to SET_ACCELERATION");
          layer.counter.SetTargetMeters();
          currentMenu = SET_ACCELERATION;
        }
        else if (currentMenu == SET_ACCELERATION)
        {           
           Serial.println("Current menu is SET_ACCELERATION, set to SET_LAYER_STEPS");
           layer.SetAcceleration();
           currentMenu = SET_LAYER_STEPS;
        }
        else if (currentMenu == SET_LAYER_STEPS)
        {
          Serial.println("Current menu is SET_LAYER_STEPS, set to MAIN");
          layer.SetLayerSteps();
          currentMenu = MAIN;
        }
        Serial.println("needsRefresh 182");
        needsRefresh = true;
    }
   /* Direction dir = encoder->GetDirection();
    if (dir != NONE)
    {
      
      if (currentMenu == SET_METERS)
      {
        counter.setTargetSelectionInList(dir);
        needsRefresh = true;
      }
    }*/

    //check opticalSensor
    if (opticalSensor_counter->IsNewLeaf())
    {
      layer.counter.AddDistance(opticalSensor_counter->GetDistanceToIncrease());
    // needsRefresh = true;
    }
    if (layer.counter.DidIncrease())
    {
        //Serial.println("needsRefresh 204");
        needsRefresh = true;
    }
    
#ifdef MINDEXPERIMENT 
      if (timerCounter % 2  == 0)
      {
        // Serial.println("Adding distance");
        layer.counter.AddDistance(opticalSensor_counter->GetDistanceToIncrease());
      }
#endif
    
    if (timerCounter == (ONE_SECOND / TIMER_PERIOD))
    {
      //one second passed
      time.AddSecond();
      timerCounter = 0;
      //Serial.println("needsRefresh 216");
      needsRefresh = true;
    }
    //TODO: do not update on every timer proc PROBABLY
    if (currentMenu == SET_METERS || currentMenu == SET_ACCELERATION || currentMenu == SET_LAYER_STEPS )
    {
      //Serial.println("needsRefresh 222");
      needsRefresh = true;
    }
    
  
    /*if (counter.ShouldSignal() )
    {
         Serial.println("Should signal");
         shouldUpdateBuzzer = true;
         if(showNumWhileBlink)
         {  
            soundOn = true;
         }
         else
         {
            soundOn = false;
         }
    }
    else 
    {
      noTone(BUZZER_PIN);
      digitalWrite(13,0);
      shouldUpdateBuzzer = false;
    }*/ //we won't need this probably

    if (encoderDelay != 0)
    {
      encoderDelay--;
    }
}


const int stepsPerRevolution = 1000;





void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    display.Begin();


  //setup reset button;
    pinMode(START_STOP_BUTTON_PIN, INPUT_PULLUP);
    pinMode(OPTICALSENSOR_COUNTER_PIN, INPUT_PULLUP);
    pinMode(ENCODERBUTTON_PIN, INPUT_PULLUP);
    pinMode(ENCODECLK_PIN, INPUT_PULLUP);
    pinMode(ENCODERDT_PIN, INPUT_PULLUP);
    //Serial.println("delay value  = ");Serial.println( BUTTON_DELAY/ TIMER_PERIOD );
    resetButton = new Button();
    resetButton->Init(START_STOP_BUTTON_PIN, BUTTON_DELAY/ TIMER_PERIOD , START_STOP_BUTTON_PRESSED_STATE);
    sensorsArray[0] = resetButton;
    buttonsArray[0] = resetButton;

    opticalSensor_counter = new OpticalSensor();
    opticalSensor_counter->Init(OPTICALSENSOR_COUNTER_PIN);
    sensorsArray[1] = opticalSensor_counter;

    opticalSensor_parking = new OpticalSensor();
    opticalSensor_parking->Init(OPTICALSENSOR_PARKING_PIN);
    sensorsArray[2] = opticalSensor_parking;

    //setup encoder
    //button
    encoderButton = new Button();    
    encoderButton->Init(ENCODERBUTTON_PIN, BUTTON_DELAY/ TIMER_PERIOD , ENCODERBUTTON_PRESSED_STATE);
    sensorsArray[3] = encoderButton;
    buttonsArray[1] = encoderButton;
    //rotation
    encoder = new Encoder();
    encoder->Init(ENCODECLK_PIN,ENCODERDT_PIN );
    sensorsArray[4] = encoder;

    //pinMode(BUZZER_PIN, OUTPUT);
    display.Clear();
    layer.counter.Init(SIGNAL_METERS);
    Serial.print("Setup finished, button is pressed = ");Serial.println(buttonsArray[1]->IsPressed());

    currentMenu = MAIN;

    //shouldUpdateBuzzer= false;
    pinMode(13, OUTPUT);

   
    
    //do this in the end of the setup!
     Serial.print("Init Timer with  ");Serial.println(TIMER_PERIOD);
    Timer1.initialize(TIMER_PERIOD);
    Timer1.attachInterrupt(TimerProc);
}


/*void setup() {
  // Set the speed to 5 rpm:
  /*myStepper.setSpeed(5);
  
  // Begin Serial communication at a baud rate of 9600:
  Serial.begin(9600);
}*/
void loop() {

  
  
  /*Serial.println("clockwise");
  myStepper.step(stepsPerRevolution);
  delay(500);
  
  // Step one revolution in the other direction:
  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution);
  delay(500);*/

  ShowMenu(currentMenu);

  //buzzer
  /*if (shouldUpdateBuzzer)
  {
    if (soundOn)
    {
       Serial.println("tone");
      tone(BUZZER_PIN, 1000);
      digitalWrite(13,1);
    }
    else
    {
      Serial.println("no tone");
      noTone(BUZZER_PIN);
      digitalWrite(13,0);
    }
    shouldUpdateBuzzer = false;
  }*/

  if (currentMenu == SET_METERS || currentMenu == SET_ACCELERATION || currentMenu == SET_LAYER_STEPS)
      {
      encoder_A = digitalRead(ENCODECLK_PIN);
      encoder_B = digitalRead(ENCODERDT_PIN);
    
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
          //операции по обработке данных в нужном направлени
          bool canProcess = ProcessEncoderDelay();
          if (canProcess)
          {
             encoderDelay = ENCODER_DELAY;
          Serial.println("increment");
          switch (currentMenu)
          {
            case SET_METERS: 
              layer.counter.IncrementTarget();
              break;
            case SET_ACCELERATION:
              layer.IncrementAcceleration();
              break;
            case SET_LAYER_STEPS:
              layer.IncrementLayerSteps();
              break;
            default:
              //do nothing
              break;
          }
          }
          
          
        }
        //Если уровень сигнала В низкий
        else
        {
          //Значит вращение происходит против часовой стрелки
          //Здесь можно вставить операцию декремента
          //Здесь можно вставлять какие либо свои 
          //операции по обработке данных в нужном направлении
          bool canProcess = ProcessEncoderDelay();
          if (canProcess)
          {
            encoderDelay = ENCODER_DELAY;
            Serial.println("decrement");
             switch (currentMenu)
            {
              case SET_METERS: 
                layer.counter.DecrementTarget();
                break;
              case SET_ACCELERATION:
                layer.DecrementAcceleration();
                break;
              case SET_LAYER_STEPS:
                layer.DecrementLayerSteps();
                break;
              default:
                //do nothing
                break;
            }
          }
        }
      }
      //Обязательно нужно сохранить состояние текущего уровня сигнала А
      //для использования этого значения в следующем цикле сканирования программы
      encoder_A_prev = encoder_A;
      }


      
}
