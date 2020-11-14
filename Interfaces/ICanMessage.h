#pragma once

#include "ICanSignal.h"

#include <stdint.h>
#include <iostream>

class ICanMessage
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
   virtual const char* ToString(void) = 0;
};