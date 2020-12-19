#pragma once

#include "ICanAttributeValue.h"

class ICanEnumAttributeValue : public ICanAttributeValue
{
protected:
   ICanEnumAttributeValue() {};
   virtual ~ICanEnumAttributeValue() {};
public:
   virtual const char * GetValue(void) const = 0;
};