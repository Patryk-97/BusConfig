#pragma once

#include "ICanAttribute.h"
#include <string>

class CanAttribute : public ICanAttribute
{
public:
   CanAttribute(IValueType_e valueType) : valueType { valueType } {}
   ~CanAttribute();

   void Clear(void) override;

   const char* GetName(void) const override;
   void SetName(const char * name);

   IValueType_e GetValueType(void) const override;

   IObjectType_e GetObjectType(void) const override;
   void SetObjectType(IObjectType_e objectType);

private:
   std::string name;
   IValueType_e valueType { IValueType_e::UNDEFINED };
   IObjectType_e objectType { IObjectType_e::UNDEFINED };
};