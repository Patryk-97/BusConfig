#pragma once

#include "ICanSignal.h"

#include <stdint.h>
#include <iostream>

class ICanMessage : public ICanAttributeOwner
{
protected:
   ICanMessage() {};
   virtual ~ICanMessage() {};
public:
   virtual void Clear(void) = 0;
   virtual uint32_t GetId(void) const = 0;
   virtual const char* GetName(void) const = 0;
   virtual uint32_t GetSize(void) const = 0;
   virtual const char* GetMainTransmitter(void) const = 0;

   virtual size_t GetSignalsCount(void) const = 0;
   virtual ICanSignal* GetSignalByName(const char* name) const = 0;
   virtual ICanSignal* GetSignalByIndex(size_t index) const = 0;

   virtual size_t GetAttributesCount(void) const = 0;
   virtual ICanAttribute* GetAttributeByIndex(size_t index) const = 0;
   virtual ICanAttribute* GetAttributeByName(const char* name) const = 0;

   virtual ICanAttributeValue* GetAttributeValue(const char* attributeName) const = 0;

   virtual const char* ToString(void) = 0;
};