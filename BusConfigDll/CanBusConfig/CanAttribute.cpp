#include "CanAttribute.h"

CanAttribute::~CanAttribute()
{
   this->Clear();
}

void CanAttribute::Clear(void)
{
   this->name = "";
   this->valueType = IValueType_e::UNDEFINED;
   this->objectType = IObjectType_e::UNDEFINED;
}

const char * CanAttribute::GetName(void) const
{
   return this->name.c_str();
}

void CanAttribute::SetName(const char* name)
{
   this->name = name;
}

ICanAttribute::IValueType_e CanAttribute::GetValueType(void) const
{
   return this->valueType;
}

void CanAttribute::SetValueType(IValueType_e valueType)
{
   this->valueType = valueType;
}

ICanAttribute::IObjectType_e CanAttribute::GetObjectType(void) const
{
   return this->objectType;
}

void CanAttribute::SetObjectType(IObjectType_e objectType)
{
   this->objectType = objectType;
}