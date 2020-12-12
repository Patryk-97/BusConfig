#pragma once

#include "ICanFloatAttribute.h"
#include "CanAttribute.h"

class CanFloatAttribute : public CanAttribute, ICanFloatAttribute
{
public:
   CanFloatAttribute() = default;
   ~CanFloatAttribute();

   void Clear(void) override;

   double GetMinimum(void) const override;
   void SetMinimum(double minimum);

   double GetMaximum(void) const override;
   void SetMaximum(double maximum);

   double GetDefaultValue(void) const override;
   void SetDefaultValue(double defaultValue);

private:
   double minimum{};
   double maximum{};
   double defaultValue{};
};