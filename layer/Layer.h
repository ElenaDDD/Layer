#ifndef LAYER_H
#define LAYER_H

#include "Counter.h"


enum LayerMode
{
  RUNNING,
  AWAIT
};

class Layer {
  public:
    Counter counter;
    int  acceleration; //maybe needs change of type,, haven't decided yet
    int layer_steps;
    int spool_depth;    //spool depth in mm

    Layer()
    {
      counter.SetTargetMeters();
      acceleration = 20;
      layer_steps = 100;
      spool_depth = 200; //
    }

    void SetAcceleration()
    {
      
    }

    void SetLayerSteps()
    {
      
    }

    void IncrementAcceleration()
    {
      acceleration++;
    }

    void DecrementAcceleration()
    {
      acceleration--;
    }

    int GetCurrentAcceleration()
    {
      return acceleration;
    }

    void IncrementLayerSteps()
    {
      layer_steps++;
    }

    void DecrementLayerSteps()
    {
      layer_steps--;
    }

    int GetCurrentLayerSteps()
    {
      return layer_steps;
    }

    int GetCurrentSpoolDepth()
    {
       return spool_depth;
    }

    void IncrementSpoolDepth()
    {
      spool_depth++;
    }

    void DecrementSpoolDepth()
    {
      spool_depth--;
    }

    void SetSpoolLength()
    {
      
    }
};

#endif
