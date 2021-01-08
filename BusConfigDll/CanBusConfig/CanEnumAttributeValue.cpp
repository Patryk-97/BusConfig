#include "CanEnumAttributeValue.h"

CanEnumAttributeValue::~CanEnumAttributeValue()
{
   this->Clear();
}

void CanEnumAttributeValue::Clear()
{
   CanAttributeValue::Clear();
   this->value = "";
}

const char * CanEnumAttributeValue::GetValue(void) const
{
   return this->value.c_str();
}

void CanEnumAttributeValue::SetValue(const std::string& value)
{
   this->value = value;
}