#pragma once

#include "ICanFloatAttributeValue.h"

class CanFloatAttributeValue : public ICanFloatAttributeValue
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