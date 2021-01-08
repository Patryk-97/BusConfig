#pragma once

#include "ICanHexAttributeValue.h"
#include "CanAttributeValue.h"

class CanHexAttributeValue : public CanAttributeValue, public ICanHexAttributeValue
{
public:
   CanHexAttributeValue() = default;
   ~CanHexAttributeValue();

   void Clear(void) override;
   int32_t GetValue(void) const override;
   void SetValue(int32_t value);

private:
   int32_t value {};
};