#include "CanMessage.h"
#include <algorithm>
#include "helpers.h"

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
   this->idFormat = idFormat;
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
   this->txMethod = txMethod;
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

const char* CanMessage::ToString(void)
{
   this->stringRepresentation += "Message { id: " + std::to_string(this->id) + ", name: " + this->name;
   this->stringRepresentation += ", size: " + std::to_string(this->size) + ", transmitter: " + this->mainTransmitter + " }";
   return this->stringRepresentation.c_str();
}