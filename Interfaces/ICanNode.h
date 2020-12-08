#pragma once

#include "ICanMessage.h"

class ICanNode
{
protected:
   ICanNode() {};
   virtual ~ICanNode() {};
public:
   virtual void Clear(void) = 0;
   virtual const char* GetName(void) const = 0;
   virtual size_t GetTxMessagesCount(void) const = 0;
   virtual ICanMessage* GetTxMessageByIndex(size_t index) const = 0;
   virtual size_t GetRxMessagesCount(void) const = 0;
   virtual ICanMessage* GetRxMessageByIndex(size_t index) const = 0;
   virtual size_t GetMappedTxSignalsCount(void) const = 0;
   virtual ICanSignal* GetMappedTxSignalByIndex(size_t index) const = 0;
   virtual size_t GetMappedRxSignalsCount(void) const = 0;
   virtual ICanSignal* GetMappedRxSignalByIndex(size_t index) const = 0;

   // static variables
   constexpr static const char * PSEUDO_NODE_NAME = "Vector__XXX";
};