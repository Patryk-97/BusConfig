#include "CanFloatAttribute.h"

CanFloatAttribute::~CanFloatAttribute()
{
   this->Clear();
}

void CanFloatAttribute::Clear(void)
{
   CanAttribute::Clear();
   this->minimum = 0;
   this->maximum = 0;
   this->defaultValue = 0;
}

const char* CanFloatAttribute::GetName(void) const
{
   return CanAttribute::GetName();
}

ICanAttribute::IValueType_e CanFloatAttribute::GetValueType(void) const
{
   return CanAttribute::GetValueType();
}

ICanAttribute::IObjectType_e CanFloatAttribute::GetObjectType(void) const
{
   return CanAttribute::GetObjectType();
}

double CanFloatAttribute::GetMinimum(void) const
{
   return this->minimum;
}

void CanFloatAttribute::SetMinimum(double minimum)
{
   this->minimum = minimum;
}

double CanFloatAttribute::GetMaximum(void) const
{
   return this->maximum;
}

void CanFloatAttribute::SetMaximum(double maximum)
{
   this->maximum = maximum;
}

double CanFloatAttribute::GetDefaultValue(void) const
{
   return this->defaultValue;
}

void CanFloatAttribute::SetDefaultValue(double defaultValue)
{
   this->defaultValue = defaultValue;
}