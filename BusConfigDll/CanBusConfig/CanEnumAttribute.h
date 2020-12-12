#pragma once

#include "ICanEnumAttribute.h"
#include "CanAttribute.h"
#include <vector>

class CanEnumAttribute : public CanAttribute, ICanEnumAttribute
{
public:
   CanEnumAttribute() = default;
   ~CanEnumAttribute();

   void Clear(void) override;

   size_t GetEnumaratorsCount(void) const override;
   const char* GetEnumarator(size_t index) const override;
   void AddEnumarator(const char * enumarator);

   const char* GetDefaultValue(void) const override;
   void SetDefaultValue(const char* defaultValue);

private:
   std::vector<std::string> enumerators;
   std::string defaultValue{};
};