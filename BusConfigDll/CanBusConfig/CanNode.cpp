#include "CanNode.h"

CanNode::~CanNode()
{
   this->Clear();
}

void CanNode::Clear(void)
{
   this->name = "";
   this->txMessages.clear();
   this->rxSignals.clear();
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
   return (index < this->txMessages.size() ? this->txMessages[index] : nullptr);
}

void CanNode::AddTxMessage(CanMessage* txMessage)
{
   if (txMessage)
   {
      this->txMessages.push_back(txMessage);
   }
}

size_t CanNode::GetRxSignalsCount(void) const
{
   return this->rxSignals.size();
}

ICanSignal* CanNode::GetRxSignalByIndex(size_t index) const
{
   return (index < this->rxSignals.size() ? this->rxSignals[index] : nullptr);
}

void CanNode::AddRxSignal(CanSignal* rxSignal)
{
   if (rxSignal)
   {
      this->rxSignals.push_back(rxSignal);
   }
}