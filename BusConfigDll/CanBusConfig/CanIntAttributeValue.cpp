#include "CanIntAttributeValue.h"

CanIntAttributeValue::~CanIntAttributeValue()
{
   this->Clear();
}

void CanIntAttributeValue::Clear()
{
   CanAttributeValue::Clear();
   this->value = 0;
}

int32_t CanIntAttributeValue::GetValue(void) const
{
   return this->value;
}

void CanIntAttributeValue::SetValue(int32_t value)
{
   this->value = value;
}