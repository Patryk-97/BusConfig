#include "CanMessage.h"
#include <algorithm>
#include "helpers.h"
#include "CanAttributeManager.h"
#include "CanNetwork.h" // circular dependency
#include "ICanAttributeManager.h"

namespace ranges = std::ranges;

CanMessage::~CanMessage()
{
   this->Clear();
}

void CanMessage::Clear(void)
{
   CanAttributeOwner::Clear();
   this->id = 0;
   this->name.clear();
   this->size = 0;
   this->mainTransmitter.clear();
   this->signals.clear();
   this->idFormat = ICanMessage::DEFAULT_ID_FORMAT;
   this->txMethod = ICanMessage::DEFAULT_TX_METHOD;
   this->cycleTime =  ICanMessage::DEFAULT_CYCLE_TIME;
   this->comment.clear();
   this->network = nullptr;
   this->stringRepresentation.clear();
}

uint32_t CanMessage::GetId(void) const
{
   return this->id;
}

void CanMessage::ModifyId(uint32_t id)
{
   this->id = id;
}

void CanMessage::SetId(uint32_t id)
{
   this->id = id;
}

const char* CanMessage::GetName(void) const
{
   return this->name.c_str();
}

void CanMessage::ModifyName(const char* name)
{
   this->name = name;
}

void CanMessage::SetName(const char* name)
{
   this->name = name;
}

uint32_t CanMessage::GetSize(void) const
{
   return this->size;
}

void CanMessage::ModifySize(uint32_t size)
{
   this->size = size;
}

void CanMessage::SetSize(uint32_t size)
{
   this->size = size;
}

const char* CanMessage::GetMainTransmitter(void) const
{
   return this->mainTransmitter.c_str();
}

void CanMessage::SetMainTransmitter(const char* mainTransmitter)
{
   this->mainTransmitter = mainTransmitter;
}

size_t CanMessage::GetSignalsCount(void) const
{
   return this->signals.size();
}

ICanSignal* CanMessage::GetSignalByName(const char* name) const
{
   auto it = ranges::find_if(this->signals, [&name] (const CanSignal* signal) { return !std::strcmp(name, signal->GetName()); });
   return (it != this->signals.end() ? *it : nullptr);
}

ICanSignal* CanMessage::GetSignalByIndex(size_t index) const
{
   return (index < this->signals.size() ? this->signals[index] : nullptr);
}

std::vector<CanSignal*> CanMessage::GetSignals(void)
{
   return this->signals;
}

bool CanMessage::RemoveSignalByIndex(size_t index)
{
   if (index < this->signals.size())
   {
      this->signals.erase(this->signals.begin() + index);
      return true;
   }
   else
   {
      return false;
   }
}

bool CanMessage::RemoveSignalByName(const char* name)
{
   return (0 != std::erase_if(this->signals, [&name] (CanSignal* signal)
      { return !std::strcmp(signal->GetName(), name); }));
}

void CanMessage::SortSignalsByName(bool caseSensitive)
{
   auto comparator = [&caseSensitive] (const std::string& name1, const std::string& name2)
      { return caseSensitive ? (name1 < name2) : helpers::iless(name1, name2); };
   ranges::sort(this->signals, [&comparator] (CanSignal* signal1, CanSignal* signal2)
      { return comparator(signal1->GetName(), signal2->GetName()); });
}

void CanMessage::SortSignalsByStartBit(void)
{
   ranges::sort(this->signals, [] (CanSignal* signal1, CanSignal* signal2)
      { return signal1->GetStartBit() < signal2->GetStartBit(); });
}

void CanMessage::AddSignal(CanSignal* signal)
{
   if (signal)
   {
      this->signals.push_back(signal);
   }
}

size_t CanMessage::GetAttributesCount(void) const
{
   return CanAttributeOwner::GetAttributesCount();
}

ICanAttribute* CanMessage::GetAttributeByIndex(size_t index) const
{
   return CanAttributeOwner::GetAttributeByIndex(index);
}

ICanAttribute* CanMessage::GetAttributeByName(const char* name) const
{
   return CanAttributeOwner::GetAttributeByName(name);
}

size_t CanMessage::GetAttributesValuesCount(void) const
{
   return CanAttributeOwner::GetAttributesValuesCount();
}

ICanAttributeValue* CanMessage::GetAttributeValue(const char* attributeName) const
{
   return CanAttributeOwner::GetAttributeValue(attributeName);
}

ICanMessage::IdFormat_e CanMessage::GetIdFormat(void) const
{
   return this->idFormat;
}

void CanMessage::ModifyIdFormat(IdFormat_e idFormat)
{
   if (this->idFormat != idFormat)
   {
      this->idFormat = idFormat;
      const auto key = GetIdFormatKey(idFormat).data();
      const auto attribute = this->GetAttributeByName(ICanMessage::ID_FORMAT);
      auto attributeValue = this->GetAttributeValue(attribute->GetName());
      CanAttributeManager::SetValue(attributeValue, key);
   }
}

void CanMessage::SetIdFormat(IdFormat_e idFormat)
{
   this->idFormat = idFormat;
}

ICanMessage::TxMethod_e CanMessage::GetTxMethod(void) const
{
   return this->txMethod;
}

void CanMessage::ModifyTxMethod(TxMethod_e txMethod)
{
   if (this->txMethod != txMethod)
   {
      this->txMethod = txMethod;
      const auto key = GetTxMethodKey(txMethod).data();
      const auto attribute = this->GetAttributeByName(ICanMessage::TX_METHOD);
      auto attributeValue = this->GetAttributeValue(attribute->GetName());
      CanAttributeManager::SetValue(attributeValue, key);
   }
}

void CanMessage::SetTxMethod(TxMethod_e txMethod)
{
   this->txMethod = txMethod;
}

ICanMessage::cycle_time_t CanMessage::GetCycleTime(void) const
{
   return this->cycleTime;
}

void CanMessage::ModifyCycleTime(cycle_time_t cycleTime)
{
   this->cycleTime = cycleTime;
}

void CanMessage::SetCycleTime(cycle_time_t cycleTime)
{
   this->cycleTime = cycleTime;
}

const char* CanMessage::GetComment(void) const
{
   return this->comment.c_str();
}

void CanMessage::ModifyComment(const char* comment)
{
   this->comment = comment;
}

void CanMessage::SetComment(const char* comment)
{
   this->comment = comment;
}

ICanNetwork* CanMessage::GetNetwork(void) const
{
   return this->network;
}

void CanMessage::SetNetwork(CanNetwork* network)
{
   this->network = network;
}

const char* CanMessage::ToString(void)
{
   this->stringRepresentation += "Message { id: " + std::to_string(this->id) + ", name: " + this->name;
   this->stringRepresentation += ", size: " + std::to_string(this->size) + ", transmitter: " + this->mainTransmitter + " }";
   return this->stringRepresentation.c_str();
}

void CanMessage::SetMainAttributes(void)
{
   for (auto& attribute : this->GetAttributes())
   {
      if (attribute)
      {
         std::string_view attributeName = attribute->GetName();
         if (attributeName == ICanMessage::ID_FORMAT)
         {
            if (auto attributeValue = this->GetAttributeValue(attributeName.data()); attributeValue)
            {
               auto value = ICanAttributeManager::GetAttributeValue<ICanMessage::IdFormat::VALUE_TYPE>
                  (attributeValue);
               this->idFormat = CanMessage::ID_FORMATS.at(value);
            }
         }
         else if (attributeName == ICanMessage::TX_METHOD)
         {
            if (auto attributeValue = this->GetAttributeValue(attributeName.data()); attributeValue)
            {
               auto value = ICanAttributeManager::GetAttributeValue<ICanMessage::TxMethod::VALUE_TYPE>
                  (attributeValue);
               this->txMethod = CanMessage::TX_METHODS.at(value);
            }
         }
         else if (attributeName == ICanMessage::CYCLE_TIME)
         {
            if (auto attributeValue = this->GetAttributeValue(attributeName.data()); attributeValue)
            {
               auto value = ICanAttributeManager::GetAttributeValue<ICanMessage::CycleTime::VALUE_TYPE>
                  (attributeValue);
               this->SetCycleTime(value);
            }
         }
      }
   }
}