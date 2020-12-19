#pragma once

#include "ICanAttributeValue.h"

class ICanStringAttributeValue : public ICanAttributeValue
{
protected:
   ICanStringAttributeValue() {};
   virtual ~ICanStringAttributeValue() {};
public:
   virtual const char * GetValue(void) const = 0;
};