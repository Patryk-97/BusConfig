#include "CanStringAttributeValue.h"

CanStringAttributeValue::~CanStringAttributeValue()
{
   this->Clear();
}

void CanStringAttributeValue::Clear()
{
   CanAttributeValue::Clear();
   this->value = "";
}

const char * CanStringAttributeValue::GetValue(void) const
{
   return this->value.c_str();
}

void CanStringAttributeValue::SetValue(const std::string& value)
{
   this->value = value;
}