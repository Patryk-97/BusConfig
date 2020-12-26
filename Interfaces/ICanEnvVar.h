#pragma once

#include "ICanAttributeOwner.h"
#include "ICanValueTable.h"

class ICanEnvVar : public ICanAttributeOwner
{
protected:
   ICanEnvVar() {};
   virtual ~ICanEnvVar() {};
public:
   virtual void Clear(void) = 0;
   virtual const char * GetName(void) const = 0;

   virtual ICanValueTable* GetValueTable(void) const = 0;

   virtual size_t GetAttributesCount(void) const = 0;
   virtual ICanAttribute* GetAttributeByIndex(size_t index) const = 0;
   virtual ICanAttribute* GetAttributeByName(const char* name) const = 0;

   virtual ICanAttributeValue* GetAttributeValue(const char* attributeName) const = 0;
};