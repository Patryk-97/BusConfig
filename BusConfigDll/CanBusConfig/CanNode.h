#pragma once

#include "ICanNode.h"
#include "CanMessage.h"
#include <string>
#include <set>

class CanNode : public CanAttributeOwner, public ICanNode
{
public:
   CanNode() : CanAttributeOwner(ICanAttribute::IObjectType_e::NODE) {}
   ~CanNode();

   void Clear(void) override;
   const char* GetName(void) const override;
   void SetName(const char* name);

   size_t GetTxMessagesCount(void) const override;
   ICanMessage* GetTxMessageByIndex(size_t index) const override;
   void AddTxMessage(CanMessage* txMessage);

   size_t GetRxMessagesCount(void) const override;
   ICanMessage* GetRxMessageByIndex(size_t index) const override;
   void AddRxMessage(CanMessage* rxMessage);

   size_t GetMappedTxSignalsCount(void) const override;
   ICanSignal* GetMappedTxSignalByIndex(size_t index) const override;
   void AddMappedTxSignal(CanSignal* mappedTxSignal);

   size_t GetMappedRxSignalsCount(void) const override;
   ICanSignal* GetMappedRxSignalByIndex(size_t index) const override;
   void AddMappedRxSignal(CanSignal* mappedRxSignal);

   size_t GetAttributesCount(void) const override;
   ICanAttribute* GetAttributeByIndex(size_t index) const override;
   ICanAttribute* GetAttributeByName(const char* name) const override;

   ICanAttributeValue* GetAttributeValue(const char* attributeName) const override;

private:

   template <typename T>
   struct Comparator
   {
      bool operator() (const T* t1, const T* t2) const
      {
         return std::strcmp(t1->GetName(), t2->GetName()) < 0;
      }
   };

   std::string name;
   std::set<CanMessage*, Comparator<CanMessage>> txMessages;
   std::set<CanMessage*, Comparator<CanMessage>> rxMessages;
   std::set<CanSignal*, Comparator<CanSignal>> mappedTxSignals;
   std::set<CanSignal*, Comparator<CanSignal>> mappedRxSignals;
};