#pragma once
#include "rgbled.h"

class Pattern {
protected:
  Pattern();
public:
  virtual void setup();
  virtual void tick(); 
};
