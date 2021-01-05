#pragma once

#include "ICanFloatAttribute.h"
#include "CanAttribute.h"

class CanFloatAttribute : public CanAttribute, public ICanFloatAttribute
{
public:
   CanFloatAttribute() : CanAttribute(IValueType_e::FLOAT) {}
   ~CanFloatAttribute();

   void Clear(void) override;

   const char* GetName(void) const override;
   IValueType_e GetValueType(void) const override;
   IObjectType_e GetObjectType(void) const override;

   double GetMinimum(void) const override;
   void SetMinimum(double minimum);

   double GetMaximum(void) const override;
   void SetMaximum(double maximum);

   double GetDefaultValue(void) const override;
   void SetDefaultValue(double defaultValue);

private:
   double minimum {};
   double maximum {};
   double defaultValue {};
};