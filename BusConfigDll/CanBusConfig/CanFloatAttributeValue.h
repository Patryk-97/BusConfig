#pragma once

#include "ICanFloatAttributeValue.h"
#include "CanAttributeValue.h"

class CanFloatAttributeValue : public CanAttributeValue, public ICanFloatAttributeValue
{
public:
   CanFloatAttributeValue() = default;
   CanFloatAttributeValue(double value) : value(value) {}
   ~CanFloatAttributeValue();

   void Clear(void) override;
   double GetValue(void) const override;
   void SetValue(double value);

private:
   double value {};
};