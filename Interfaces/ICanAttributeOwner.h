#pragma once

#include "ICanAttribute.h"
#include "ICanAttributeValue.h"
#include <stdint.h>

class ICanAttributeOwner
{
protected:
   ICanAttributeOwner() {};
   virtual ~ICanAttributeOwner() {};
public:
   virtual void Clear(void) = 0;
   virtual size_t GetAttributesCount(void) const = 0;
   virtual ICanAttribute* GetAttributeByIndex(size_t index) const = 0;
   virtual ICanAttribute* GetAttributeByName(const char* name) const = 0;

   virtual size_t GetAttributesValuesCount(void) const = 0;
   virtual ICanAttributeValue* GetAttributeValue(const char* attributeName) const = 0;
};