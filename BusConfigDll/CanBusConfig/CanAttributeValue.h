#pragma once

#include "ICanAttributeValue.h"

class CanAttributeValue : public ICanAttributeValue
{
public:
   CanAttributeValue() = default;
   ~CanAttributeValue();

   void Clear(void) override;
};