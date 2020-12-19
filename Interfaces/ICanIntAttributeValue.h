#pragma once

#include "ICanAttributeValue.h"
#include <stdint.h>

class ICanIntAttributeValue : public ICanAttributeValue
{
protected:
   ICanIntAttributeValue() {};
   virtual ~ICanIntAttributeValue() {};
public:
   virtual int32_t GetValue(void) const = 0;
};