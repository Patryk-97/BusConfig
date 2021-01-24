#include "CanSignalBuilder.h"
#include "CanMessage.h"
#include "CanNetwork.h"

CanSignalBuilder::~CanSignalBuilder()
{
   this->Clear();
}

void CanSignalBuilder::Clear(void)
{
   this->network = nullptr;
   this->name.clear();
   this->startBit = ICanSignal::INVALID_START_BIT;
   this->size = ICanSignal::INVALID_SIZE;
   this->byteOrder = ICanSignal::IByteOrder_e::UNDEFINED;
   this->valueType = ICanSignal::IValueType_e::UNDEFINED_TYPE;
   this->factor = ICanSignal::INVALID_FACTOR;
   this->offset = 0.0;
   this->minimum = 0.0;
   this->maximum = 0.0;
   this->unit.clear();
   this->receivers.clear();
   this->comment.clear();
   this->message = nullptr;
}

ICanSignalBuilder* CanSignalBuilder::WithName(const char* name)
{
   this->name = name;
   return this;
}

ICanSignalBuilder* CanSignalBuilder::WithStartBit(uint32_t startBit)
{
   this->startBit = startBit;
   return this;
}

ICanSignalBuilder* CanSignalBuilder::WithSize(uint32_t size)
{
   this->size = size;
   return this;
}

ICanSignalBuilder* CanSignalBuilder::WithByteOrder(ICanSignal::IByteOrder_e byteOrder)
{
   this->byteOrder = byteOrder;
   return this;
}

ICanSignalBuilder* CanSignalBuilder::WithValueType(ICanSignal::IValueType_e valueType)
{
   this->valueType = valueType;
   return this;
}

ICanSignalBuilder* CanSignalBuilder::WithFactor(double factor)
{
   this->factor = factor;
   return this;
}

ICanSignalBuilder* CanSignalBuilder::WithOffset(double offset)
{
   this->offset = offset;
   return this;
}

ICanSignalBuilder* CanSignalBuilder::WithMinimum(double minimum)
{
   this->minimum = minimum;
   return this;
}

ICanSignalBuilder* CanSignalBuilder::WithMaximum(double maximum)
{
   this->maximum = maximum;
   return this;
}

ICanSignalBuilder* CanSignalBuilder::WithUnit(const char* unit)
{
   this->unit = unit;
   return this;
}

ICanSignalBuilder* CanSignalBuilder::WithReceiver(ICanNode* receiver)
{
   if (receiver)
   {
      this->receivers.push_back(dynamic_cast<CanNode*>(receiver));
   }
   return this;
}

ICanSignalBuilder* CanSignalBuilder::WithValueTable(ICanValueTable* valueTable)
{
   if (valueTable)
   {
      this->valueTable = dynamic_cast<CanValueTable*>(valueTable);
      std::string valueTableName = ICanValueTable::SIGNAL_PREFIX + this->name;
      this->valueTable->SetName(valueTableName.c_str());
   }
   return this;
}

ICanSignalBuilder* CanSignalBuilder::WithComment(const char* comment)
{
   this->comment = comment;
   return this;
}

ICanSignalBuilder* CanSignalBuilder::AddToMessage(ICanMessage* message)
{
   if (message)
   {
      this->message = dynamic_cast<CanMessage*>(message);
   }
   return this;
}

ICanSignal* CanSignalBuilder::Build(void)
{
   auto ifInvalidData = [this] { this->Clear(); return nullptr; };
   if (this->name.empty()) return ifInvalidData();
   if (this->startBit == ICanSignal::INVALID_START_BIT) return ifInvalidData();
   if (this->size == ICanSignal::INVALID_SIZE) return ifInvalidData();
   if (this->byteOrder == ICanSignal::IByteOrder_e::UNDEFINED) return ifInvalidData();
   if (this->valueType == ICanSignal::IValueType_e::UNDEFINED_TYPE) return ifInvalidData();
   if (this->factor == ICanSignal::INVALID_FACTOR) return ifInvalidData();
   if (this->message == nullptr) return ifInvalidData();
   if (this->network == nullptr) return ifInvalidData();

   auto signal = new CanSignal {};
   signal->SetName(this->name.c_str());
   signal->SetStartBit(this->startBit);
   signal->SetSize(this->size);
   signal->SetByteOrder(this->byteOrder);
   signal->SetValueType(this->valueType);
   signal->SetFactor(this->factor);
   signal->SetOffset(this->offset);
   signal->SetMinimum(this->minimum);
   signal->SetMaximum(this->maximum);
   signal->SetUnit(this->unit.c_str());
   signal->SetComment(this->comment.c_str());

   signal->SetMessage(this->message);
   this->message->AddSignal(signal);

   signal->SetNetwork(this->network);
   this->network->AddSignal(signal);

   for (auto& receiver : this->receivers)
   {
      receiver->AddMappedRxSignal(signal);
      receiver->AddRxMessage(this->message);

      if (auto mainTransmitter = this->message->GetMainTransmitter(); mainTransmitter)
      {
         if (auto transmitter = dynamic_cast<CanNode*>(mainTransmitter); transmitter)
         {
            transmitter->AddMappedTxSignal(signal);
         }
      }
   }

   return signal;
}