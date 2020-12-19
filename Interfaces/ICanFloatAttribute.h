#pragma once

#include "ICanAttribute.h"

class ICanFloatAttribute : public ICanAttribute
{
protected:
   ICanFloatAttribute() {};
   virtual ~ICanFloatAttribute() {};
public:
   virtual void Clear(void) = 0;
   virtual double GetMinimum(void) const = 0;
   virtual double GetMaximum(void) const = 0;
   virtual double GetDefaultValue(void) const = 0;
};