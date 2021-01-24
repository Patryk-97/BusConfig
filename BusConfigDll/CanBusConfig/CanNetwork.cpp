#include "CanNetwork.h"
#include "helpers.h"
#include "ICanAttributeManager.h"
#include <algorithm>

namespace ranges = std::ranges;

CanNetwork::~CanNetwork()
{
   this->Clear();
}

void CanNetwork::Clear(void)
{
   CanAttributeOwner::Clear();
   this->name.clear();
   this->protocol.clear();
   helpers::ClearContainer(this->messages);
   helpers::ClearContainer(this->nodes);
   helpers::ClearContainer(this->signals);
   helpers::ClearContainer(this->envVars);
   this->comment.clear();
}

const char* CanNetwork::GetName(void) const
{
   return this->name.c_str();
}

void CanNetwork::ModifyName(const char* name)
{
   this->name = name;
}

void CanNetwork::SetName(const char* name)
{
   this->name = name;
}

const char* CanNetwork::GetProtocol(void) const
{
   return this->protocol.c_str();
}

void CanNetwork::ModifyProtocol(const char* protocol)
{
   this->protocol = protocol;
}

void CanNetwork::SetProtocol(const char* protocol)
{
   this->protocol = protocol;
}

size_t CanNetwork::GetNodesCount(void) const
{
   return this->nodes.size();
}

ICanNode* CanNetwork::GetNodeByIndex(size_t index) const
{
   return (index < this->nodes.size() ? this->nodes[index] : nullptr);
}

ICanNode* CanNetwork::GetNodeByName(const char* name) const
{
   auto it = ranges::find_if(this->nodes, [&name](CanNode* node) { return !std::strcmp(node->GetName(), name); });
   return (it != this->nodes.end() ? *it : nullptr);
}

std::vector<CanNode*> CanNetwork::GetNodes(void)
{
   return this->nodes;
}

size_t CanNetwork::GetNodeIndex(const char* name) const
{
   auto it = ranges::find_if(this->nodes, [&name](CanNode* node) { return !std::strcmp(node->GetName(), name); });
   return (it != this->nodes.end() ? std::distance(this->nodes.begin(), it) :
      ICanNetwork::INVALID_INDEX);
}

bool CanNetwork::RemoveNodeByIndex(size_t index)
{
   if (index < this->nodes.size())
   {
      helpers::ClearPtr(this->nodes[index]);
      this->nodes.erase(this->nodes.begin() + index);
      return true;
   }
   else
   {
      return false;
   }
}

bool CanNetwork::RemoveNodeByName(const char* name)
{
   auto it = ranges::find_if(this->nodes, [&name](CanNode* node)
      {
         return !std::strcmp(node->GetName(), name);
      });

   if (it != this->nodes.end())
   {
      helpers::ClearPtr(*it);
      this->nodes.erase(it);
      return true;
   }
   else
   {
      return false;
   }
}

void CanNetwork::AddNode(CanNode* node)
{
   if (node)
   {
      this->nodes.push_back(node);
   }
}

CanNode* CanNetwork::CreateAndAddNode(void)
{
   CanNode* node = new CanNode{};
   this->nodes.push_back(node);
   return node;
}

size_t CanNetwork::GetMessagesCount(void) const
{
   return this->messages.size();
}

ICanMessage* CanNetwork::GetMessageById(uint32_t id) const
{
   auto it = ranges::find_if(this->messages, [&id](CanMessage* message) { return message->GetId() == id; });
   return (it != this->messages.end() ? *it : nullptr);
}

ICanMessage* CanNetwork::GetMessageByName(const char* name) const
{
   auto it = ranges::find_if(this->messages, [&name](CanMessage* message) { return !std::strcmp(message->GetName(), name); });
   return (it != this->messages.end() ? *it : nullptr);
}

ICanMessage* CanNetwork::GetMessageByIndex(size_t index) const
{
   return (index < this->messages.size() ? this->messages[index] : nullptr);
}

std::vector<CanMessage*> CanNetwork::GetMessages(void)
{
   return this->messages;
}

ICanMessage* CanNetwork::GetMessageFront(void) const
{
   return (this->messages.size() > 0 ? this->messages.front() : nullptr);
}

ICanMessage* CanNetwork::GetMessageBack(void) const
{
   return (this->messages.size() > 0 ? this->messages.back() : nullptr);
}

bool CanNetwork::RemoveMessageByIndex(size_t index)
{
   if (index < this->messages.size())
   {
      for (auto& node : this->nodes)
      {
         node->RemoveTxMessageById(this->messages[index]->GetId());
         node->RemoveRxMessageById(this->messages[index]->GetId());
      }

      helpers::ClearPtr(this->messages[index]);
      this->messages.erase(this->messages.begin() + index);
      return true;
   }
   else
   {
      return false;
   }
}

bool CanNetwork::RemoveMessageByName(const char* name)
{
   auto it = ranges::find_if(this->messages, [&name](CanMessage* message)
      {
         return !std::strcmp(message->GetName(), name);
      });

   if (it != this->messages.end())
   {
      for (auto& node : this->nodes)
      {
         node->RemoveTxMessageByName(name);
         node->RemoveRxMessageByName(name);
      }

      helpers::ClearPtr(*it);
      this->messages.erase(it);
      return true;
   }
   else
   {
      return false;
   }
}

bool CanNetwork::RemoveMessageById(uint32_t id)
{
   auto it = ranges::find_if(this->messages, [&id](CanMessage* message)
      {
         return message->GetId() == id;
      });

   if (it != this->messages.end())
   {
      for (auto& node : this->nodes)
      {
         node->RemoveTxMessageById(id);
         node->RemoveRxMessageById(id);
      }

      helpers::ClearPtr(*it);
      this->messages.erase(it);
      return true;
   }
   else
   {
      return false;
   }
}

size_t CanNetwork::GetSignalsCount(void) const
{
   return this->signals.size();
}

ICanSignal* CanNetwork::GetSignalByIndex(size_t index) const
{
   return (index < this->signals.size() ? this->signals[index] : nullptr);
}

ICanSignal* CanNetwork::GetSignalByName(const char* name) const
{
   auto it = ranges::find_if(this->signals, [&name](CanSignal* signal) { return !std::strcmp(signal->GetName(), name); });
   return (it != this->signals.end() ? *it : nullptr);
}

bool CanNetwork::SignalExists(const char* name) const
{
   return this->GetSignalByName(name) != nullptr;
}

std::vector<CanSignal*> CanNetwork::GetSignals(void)
{
   return this->signals;
}

size_t CanNetwork::GetSignalIndex(const char* name) const
{
   auto it = ranges::find_if(this->signals, [&name](CanSignal* signal) { return !std::strcmp(signal->GetName(), name); });
   return (it != this->signals.end() ? std::distance(this->signals.begin(), it) :
      ICanNetwork::INVALID_INDEX);
}

bool CanNetwork::RemoveSignalByIndex(size_t index)
{
   if (index < this->signals.size())
   {
      for (auto& node : this->nodes)
      {
         node->RemoveMappedTxSignalByName(this->signals[index]->GetName());
         node->RemoveMappedRxSignalByName(this->signals[index]->GetName());
      }
      if (auto message = this->signals[index]->GetMessage(); message)
      {
         message->RemoveSignalByName(this->signals[index]->GetName());
      }

      helpers::ClearPtr(this->signals[index]);
      this->signals.erase(this->signals.begin() + index);
      return true;
   }
   else
   {
      return false;
   }
}

void CanNetwork::SortSignalsByName(bool caseSensitive)
{
   auto comparator = [&caseSensitive](const std::string& name1, const std::string& name2)
   {
      return caseSensitive ? (name1 < name2) : helpers::iless(name1, name2);
   };
   ranges::sort(this->signals, [&comparator](CanSignal* signal1, CanSignal* signal2)
      {
         return comparator(signal1->GetName(), signal2->GetName());
      });
}

void CanNetwork::SortSignalsByMessageName(bool caseSensitive)
{
   auto comparator = [&caseSensitive](const std::string& name1, const std::string& name2)
      { return caseSensitive ? name1.compare(name2) : helpers::icompare(name1, name2); };
   ranges::sort(this->signals, [&comparator](CanSignal* signal1, CanSignal* signal2)
   {
      const auto message1 = signal1->GetMessage();
      const auto message2 = signal2->GetMessage();
      if (message1 && message2)
      {
         if (int cmp = comparator(message1->GetName(), message2->GetName()); !cmp)
         {
            return signal1->GetStartBit() < signal2->GetStartBit();
         }
         else
         {
            return cmp < 0;
         }
      }
      return false;
   });
}

bool CanNetwork::RemoveSignalByName(const char* name)
{
   auto it = ranges::find_if(this->signals, [&name](CanSignal* signal)
      {
         return !std::strcmp(signal->GetName(), name);
      });

   if (it != this->signals.end() && *it)
   {
      for (auto& node : this->nodes)
      {
         node->RemoveMappedTxSignalByName(name);
         node->RemoveMappedRxSignalByName(name);
      }
      if (auto message = (*it)->GetMessage(); message)
      {
         message->RemoveSignalByName((*it)->GetName());
      }

      helpers::ClearPtr(*it);
      this->signals.erase(it);
      return true;
   }
   else
   {
      return false;
   }
}

void CanNetwork::AddSignal(CanSignal* signal)
{
   if (signal)
   {
      this->signals.push_back(signal);
   }
}

CanSignal* CanNetwork::CreateAndAddSignal(void)
{
   CanSignal* signal = new CanSignal{};
   this->signals.push_back(signal);
   return signal;
}

ICanSignalBuilder* CanNetwork::SignalBuilder(void) const
{
   return this->signalBuilder.get();
}

ICanValueTableBuilder* CanNetwork::ValueTableBuilder(void) const
{
   return this->valueTableBuilder.get();
}

void CanNetwork::AddMessage(CanMessage* message)
{
   if (message)
   {
      this->messages.push_back(message);
   }
}

CanMessage* CanNetwork::CreateAndAddMessage(void)
{
   CanMessage* message = new CanMessage{};
   this->messages.push_back(message);
   return message;
}

size_t CanNetwork::GetEnvVarsCount(void) const
{
   return this->envVars.size();
}

ICanEnvVar* CanNetwork::GetEnvVarByIndex(size_t index) const
{
   return (index < this->envVars.size() ? this->envVars[index] : nullptr);
}

ICanEnvVar* CanNetwork::GetEnvVarByName(const char* name) const
{
   auto it = ranges::find_if(this->envVars, [&name](CanEnvVar* envVar) { return !std::strcmp(envVar->GetName(), name); });
   return (it != this->envVars.end() ? *it : nullptr);
}

std::vector<CanEnvVar*> CanNetwork::GetEnvVars(void)
{
   return this->envVars;
}

bool CanNetwork::RemoveEnvVarByIndex(size_t index)
{
   if (index < this->envVars.size())
   {
      helpers::ClearPtr(this->envVars[index]);
      this->envVars.erase(this->envVars.begin() + index);
      return true;
   }
   else
   {
      return false;
   }
}

bool CanNetwork::RemoveEnvVarByName(const char* name)
{
   auto it = ranges::find_if(this->envVars, [&name](CanEnvVar* envVar)
      {
         return !std::strcmp(envVar->GetName(), name);
      });

   if (it != this->envVars.end())
   {
      helpers::ClearPtr(*it);
      this->envVars.erase(it);
      return true;
   }
   else
   {
      return false;
   }
}

void CanNetwork::AddEnvVar(CanEnvVar* envVar)
{
   if (envVar)
   {
      this->envVars.push_back(envVar);
   }
}

size_t CanNetwork::GetAttributesCount(void) const
{
   return CanAttributeOwner::GetAttributesCount();
}

ICanAttribute* CanNetwork::GetAttributeByIndex(size_t index) const
{
   return CanAttributeOwner::GetAttributeByIndex(index);
}

ICanAttribute* CanNetwork::GetAttributeByName(const char* name) const
{
   return CanAttributeOwner::GetAttributeByName(name);
}

size_t CanNetwork::GetAttributesValuesCount(void) const
{
   return CanAttributeOwner::GetAttributesValuesCount();
}

ICanAttributeValue* CanNetwork::GetAttributeValue(const char* attributeName) const
{
   return CanAttributeOwner::GetAttributeValue(attributeName);
}

const char* CanNetwork::GetComment(void) const
{
   return this->comment.c_str();
}

void CanNetwork::ModifyComment(const char* comment)
{
   this->comment = comment;
}

void CanNetwork::SetComment(const char* comment)
{
   this->comment = comment;
}

void CanNetwork::SetMainAttributes(void)
{
   for (auto& attribute : this->GetAttributes())
   {
      if (attribute)
      {
         if (std::string_view attributeName = attribute->GetName(); attributeName == ICanNetwork::NETWORK_NAME)
         {
            if (auto attributeValue = this->GetAttributeValue(attributeName.data()); attributeValue)
            {
               auto value = ICanAttributeManager::GetAttributeValue<ICanAttribute::IValueType_e::STRING>
                  (attributeValue);
               this->SetName(value.c_str());
            }
         }
         else if (attributeName == ICanNetwork::PROTOCOL)
         {
            if (auto attributeValue = this->GetAttributeValue(attributeName.data()); attributeValue)
            {
               auto value = ICanAttributeManager::GetAttributeValue<ICanAttribute::IValueType_e::STRING>
                  (attributeValue);
               this->SetProtocol(value.c_str());
            }
         }
      }
   }
}