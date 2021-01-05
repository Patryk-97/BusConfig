#pragma once

#include "ICanEnumAttribute.h"
#include "CanAttribute.h"
#include <vector>

class CanEnumAttribute : public CanAttribute, public ICanEnumAttribute
{
public:
   CanEnumAttribute() : CanAttribute { IValueType_e::ENUM } {}
   ~CanEnumAttribute();

   void Clear(void) override;

   const char* GetName(void) const override;
   IValueType_e GetValueType(void) const override;
   IObjectType_e GetObjectType(void) const override;

   size_t GetEnumaratorsCount(void) const override;
   const char* GetEnumarator(size_t index) const override;
   void AddEnumarator(const char * enumarator);

   const char* GetDefaultValue(void) const override;
   void SetDefaultValue(const char* defaultValue);

private:
   std::vector<std::string> enumerators;
   std::string defaultValue{};
};