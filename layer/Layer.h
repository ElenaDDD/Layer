class Layer {
  public:
    int meters;
    int  acceleration; //maybe needs change of type,, haven't decided yet
    int layer_steps;

    Layer()
    {
      meters = 200;
      acceleration = 20;
      layer_steps = 100;
    }
};
