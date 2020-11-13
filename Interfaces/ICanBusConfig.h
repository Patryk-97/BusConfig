#pragma once

#include "ICanMessage.h"

// Classes
class ICanBusConfig;

// Dll functions
ICanBusConfig* CanBusConfigInstanceCreate(void);
bool CanBusConfigInstanceDelete(ICanBusConfig* canBusConfig);

class ICanBusConfig
{
protected:
   ICanBusConfig() {};
   virtual ~ICanBusConfig() {};
public:
   virtual void Clear(void) = 0;
   virtual const char* GetLog(void) const = 0;
   virtual bool Load(const char* filename) = 0;
   virtual size_t GetMessagesCount(void) const = 0;
   virtual ICanMessage* GetMessageById(uint32_t id) const = 0;
   virtual ICanMessage* GetMessageByName(const char* name) const = 0;
   virtual ICanMessage* GetMessageByIndex(size_t index) const = 0;
};