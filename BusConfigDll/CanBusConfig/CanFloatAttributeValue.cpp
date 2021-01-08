#include "CanFloatAttributeValue.h"

CanFloatAttributeValue::~CanFloatAttributeValue()
{
   this->Clear();
}

void CanFloatAttributeValue::Clear()
{
   CanAttributeValue::Clear();
   this->value = 0.f;
}

double CanFloatAttributeValue::GetValue(void) const
{
   return this->value;
}

void CanFloatAttributeValue::SetValue(double value)
{
   this->value = value;
}