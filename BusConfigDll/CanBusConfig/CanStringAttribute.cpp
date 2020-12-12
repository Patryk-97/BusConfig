#include "CanStringAttribute.h"

CanStringAttribute::~CanStringAttribute()
{
   this->Clear();
}

void CanStringAttribute::Clear(void)
{
   CanAttribute::Clear();
   this->defaultValue = "";
}

const char* CanStringAttribute::GetName(void) const
{
   return CanAttribute::GetName();
}

ICanAttribute::IValueType_e CanStringAttribute::GetValueType(void) const
{
   return CanAttribute::GetValueType();
}

ICanAttribute::IObjectType_e CanStringAttribute::GetObjectType(void) const
{
   return CanAttribute::GetObjectType();
}

const char* CanStringAttribute::GetDefaultValue(void) const
{
   return this->defaultValue.c_str();
}

void CanStringAttribute::SetDefaultValue(const char* defaultValue)
{
   this->defaultValue = defaultValue;
}