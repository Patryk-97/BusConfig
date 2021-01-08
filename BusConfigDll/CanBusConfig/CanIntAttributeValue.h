#pragma once

#include "ICanIntAttributeValue.h"
#include "CanAttributeValue.h"

class CanIntAttributeValue : public CanAttributeValue, public ICanIntAttributeValue
{
public:
   CanIntAttributeValue() = default;
   CanIntAttributeValue(int32_t value): value(value) {}
   ~CanIntAttributeValue();

   void Clear(void) override;
   int32_t GetValue(void) const override;
   void SetValue(int32_t value);

private:
   int32_t value {};
};