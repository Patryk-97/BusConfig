#pragma once

#include "ICanNode.h"
#include "CanMessage.h"
#include <string>
#include <vector>

class CanNetwork; // circular dependency

class CanNode : public CanAttributeOwner, public ICanNode
{
public:
   CanNode() : CanAttributeOwner(ICanAttribute::IObjectType_e::NODE) {}
   ~CanNode();

   void Clear(void) override;

   const char* GetName(void) const override;
   void ModifyName(const char* name) override;
   void SetName(const char* name);

   int32_t GetAddress(void) const override;
   void ModifyAddress(int32_t address) override;
   void SetAddress(int32_t address);

   size_t GetTxMessagesCount(void) const override;
   ICanMessage* GetTxMessageByIndex(size_t index) const override;
   bool RemoveTxMessageByIndex(size_t index) override;
   bool RemoveTxMessageByName(const char* name) override;
   bool RemoveTxMessageById(size_t id) override;
   void SortTxMessagesByName(void) override;
   void SortTxMessagesById(void) override;
   void AddTxMessage(CanMessage* txMessage);

   size_t GetRxMessagesCount(void) const override;
   ICanMessage* GetRxMessageByIndex(size_t index) const override;
   bool RemoveRxMessageByIndex(size_t index) override;
   bool RemoveRxMessageByName(const char* name) override;
   bool RemoveRxMessageById(size_t id) override;
   void SortRxMessagesByName(void) override;
   void SortRxMessagesById(void) override;
   void AddRxMessage(CanMessage* rxMessage);

   size_t GetMappedTxSignalsCount(void) const override;
   ICanSignal* GetMappedTxSignalByIndex(size_t index) const override;
   bool RemoveMappedTxSignalByIndex(size_t index) override;
   bool RemoveMappedTxSignalByName(const char* name) override;
   void SortMappedTxSignalsByName(void) override;
   void AddMappedTxSignal(CanSignal* mappedTxSignal);

   size_t GetMappedRxSignalsCount(void) const override;
   ICanSignal* GetMappedRxSignalByIndex(size_t index) const override;
   bool RemoveMappedRxSignalByIndex(size_t index) override;
   bool RemoveMappedRxSignalByName(const char* name) override;
   void SortMappedRxSignalsByName(void) override;
   void AddMappedRxSignal(CanSignal* mappedRxSignal);

   size_t GetAttributesCount(void) const override;
   ICanAttribute* GetAttributeByIndex(size_t index) const override;
   ICanAttribute* GetAttributeByName(const char* name) const override;

   size_t GetAttributesValuesCount(void) const override;
   ICanAttributeValue* GetAttributeValue(const char* attributeName) const override;

   const char* GetComment(void) const override;
   void ModifyComment(const char* comment) override;
   void SetComment(const char* comment);

   ICanNetwork* GetNetwork(void) const override;
   void SetNetwork(CanNetwork* network);

   void SetMainAttributes(void);

private:

   std::string name;
   int32_t address {};
   std::vector<CanMessage*> txMessages;
   std::vector<CanMessage*> rxMessages;
   std::vector<CanSignal*> mappedTxSignals;
   std::vector<CanSignal*> mappedRxSignals;
   std::string comment;
   CanNetwork* network { nullptr };
};