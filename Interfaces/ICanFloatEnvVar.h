#pragma once

#include "ICanEnvVar.h"
#include <stdint.h>

class ICanFloatEnvVar : public ICanEnvVar
{
protected:
   ICanFloatEnvVar() {};
   virtual ~ICanFloatEnvVar() {};
public:
   virtual double GetMinimum(void) const = 0;
   virtual double GetMaximum(void) const = 0;
   virtual double GetInitialValue(void) const = 0;
};