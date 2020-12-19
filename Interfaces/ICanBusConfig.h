#pragma once

#include "ICanMessage.h"
#include "ICanNode.h"
#include "ICanEnvVar.h"
#include "ICanAttributeOwner.h"

// Classes
class ICanBusConfig;

// Dll functions
ICanBusConfig* CanBusConfigInstanceCreate(void);
bool CanBusConfigInstanceDelete(ICanBusConfig* canBusConfig);

class ICanBusConfig : public ICanAttributeOwner
{
protected:
   ICanBusConfig() {};
   virtual ~ICanBusConfig() {};
public:
   virtual void Clear(void) = 0;
   virtual const char* GetLog(void) const = 0;
   virtual bool Load(const char* filename) = 0;

   virtual size_t GetNodesCount(void) const = 0;
   virtual ICanNode* GetNodeByIndex(size_t index) const = 0;
   virtual ICanNode* GetNodeByName(const char* name) const = 0;

   virtual size_t GetMessagesCount(void) const = 0;
   virtual ICanMessage* GetMessageById(uint32_t id) const = 0;
   virtual ICanMessage* GetMessageByName(const char* name) const = 0;
   virtual ICanMessage* GetMessageByIndex(size_t index) const = 0;
   virtual ICanMessage* GetMessageFront(void) const = 0;
   virtual ICanMessage* GetMessageBack(void) const = 0;

   virtual size_t GetSignalsCount(void) const = 0;
   virtual ICanSignal* GetSignalByIndex(size_t index) const = 0;
   virtual ICanSignal* GetSignalByName(const char* name) const = 0;

   virtual size_t GetEnvVarsCount(void) const = 0;
   virtual ICanEnvVar* GetEnvVarByIndex(size_t index) const = 0;
   virtual ICanEnvVar* GetEnvVarByName(const char* name) const = 0;
};