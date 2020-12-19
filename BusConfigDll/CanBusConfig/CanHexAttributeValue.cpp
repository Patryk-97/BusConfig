#include "CanHexAttributeValue.h"

CanHexAttributeValue::~CanHexAttributeValue()
{
   this->Clear();
}

void CanHexAttributeValue::Clear()
{
   this->value = 0;
}

int32_t CanHexAttributeValue::GetValue(void) const
{
   return this->value;
}

void CanHexAttributeValue::SetValue(int32_t value)
{
   this->value = value;
}