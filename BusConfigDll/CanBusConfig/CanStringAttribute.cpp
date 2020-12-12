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

const char* CanStringAttribute::GetDefaultValue(void) const
{
   return this->defaultValue.c_str();
}

void CanStringAttribute::SetDefaultValue(const char* defaultValue)
{
   this->defaultValue = defaultValue;
}