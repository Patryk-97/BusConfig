#pragma once

#include "ICanStringAttribute.h"
#include "CanAttribute.h"

class CanStringAttribute : public CanAttribute, public ICanStringAttribute
{
public:
   CanStringAttribute() : CanAttribute { IValueType_e::STRING } {}
   ~CanStringAttribute();

   void Clear(void) override;

   const char* GetName(void) const override;
   IValueType_e GetValueType(void) const override;
   IObjectType_e GetObjectType(void) const override;

   const char* GetDefaultValue(void) const override;
   void SetDefaultValue(const char* defaultValue);

private:
   std::string defaultValue{};
};