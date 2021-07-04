#ifndef LAYER_H
#define LAYER_H

#include "Counter.h"
class Layer {
  public:
    Counter counter;
    int  acceleration; //maybe needs change of type,, haven't decided yet
    int layer_steps;

    Layer()
    {
      counter.SetTargetMeters();
      acceleration = 20;
      layer_steps = 100;
    }

    void SetAcceleration()
    {
      
    }

    void SetLayerSteps()
    {
      
    }

    void IncrementAcceleration()
    {
      
    }

    void DecrementAcceleration()
    {
      
    }

    int GetCurrentAcceleration()
    {
      return acceleration;
    }

    void IncrementLayerSteps()
    {
      
    }

    void DecrementLayerSteps()
    {
      
    }

    int GetCurrentLayerSteps()
    {
      return layer_steps;
    }
};

#endif
