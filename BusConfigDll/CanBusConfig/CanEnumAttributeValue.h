#pragma once

#include "ICanEnumAttributeValue.h"
#include <string>

class CanEnumAttributeValue : public ICanEnumAttributeValue
{
public:
   CanEnumAttributeValue() = default;
   CanEnumAttributeValue(const std::string& value) : value(value) {}
   ~CanEnumAttributeValue();

   void Clear(void) override;
   const char * GetValue(void) const override;
   void SetValue(const std::string& value);

private:
   std::string value;
};