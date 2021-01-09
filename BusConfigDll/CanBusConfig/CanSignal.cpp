#include "CanSignal.h"
#include "CanMessage.h" // circular dependency
#include "helpers.h"
#include <algorithm>

namespace ranges = std::ranges;
using namespace std::string_literals;

CanSignal::~CanSignal()
{
   this->Clear();
}

void CanSignal::Clear(void)
{
   CanAttributeOwner::Clear();
   this->name.clear();
   this->muxType = IMuxType_e::NOT_MULTIPLEXED;
   this->muxTypeStr.clear();
   this->multiplexValue = ICanSignal::INVALID_MULTIPLEX_VALUE;
   this->muxIndicator.clear();
   this->startBit = 0;
   this->size = 0;
   this->byteOrderSymbol = ICanSignal::UNDEFINED_BYTE_ORDER_SYMBOL;
   this->byteOrder = IByteOrder_e::UNDEFINED;
   this->valueTypeSymbol = ICanSignal::UNDEFINED_VALUE_TYPE_SYMBOL;
   this->valueType = IValueType_e::UNDEFINED_TYPE;
   this->factor = 0.0;
   this->offset = 0.0;
   this->minimum = 0.0;
   this->maximum = 0.0;
   this->unit.clear();
   this->receivers.clear();
   this->message = nullptr;
   helpers::ClearPtr(this->valueTable);
   this->comment.clear();
   this->stringRepresentation.clear();
}

const char* CanSignal::GetName(void) const
{
   return this->name.c_str();
}

void CanSignal::ModifyName(const char* name)
{
   this->name = name;
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

void CanSignal::ModifyStartBit(uint32_t startBit)
{
   this->startBit = startBit;
}

void CanSignal::SetStartBit(uint32_t startBit)
{
   this->startBit = startBit;
}

uint32_t CanSignal::GetSize(void) const
{
   return this->size;
}

void CanSignal::ModifySize(uint32_t size)
{
   this->size = size;
}

void CanSignal::SetSize(uint32_t size)
{
   this->size = size;
}

ICanSignal::IByteOrder_e CanSignal::GetByteOrder(void) const
{
   return this->byteOrder;
}

void CanSignal::ModifyByteOrder(IByteOrder_e byteOrder)
{
   this->byteOrder = byteOrder;
   this->byteOrderSymbol = (byteOrder == IByteOrder_e::BIG_ENDIAN ? '0' : '1');
}

uint8_t CanSignal::GetByteOrderSymbol(void) const
{
   return this->byteOrderSymbol;
}

void CanSignal::SetByteOrderSymbol(uint8_t byteOrderSymbol)
{
   this->byteOrderSymbol = byteOrderSymbol;
   byteOrderSymbol -= '0';
   if (byteOrderSymbol <= 1)
   {
      this->byteOrder = static_cast<IByteOrder_e>(byteOrderSymbol);
   }
   else
   {
      this->byteOrder = IByteOrder_e::UNDEFINED;
   }
}

ICanSignal::IValueType_e CanSignal::GetValueType(void) const
{
   return this->valueType;
}

void CanSignal::ModifyValueType(IValueType_e valueType)
{
   this->valueType = valueType;
   this->valueTypeSymbol = (valueType == IValueType_e::UNSIGNED_TYPE ? '+' : '-');
}

uint8_t CanSignal::GetValueTypeSymbol(void) const
{
   return this->valueTypeSymbol;
}

void CanSignal::SetValueTypeSymbol(uint8_t valueTypeSymbol)
{
   this->valueTypeSymbol = valueTypeSymbol;
   switch (valueTypeSymbol)
   {
      case '+':
      {
         this->valueType = ICanSignal::IValueType_e::UNSIGNED_TYPE;
         break;
      }
      case '-':
      {
         this->valueType = ICanSignal::IValueType_e::SIGNED_TYPE;
         break;
      }
      default:
      {
         this->valueType = ICanSignal::IValueType_e::UNDEFINED_TYPE;
         break;
      }
   }
}

double CanSignal::GetFactor(void) const
{
   return this->factor;
}

void CanSignal::ModifyFactor(double factor)
{
   this->factor = factor;
}

void CanSignal::SetFactor(double factor)
{
   this->factor = factor;
}

double CanSignal::GetOffset(void) const
{
   return this->offset;
}

void CanSignal::ModifyOffset(double offset)
{
   this->offset = offset;
}

void CanSignal::SetOffset(double offset)
{
   this->offset = offset;
}

double CanSignal::GetMinimum(void) const
{
   return this->minimum;
}

void CanSignal::ModifyMinimum(double minimum)
{
   this->minimum = minimum;
}

void CanSignal::SetMinimum(double minimum)
{
   this->minimum = minimum;
}

double CanSignal::GetMaximum(void) const
{
   return this->maximum;  
}

void CanSignal::ModifyMaximum(double maximum)
{
   this->maximum = maximum;
}

void CanSignal::SetMaximum(double maximum)
{
   this->maximum = maximum;
}

const char* CanSignal::GetUnit(void) const
{
   return this->unit.c_str();
}

void CanSignal::ModifyUnit(const char* unit)
{
   this->unit = unit;
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

ICanMessage* CanSignal::GetMessage(void) const
{
   return this->message;
}

void CanSignal::SetMessage(CanMessage* message)
{
   this->message = message;
}

ICanValueTable* CanSignal::GetValueTable(void) const
{
   return this->valueTable;
}

void CanSignal::SetValueTable(CanValueTable* valueTable)
{
   this->valueTable = valueTable;
}

size_t CanSignal::GetAttributesCount(void) const
{
   return CanAttributeOwner::GetAttributesCount();
}

ICanAttribute* CanSignal::GetAttributeByIndex(size_t index) const
{
   return CanAttributeOwner::GetAttributeByIndex(index);
}

ICanAttribute* CanSignal::GetAttributeByName(const char* name) const
{
   return CanAttributeOwner::GetAttributeByName(name);
}

size_t CanSignal::GetAttributesValuesCount(void) const
{
   return CanAttributeOwner::GetAttributesValuesCount();
}

ICanAttributeValue* CanSignal::GetAttributeValue(const char* attributeName) const
{
   return CanAttributeOwner::GetAttributeValue(attributeName);
}

const char* CanSignal::GetComment(void) const
{
   return this->comment.c_str();
}

void CanSignal::SetComment(const char* comment)
{
   this->comment = comment;
}

const char* CanSignal::ToString(void)
{
   this->stringRepresentation += "Signal { name: " + this->name + ", muxType: " + this->muxTypeStr;
   this->stringRepresentation += ", start bit: " + std::to_string(this->startBit) + ", size: " + std::to_string(this->size);
   this->stringRepresentation += ", byte order: "s;
   this->stringRepresentation += this->byteOrderSymbol;
   this->stringRepresentation += ", value type: "s;
   this->stringRepresentation += this->valueTypeSymbol;
   this->stringRepresentation += ", factor: " + std::to_string(this->factor) + ", offset: " + std::to_string(this->offset);
   this->stringRepresentation += ", min: " + std::to_string(this->minimum) + ", max: " + std::to_string(this->maximum);
   this->stringRepresentation += " unit: " + this->unit + ", receivers: ";
   for (const auto& receiver : this->receivers)
   {
      this->stringRepresentation += receiver + ", ";
   }
   this->stringRepresentation += "}";
   return this->stringRepresentation.c_str();
}