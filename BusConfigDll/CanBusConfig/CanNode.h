#pragma once

#include "ICanNode.h"
#include "CanMessage.h"
#include <string>

class CanNode : public ICanNode
{
public:
   CanNode() = default;
   ~CanNode();

   void Clear(void) override;
   const char* GetName(void) const override;
   void SetName(const char* name);

   size_t GetTxMessagesCount(void) const override;
   ICanMessage* GetTxMessageByIndex(size_t index) const override;
   void AddTxMessage(CanMessage* txMessage);

   size_t GetRxSignalsCount(void) const override;
   ICanSignal* GetRxSignalByIndex(size_t index) const override;
   void AddRxSignal(CanSignal* rxSignal);

private:
   std::string name;
   std::vector<CanMessage*> txMessages;
   std::vector<CanSignal*> rxSignals;
};