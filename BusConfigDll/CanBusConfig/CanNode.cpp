#include "CanNode.h"
#include <algorithm>

namespace ranges = std::ranges;

CanNode::~CanNode()
{
   this->Clear();
}

void CanNode::Clear(void)
{
   CanAttributeOwner::Clear();
   this->name.clear();
   this->txMessages.clear();
   this->rxMessages.clear();
   this->mappedRxSignals.clear();
   this->mappedTxSignals.clear();
   this->comment.clear();
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

size_t CanNode::GetAttributesCount(void) const
{
   return CanAttributeOwner::GetAttributesCount();
}

ICanAttribute* CanNode::GetAttributeByIndex(size_t index) const
{
   return CanAttributeOwner::GetAttributeByIndex(index);
}

ICanAttribute* CanNode::GetAttributeByName(const char* name) const
{
   return CanAttributeOwner::GetAttributeByName(name);
}

size_t CanNode::GetAttributesValuesCount(void) const
{
   return CanAttributeOwner::GetAttributesValuesCount();
}

ICanAttributeValue* CanNode::GetAttributeValue(const char* attributeName) const
{
   return CanAttributeOwner::GetAttributeValue(attributeName);
}

const char* CanNode::GetComment(void) const
{
   return this->comment.c_str();
}

void CanNode::SetComment(const char* comment)
{
   this->comment = comment;
}