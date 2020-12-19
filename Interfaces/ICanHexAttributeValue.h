#pragma once

#include "ICanAttributeValue.h"
#include <stdint.h>

class ICanHexAttributeValue : public ICanAttributeValue
{
protected:
   ICanHexAttributeValue() {};
   virtual ~ICanHexAttributeValue() {};
public:
   virtual int32_t GetValue(void) const = 0;
};