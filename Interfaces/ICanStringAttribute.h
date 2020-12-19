#pragma once

#include "ICanAttribute.h"

class ICanStringAttribute : public ICanAttribute
{
protected:
   ICanStringAttribute() {};
   virtual ~ICanStringAttribute() {};
public:
   virtual void Clear(void) = 0;
   virtual const char* GetDefaultValue(void) const = 0;
};