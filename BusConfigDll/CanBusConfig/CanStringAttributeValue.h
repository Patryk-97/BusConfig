#pragma once

#include "ICanStringAttributeValue.h"
#include "CanAttributeValue.h"
#include <string>

class CanStringAttributeValue : public CanAttributeValue, public ICanStringAttributeValue
{
public:
   CanStringAttributeValue() = default;
   CanStringAttributeValue(const std::string& value) : value(value) {}
   ~CanStringAttributeValue();

   void Clear(void) override;
   const char * GetValue(void) const override;
   void SetValue(const std::string& value);

private:
   std::string value;
};