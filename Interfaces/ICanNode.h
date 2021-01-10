#pragma once

#include "ICanMessage.h"
#include "ICanAttributeOwner.h"

class ICanNode : public ICanAttributeOwner
{
protected:
   ICanNode() {};
   virtual ~ICanNode() {};
public:
   virtual void Clear(void) = 0;
   virtual const char* GetName(void) const = 0;
   virtual size_t GetTxMessagesCount(void) const = 0;
   virtual ICanMessage* GetTxMessageByIndex(size_t index) const = 0;
   virtual bool RemoveTxMessageByIndex(size_t index) = 0;
   virtual bool RemoveTxMessageByName(const char* name) = 0;
   virtual bool RemoveTxMessageById(size_t id) = 0;
   virtual void SortTxMessagesByName(void) = 0;
   virtual void SortTxMessagesById(void) = 0;

   virtual size_t GetRxMessagesCount(void) const = 0;
   virtual ICanMessage* GetRxMessageByIndex(size_t index) const = 0;
   virtual bool RemoveRxMessageByIndex(size_t index) = 0;
   virtual bool RemoveRxMessageByName(const char* name) = 0;
   virtual bool RemoveRxMessageById(size_t id) = 0;
   virtual void SortRxMessagesByName(void) = 0;
   virtual void SortRxMessagesById(void) = 0;

   virtual size_t GetMappedTxSignalsCount(void) const = 0;
   virtual ICanSignal* GetMappedTxSignalByIndex(size_t index) const = 0;
   virtual bool RemoveMappedTxSignalByIndex(size_t index) = 0;
   virtual bool RemoveMappedTxSignalByName(const char* name) = 0;
   virtual void SortMappedTxSignalsByName(void) = 0;

   virtual size_t GetMappedRxSignalsCount(void) const = 0;
   virtual ICanSignal* GetMappedRxSignalByIndex(size_t index) const = 0;
   virtual bool RemoveMappedRxSignalByIndex(size_t index) = 0;
   virtual bool RemoveMappedRxSignalByName(const char* name) = 0;
   virtual void SortMappedRxSignalsByName(void) = 0;

   virtual size_t GetAttributesCount(void) const = 0;
   virtual ICanAttribute* GetAttributeByIndex(size_t index) const = 0;
   virtual ICanAttribute* GetAttributeByName(const char* name) const = 0;

   virtual ICanAttributeValue* GetAttributeValue(const char* attributeName) const = 0;

   virtual const char* GetComment(void) const = 0;

   // static variables
   constexpr static const char * PSEUDO_NODE_NAME = "Vector__XXX";
};