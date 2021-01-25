#include "CanMessageBuilder.h"
#include "CanNetwork.h"

CanMessageBuilder::~CanMessageBuilder()
{
   this->Clear();
}

void CanMessageBuilder::Clear(void)
{
   this->network = nullptr;
   this->name.clear();
   this->id = ICanMessage::INVALID_ID;
   this->idFormat = ICanMessage::DEFAULT_ID_FORMAT;
   this->size = ICanMessage::INVALID_SIZE;
   this->txMethod = ICanMessage::DEFAULT_TX_METHOD;
   this->cycleTime = ICanMessage::DEFAULT_CYCLE_TIME;
   this->mainTransmitter = nullptr;
   this->comment.clear();
}

ICanMessageBuilder* CanMessageBuilder::WithName(const char* name)
{
   this->name = name;
   return this;
}

ICanMessageBuilder* CanMessageBuilder::WithId(uint32_t id)
{
   this->id = id;
   return this;
}

ICanMessageBuilder* CanMessageBuilder::WithIdFormat(ICanMessage::IdFormat_e idFormat)
{
   this->idFormat = idFormat;
   return this;
}

ICanMessageBuilder* CanMessageBuilder::WithSize(uint32_t size)
{
   this->size = size;
   return this;
}

ICanMessageBuilder* CanMessageBuilder::WithTxMethod(ICanMessage::TxMethod_e txMethod)
{
   this->txMethod = txMethod;
   return this;
}

ICanMessageBuilder* CanMessageBuilder::WithCycleTime(ICanMessage::cycle_time_t cycleTime)
{
   this->cycleTime = cycleTime;
   return this;
}

ICanMessageBuilder* CanMessageBuilder::WithMainTransmitter(ICanNode* mainTransmitter)
{
   if (mainTransmitter)
   {
      this->mainTransmitter = dynamic_cast<CanNode*>(mainTransmitter);
   }
   return this;
}

ICanMessageBuilder* CanMessageBuilder::WithComment(const char* comment)
{
   this->comment = comment;
   return this;
}

ICanMessage* CanMessageBuilder::Build(void)
{
   auto ifInvalidData = [this] { this->Clear(); return nullptr; };
   if (this->name.empty()) return ifInvalidData();
   if (this->id == ICanMessage::INVALID_ID) return ifInvalidData();
   if (this->idFormat == ICanMessage::IdFormat_e::RESERVED) return ifInvalidData();
   if (this->size == ICanMessage::INVALID_SIZE) return ifInvalidData();
   if (this->txMethod == ICanMessage::TxMethod_e::NOT_USED) return ifInvalidData();
   if (this->mainTransmitter == nullptr) return ifInvalidData();
   if (this->network == nullptr) return ifInvalidData();

   auto message = new CanMessage {};
   message->SetName(this->name.c_str());
   message->SetId(this->id);
   message->SetIdFormat(this->idFormat);
   message->SetSize(this->size);
   message->SetTxMethod(this->txMethod);
   message->SetCycleTime(this->cycleTime);
   message->SetMainTransmitter(this->mainTransmitter);
   message->SetComment(this->comment.c_str());

   message->SetNetwork(this->network);
   this->network->AddMessage(message);

   this->mainTransmitter->AddTxMessage(message);

   return message;
}