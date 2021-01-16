#include "CanNode.h"
#include <algorithm>
#include "CanNetwork.h" // circular dependency

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
   this->network = nullptr;
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

bool CanNode::RemoveTxMessageByIndex(size_t index)
{
   if (index < this->txMessages.size())
   {
      auto txMessage = this->txMessages[index];
      for (size_t i = 0; i < this->mappedTxSignals.size(); ++i)
      {
         if (auto txMessage2 = this->mappedTxSignals[i]->GetMessage(); txMessage2)
         {
            if (txMessage2->GetId() == txMessage->GetId())
            {
               this->mappedTxSignals.erase(this->mappedTxSignals.begin() + i);
               --i;
            }
         }
      }
      this->txMessages.erase(this->txMessages.begin() + index);
      return true;
   }
   else
   {
      return false;
   }
}

bool CanNode::RemoveTxMessageByName(const char* name)
{
   auto it = ranges::find_if(this->txMessages, [&name] (CanMessage* txMessage) { return !std::strcmp(txMessage->GetName(), name); });
   if (it != this->txMessages.end())
   {
      for (size_t i = 0; i < this->mappedTxSignals.size(); ++i)
      {
         if (auto txMessage = this->mappedTxSignals[i]->GetMessage(); txMessage)
         {
            if (!std::strcmp(txMessage->GetName(), name))
            {
               this->mappedTxSignals.erase(this->mappedTxSignals.begin() + i);
               --i;
            }
         }
      }

      this->txMessages.erase(it);
      return true;
   }
   else
   {
      return false;
   }
}

bool CanNode::RemoveTxMessageById(size_t id)
{
   auto it = ranges::find_if(this->txMessages, [&id] (CanMessage* txMessage) { return txMessage->GetId() == id; });
   if (it != this->txMessages.end())
   {
      for (size_t i = 0; i < this->mappedTxSignals.size(); ++i)
      {
         if (auto txMessage = this->mappedTxSignals[i]->GetMessage(); txMessage)
         {
            if (txMessage->GetId() == id)
            {
               this->mappedTxSignals.erase(this->mappedTxSignals.begin() + i);
               --i;
            }
         }
      }

      this->txMessages.erase(it);
      return true;
   }
   else
   {
      return false;
   }
}

void CanNode::SortTxMessagesByName(void)
{
   ranges::sort(this->txMessages, [] (const CanMessage* txMessage1, const CanMessage* txMessage2)
      { return std::strcmp(txMessage1->GetName(), txMessage2->GetName()) < 0; });
}

void CanNode::SortTxMessagesById(void)
{
   ranges::sort(this->txMessages, [] (const CanMessage* txMessage1, const CanMessage* txMessage2)
      { return txMessage1->GetId() < txMessage2->GetId(); });
}

void CanNode::AddTxMessage(CanMessage* txMessage)
{
   if (txMessage)
   {
      if (this->txMessages.size() > 0)
      {
         if (this->txMessages.back()->GetId() != txMessage->GetId())
         {
            this->txMessages.push_back(txMessage);
         }
      }
      else
      {
         this->txMessages.push_back(txMessage);
      }
   }
}

size_t CanNode::GetRxMessagesCount(void) const
{
   return this->rxMessages.size();
}

ICanMessage* CanNode::GetRxMessageByIndex(size_t index) const
{
   return (index < this->rxMessages.size() ? this->rxMessages[index] : nullptr);
}

bool CanNode::RemoveRxMessageByIndex(size_t index)
{
   if (index < this->rxMessages.size())
   {
      auto rxMessage = this->rxMessages[index];
      for (size_t i = 0; i < this->mappedRxSignals.size(); ++i)
      {
         if (auto rxMessage2 = this->mappedRxSignals[i]->GetMessage(); rxMessage2)
         {
            if (rxMessage2->GetId() == rxMessage->GetId())
            {
               this->mappedRxSignals.erase(this->mappedRxSignals.begin() + i);
               --i;
            }
         }
      }
      this->rxMessages.erase(this->rxMessages.begin() + index);
      return true;
   }
   else
   {
      return false;
   }
}

bool CanNode::RemoveRxMessageByName(const char* name)
{
   auto it = ranges::find_if(this->rxMessages, [&name] (CanMessage* rxMessage) { return !std::strcmp(rxMessage->GetName(), name); });
   if (it != this->rxMessages.end())
   {
      for (size_t i = 0; i < this->mappedRxSignals.size(); ++i)
      {
         if (auto rxMessage = this->mappedRxSignals[i]->GetMessage(); rxMessage)
         {
            if (!std::strcmp(rxMessage->GetName(), name))
            {
               this->mappedRxSignals.erase(this->mappedRxSignals.begin() + i);
               --i;
            }
         }
      }

      this->rxMessages.erase(it);
      return true;
   }
   else
   {
      return false;
   }
}

bool CanNode::RemoveRxMessageById(size_t id)
{
   auto it = ranges::find_if(this->rxMessages, [&id] (CanMessage* rxMessage) { return rxMessage->GetId() == id; });
   if (it != this->rxMessages.end())
   {
      for (size_t i = 0; i < this->mappedRxSignals.size(); ++i)
      {
         if (auto rxMessage = this->mappedRxSignals[i]->GetMessage(); rxMessage)
         {
            if (rxMessage->GetId() == id)
            {
               this->mappedRxSignals.erase(this->mappedRxSignals.begin() + i);
               --i;
            }
         }
      }

      this->rxMessages.erase(it);
      return true;
   }
   else
   {
      return false;
   }
}

void CanNode::SortRxMessagesByName(void)
{
   ranges::sort(this->rxMessages, [] (const CanMessage* rxMessage1, const CanMessage* rxMessage2)
      { return std::strcmp(rxMessage1->GetName(), rxMessage2->GetName()) < 0; });
}

void CanNode::SortRxMessagesById(void)
{
   ranges::sort(this->rxMessages, [] (const CanMessage* rxMessage1, const CanMessage* rxMessage2)
      { return rxMessage1->GetId() < rxMessage2->GetId(); });
}

void CanNode::AddRxMessage(CanMessage* rxMessage)
{
   if (rxMessage)
   {
      if (this->rxMessages.size() > 0)
      {
         if (this->rxMessages.back()->GetId() != rxMessage->GetId())
         {
            this->rxMessages.push_back(rxMessage);
         }
      }
      else
      {
         this->rxMessages.push_back(rxMessage);
      }
   }
}

size_t CanNode::GetMappedTxSignalsCount(void) const
{
   return this->mappedTxSignals.size();
}

ICanSignal* CanNode::GetMappedTxSignalByIndex(size_t index) const
{
   return (index < this->mappedTxSignals.size() ? this->mappedTxSignals[index] : nullptr);
}

bool CanNode::RemoveMappedTxSignalByIndex(size_t index)
{
   if (index < this->mappedTxSignals.size())
   {
      if (auto txMessage = this->mappedTxSignals[index]->GetMessage(); txMessage)
      {
         this->mappedTxSignals.erase(this->mappedTxSignals.begin() + index);
         bool anySignal { false };

         for (size_t i = 0; i < this->mappedTxSignals.size(); ++i)
         {
            if (auto txMessage2 = this->mappedTxSignals[i]->GetMessage(); txMessage2)
            {
               if (txMessage->GetId() == txMessage2->GetId())
               {
                  anySignal = true;
                  break;
               }
            }
         }

         if (!anySignal)
         {
            std::erase_if(this->txMessages, [id = txMessage->GetId()] (CanMessage* txMessage)
               { return txMessage->GetId() == id; });
         }
      }
      else
      {
         this->mappedTxSignals.erase(this->mappedTxSignals.begin() + index);
      }
      return true;
   }
   else
   {
      return false;
   }
}

bool CanNode::RemoveMappedTxSignalByName(const char* name)
{
   auto it = ranges::find_if(this->mappedTxSignals, [&name] (CanSignal* mappedTxSignal)
      { return !std::strcmp(mappedTxSignal->GetName(), name); });

   if (it != this->mappedTxSignals.end() && *it)
   {
      if (auto txMessage = (*it)->GetMessage(); txMessage)
      {
         this->mappedTxSignals.erase(it);
         bool anySignal { false };

         for (size_t i = 0; i < this->mappedTxSignals.size(); ++i)
         {
            if (auto txMessage2 = this->mappedTxSignals[i]->GetMessage(); txMessage2)
            {
               if (txMessage->GetId() == txMessage2->GetId())
               {
                  anySignal = true;
                  break;
               }
            }
         }

         if (!anySignal)
         {
            std::erase_if(this->txMessages, [id = txMessage->GetId()] (CanMessage* txMessage)
               { return txMessage->GetId() == id; });
         }
      }
      else
      {
         this->mappedTxSignals.erase(it);
      }
      return true;
   }
   else
   {
      return false;
   }
}

void CanNode::SortMappedTxSignalsByName(void)
{
   ranges::sort(this->mappedTxSignals, [] (const CanSignal* mappedTxSignal1, const CanSignal* mappedTxSignal2)
      { return std::strcmp(mappedTxSignal1->GetName(), mappedTxSignal2->GetName()) < 0; });
}

void CanNode::AddMappedTxSignal(CanSignal* mappedTxSignal)
{
   if (mappedTxSignal != nullptr)
   {
      this->mappedTxSignals.push_back(mappedTxSignal);
   }
}

size_t CanNode::GetMappedRxSignalsCount(void) const
{
   return this->mappedRxSignals.size();
}

ICanSignal* CanNode::GetMappedRxSignalByIndex(size_t index) const
{
   return (index < this->mappedRxSignals.size() ? this->mappedRxSignals[index] : nullptr);
}

bool CanNode::RemoveMappedRxSignalByIndex(size_t index)
{
   if (index < this->mappedRxSignals.size())
   {
      if (auto rxMessage = this->mappedRxSignals[index]->GetMessage(); rxMessage)
      {
         this->mappedRxSignals.erase(this->mappedRxSignals.begin() + index);
         bool anySignal{ false };

         for (size_t i = 0; i < this->mappedRxSignals.size(); ++i)
         {
            if (auto rxMessage2 = this->mappedRxSignals[i]->GetMessage(); rxMessage2)
            {
               if (rxMessage->GetId() == rxMessage2->GetId())
               {
                  anySignal = true;
                  break;
               }
            }
         }

         if (!anySignal)
         {
            std::erase_if(this->rxMessages, [id = rxMessage->GetId()] (CanMessage* rxMessage)
               { return rxMessage->GetId() == id; });
         }
      }
      else
      {
         this->mappedRxSignals.erase(this->mappedRxSignals.begin() + index);
      }
      return true;
   }
   else
   {
      return false;
   }
}

bool CanNode::RemoveMappedRxSignalByName(const char* name)
{
   auto it = ranges::find_if(this->mappedRxSignals, [&name] (CanSignal* mappedRxSignal)
      { return !std::strcmp(mappedRxSignal->GetName(), name); });

   if (it != this->mappedRxSignals.end() && *it)
   {
      if (auto rxMessage = (*it)->GetMessage(); rxMessage)
      {
         this->mappedRxSignals.erase(it);
         bool anySignal{ false };

         for (size_t i = 0; i < this->mappedRxSignals.size(); ++i)
         {
            if (auto rxMessage2 = this->mappedRxSignals[i]->GetMessage(); rxMessage2)
            {
               if (rxMessage->GetId() == rxMessage2->GetId())
               {
                  anySignal = true;
                  break;
               }
            }
         }

         if (!anySignal)
         {
            std::erase_if(this->rxMessages, [id = rxMessage->GetId()] (CanMessage* rxMessage)
            { return rxMessage->GetId() == id; });
         }
      }
      else
      {
         this->mappedRxSignals.erase(it);
      }
      return true;
   }
   else
   {
      return false;
   }
}

void CanNode::SortMappedRxSignalsByName(void)
{
   ranges::sort(this->mappedRxSignals, [] (const CanSignal* mappedRxSignal1, const CanSignal* mappedRxSignal2)
      { return std::strcmp(mappedRxSignal1->GetName(), mappedRxSignal2->GetName()) < 0; });
}

void CanNode::AddMappedRxSignal(CanSignal* mappedRxSignal)
{
   if (mappedRxSignal)
   {
      this->mappedRxSignals.push_back(mappedRxSignal);
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

ICanNetwork* CanNode::GetNetwork(void) const
{
   return this->network;
}

void CanNode::SetNetwork(CanNetwork* network)
{
   this->network = network;
}