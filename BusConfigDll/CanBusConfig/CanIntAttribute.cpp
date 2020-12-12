#include "CanIntAttribute.h"

CanIntAttribute::~CanIntAttribute()
{
   this->Clear();
}

void CanIntAttribute::Clear(void)
{
   CanAttribute::Clear();
   this->minimum = 0;
   this->maximum = 0;
   this->defaultValue = 0;
}

const char* CanIntAttribute::GetName(void) const
{
   return CanAttribute::GetName();
}

ICanAttribute::IValueType_e CanIntAttribute::GetValueType(void) const
{
   return CanAttribute::GetValueType();
}

ICanAttribute::IObjectType_e CanIntAttribute::GetObjectType(void) const
{
   return CanAttribute::GetObjectType();
}

int32_t CanIntAttribute::GetMinimum(void) const
{
   return this->minimum;
}

void CanIntAttribute::SetMinimum(int32_t minimum)
{
   this->minimum = minimum;
}

int32_t CanIntAttribute::GetMaximum(void) const
{
   return this->maximum;
}

void CanIntAttribute::SetMaximum(int32_t maximum)
{
   this->maximum = maximum;
}

int32_t CanIntAttribute::GetDefaultValue(void) const
{
   return this->defaultValue;
}

void CanIntAttribute::SetDefaultValue(int32_t defaultValue)
{
   this->defaultValue = defaultValue;
}