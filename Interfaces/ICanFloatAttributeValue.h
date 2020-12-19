#pragma once

#include "ICanAttributeValue.h"

class ICanFloatAttributeValue : public ICanAttributeValue
{
protected:
   ICanFloatAttributeValue() {};
   virtual ~ICanFloatAttributeValue() {};
public:
   virtual double GetValue(void) const = 0;
};