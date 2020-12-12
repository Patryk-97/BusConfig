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
   this->id = 0;
   this->name = "";
   this->size = 0;
   this->mainTransmitter = "";
   this->signals.clear();
   this->attributes.clear();
   this->stringRepresentation = "";
}

uint32_t CanMessage::GetId(void) const
{
   return this->id;
}

void CanMessage::SetId(uint32_t id)
{
   this->id = id;
}

const char* CanMessage::GetName(void) const
{
   return this->name.c_str();
}

void CanMessage::SetName(const char* name)
{
   this->name = name;
}

uint32_t CanMessage::GetSize(void) const
{
   return this->size;
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
   return this->attributes.size();
}

ICanAttribute* CanMessage::GetAttributeByIndex(size_t index) const
{
   return (index < this->attributes.size() ? this->attributes[index] : nullptr);
}

ICanAttribute* CanMessage::GetAttributeByName(const char* name) const
{
   auto it = ranges::find_if(this->attributes, [&name](CanAttribute* attribute) { return !std::strcmp(attribute->GetName(), name); });
   return (it != this->attributes.end() ? *it : nullptr);
}

void CanMessage::AddAttribute(CanAttribute* attribute)
{
   if (attribute && attribute->GetObjectType() == ICanAttribute::IObjectType_e::MESSAGE)
   {
      this->attributes.push_back(attribute);
   }
}

const char* CanMessage::ToString(void)
{
   this->stringRepresentation += "Message { id: " + std::to_string(this->id) + ", name: " + this->name;
   this->stringRepresentation += ", size: " + std::to_string(this->size) + ", transmitter: " + this->mainTransmitter + " }";
   return this->stringRepresentation.c_str();
}