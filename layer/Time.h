#define uint unsigned int

struct Time
{
  uint seconds;
  uint minutes;
  uint hours;
  uint totalMinutes;

  void AddSecond() 
  {
    seconds ++;
    if (seconds ==60)
    {
      seconds = 0;
      minutes ++;
      totalMinutes++;
      if (minutes == 60 )
      {
        hours ++;
        if (hours == 24)
          hours = 0;
      }
    }
  }

  void ResetTimer(){
    seconds = 0;
    minutes = 0;
    hours = 0;
    totalMinutes = 0;
  }
  uint GetTotalMinutes()
  {
    return totalMinutes;
  }
};
