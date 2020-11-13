#include "CanMessage.h"

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

const char* CanMessage::ToString(void)
{
   this->stringRepresentation += ("Message { id: " + std::to_string(this->id) + ", name: " + this->name);
   this->stringRepresentation += ", size: " + std::to_string(this->size) + ", transmitter: " + this->mainTransmitter + " }";
   return this->stringRepresentation.c_str();
}