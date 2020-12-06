#include "CanNode.h"

CanNode::~CanNode()
{
   this->Clear();
}

void CanNode::Clear(void)
{
   this->name = "";
   this->txMessages.clear();
   this->rxMessages.clear();
   this->mappedRxSignals.clear();
   this->mappedTxSignals.clear();
}

const char* CanNode::GetName(void) const
{
   return this->name.c_str();
}

void CanNode::SetName(const char* name)
{
   this->name = name;
}

size_t CanNode::GetTxMessagesCount(void) const
{
   return this->txMessages.size();
}

ICanMessage* CanNode::GetTxMessageByIndex(size_t index) const
{
   return (index < this->txMessages.size() ? *std::next(this->txMessages.begin(), index) : nullptr);
}

void CanNode::AddTxMessage(CanMessage* txMessage)
{
   if (txMessage)
   {
      this->txMessages.insert(txMessage);
   }
}

size_t CanNode::GetRxMessagesCount(void) const
{
   return this->rxMessages.size();
}

ICanMessage* CanNode::GetRxMessageByIndex(size_t index) const
{
   return (index < this->rxMessages.size() ? *std::next(this->rxMessages.begin(), index) : nullptr);
}

void CanNode::AddRxMessage(CanMessage* rxMessage)
{
   if (rxMessage != nullptr)
   {
      this->rxMessages.insert(rxMessage);
   }
}

size_t CanNode::GetMappedTxSignalsCount(void) const
{
   return this->mappedTxSignals.size();
}

ICanSignal* CanNode::GetMappedTxSignalByIndex(size_t index) const
{
   return (index < this->mappedTxSignals.size() ? *std::next(this->mappedTxSignals.begin(), index) : nullptr);
}

void CanNode::AddMappedTxSignal(CanSignal* mappedTxSignal)
{
   if (mappedTxSignal != nullptr)
   {
      this->mappedTxSignals.insert(mappedTxSignal);
   }
}

size_t CanNode::GetMappedRxSignalsCount(void) const
{
   return this->mappedRxSignals.size();
}

ICanSignal* CanNode::GetMappedRxSignalByIndex(size_t index) const
{
   return (index < this->mappedRxSignals.size() ? *std::next(this->mappedRxSignals.begin(), index) : nullptr);
}

void CanNode::AddMappedRxSignal(CanSignal* mappedRxSignal)
{
   if (mappedRxSignal)
   {
      this->mappedRxSignals.insert(mappedRxSignal);
   }
}