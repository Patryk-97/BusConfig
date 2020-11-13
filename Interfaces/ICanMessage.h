#pragma once

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
   virtual const char* ToString(void) = 0;
};