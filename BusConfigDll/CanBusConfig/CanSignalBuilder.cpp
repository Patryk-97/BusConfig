#include "CanSignalBuilder.h"
#include "CanMessage.h"

CanSignalBuilder::~CanSignalBuilder()
{
   this->signal = nullptr;
}

void CanSignalBuilder::WithName(const char* name)
{
   this->signal->SetName(name);
}

void CanSignalBuilder::WithStartBit(uint32_t startBit)
{
   this->signal->SetStartBit(startBit);
}

void CanSignalBuilder::WithSize(uint32_t size)
{
   this->signal->SetSize(size);
}

void CanSignalBuilder::WithByteOrder(ICanSignal::IByteOrder_e byteOrder)
{
   this->signal->SetByteOrder(byteOrder);
}

void CanSignalBuilder::WithValueType(ICanSignal::IValueType_e valueType)
{
   this->signal->SetValueType(valueType);
}

void CanSignalBuilder::WithFactor(double factor)
{
   this->signal->SetFactor(factor);
}

void CanSignalBuilder::WithOffset(double offset)
{
   this->signal->SetOffset(offset);
}

void CanSignalBuilder::WithMinimum(double minimum)
{
   this->signal->SetMinimum(minimum);
}

void CanSignalBuilder::WithMaximum(double maximum)
{
   this->signal->SetMaximum(maximum);
}

void CanSignalBuilder::WithUnit(const char* unit)
{
   this->signal->SetUnit(unit);
}

void CanSignalBuilder::WithComment(const char* comment)
{
   this->signal->SetComment(comment);
}

void CanSignalBuilder::AddToMessage(ICanMessage* canMessage)
{
   if (auto message = dynamic_cast<CanMessage*>(canMessage); message)
   {
      this->signal->SetMessage(message);
      message->AddSignal(this->signal);
   }
}

ICanSignal* CanSignalBuilder::Build(void)
{
   return std::exchange(this->signal, new CanSignal {});
}