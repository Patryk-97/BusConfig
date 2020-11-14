#include "CanSignal.h"

CanSignal::~CanSignal()
{
   this->Clear();
}

void CanSignal::Clear(void)
{
   this->name = "";
   this->muxType = IMuxType_e::NOT_MULTIPLEXED;
   this->muxTypeStr = "";
   this->multiplexValue = ICanSignal::NOT_MULTIPLEX_VALUE;
   this->muxIndicator = "";
   this->startBit = 0;
   this->size = 0;
   this->byteOrderSymbol = ICanSignal::UNDEFINED_BYTE_ORDER_SYMBOL;
   this->byteOrder = IByteOrder_e::UNDEFINED;
   this->byteOrderStr = "";
   this->valueTypeSymbol = ICanSignal::UNDEFINED_VALUE_TYPE_SYMBOL;
   this->valueType = IValueType_e::UNDEFINED_TYPE;
   this->valueTypeStr = "";
   this->factor = 0.0;
   this->offset = 0.0;
   this->minimum = 0.0;
   this->maximum = 0.0;
   this->unit = "";
   this->receivers.clear();
}

const char* CanSignal::GetName(void) const
{
   return this->name.c_str();
}

void CanSignal::SetName(const char* name)
{
   this->name = name;
}

bool CanSignal::IsNotMultiplexed(void) const
{
   return this->muxType == CanSignal::IMuxType_e::NOT_MULTIPLEXED;
}

bool CanSignal::IsMultiplexerSwitch(void) const
{
   return this->muxType == CanSignal::IMuxType_e::MULTIPLEXER_SWITCH;
}

bool CanSignal::IsMultiplexedSignal(void) const
{
   return this->muxType == CanSignal::IMuxType_e::MULTIPLEXED_SIGNAL;
}

ICanSignal::IMuxType_e CanSignal::GetMuxType(void) const
{
   return this->muxType;
}

const char* CanSignal::GetMuxTypeStr(void) const
{
   return this->muxTypeStr.c_str();
}

void CanSignal::SetMuxType(IMuxType_e muxType)
{
   this->muxType = muxType;
}

const char* CanSignal::GetMuxIndicator(void) const
{
   return this->muxIndicator.c_str();
}

void CanSignal::SetMuxIndicator(const char* muxIndicator)
{
   this->muxIndicator = muxIndicator;
   this->muxType = std::invoke([this]
   {
      if (this->muxIndicator[0] == 'M')
      {
         return IMuxType_e::MULTIPLEXER_SWITCH;
      }
      if (this->muxIndicator[0] == 'm')
      {
         this->multiplexValue = std::stoul(this->muxIndicator.substr(1, this->muxIndicator.size()));
         return IMuxType_e::MULTIPLEXED_SIGNAL;
      }
      return IMuxType_e::NOT_MULTIPLEXED;
   });
}

uint32_t CanSignal::GetMultiplexValue(void) const
{
   return this->multiplexValue;
}

void CanSignal::SetMultiplexValue(uint32_t multiplexValue)
{
   this->multiplexValue = multiplexValue;
}

uint32_t CanSignal::GetStartBit(void) const
{
   return this->startBit;
}

void CanSignal::SetStartBit(uint32_t startBit)
{
   this->startBit = startBit;
}

uint32_t CanSignal::GetSize(void) const
{
   return this->size;
}

void CanSignal::SetSize(uint32_t size)
{
   this->size = size;
}

ICanSignal::IByteOrder_e CanSignal::GetByteOrder(void) const
{
   return this->byteOrder;
}

const char* CanSignal::GetByteOrderStr(void) const
{
   return this->byteOrderStr.c_str();
}

void CanSignal::SetByteOrder(IByteOrder_e byteOrder)
{
   this->byteOrder = byteOrder;
}

uint8_t CanSignal::GetValueTypeSymbol(void) const
{
   return this->valueTypeSymbol;
}

void CanSignal::SetValueTypeSymbol(uint8_t valueTypeSymbol)
{
   this->valueTypeSymbol = valueTypeSymbol;
}

ICanSignal::IValueType_e CanSignal::GetValueType(void) const
{
   return this->valueType;
}

void CanSignal::SetValueType(IValueType_e valueType)
{
   this->valueType = valueType;
}

const char* CanSignal::GetValueTypeStr(void) const
{
   return this->valueTypeStr.c_str();
}
void CanSignal::SetValueTypeStr(const char* valueTypeStr)
{
   this->valueTypeStr = valueTypeStr;
}

double CanSignal::GetFactor(void) const
{
   return this->factor;
}

void CanSignal::SetFactor(double factor)
{
   this->factor = factor;
}

double CanSignal::GetOffset(void) const
{
   return this->offset;
}

void CanSignal::SetOffset(double offset)
{
   this->offset = offset;
}

double CanSignal::GetMinimum(void) const
{
   return this->minimum;
}

void CanSignal::SetMinimum(double minimum)
{
   this->minimum = minimum;
}

double CanSignal::GetMaximum(void) const
{
   return this->maximum;  
}

void CanSignal::SetMaximum(double maximum)
{
   this->maximum = maximum;
}

const char* CanSignal::GetUnit(void) const
{
   return this->unit.c_str();
}

void CanSignal::SetUnit(const char* unit)
{
   this->unit = unit;
}

size_t CanSignal::GetReceiversCount(void) const
{
   return this->receivers.size();
}

const char* CanSignal::GetReceiver(size_t index) const
{
   return (index < this->receivers.size() ? this->receivers[index].c_str() : nullptr);
}

void CanSignal::AddReceiver(const char* receiver)
{
   if (receiver != nullptr)
   {
      this->receivers.push_back(receiver);
   }
}