#pragma once

#include "ICanSignal.h"
#include "CanValueTable.h"
#include "CanAttributeOwner.h"

#include <string>
#include <vector>

class CanMessage; // circular dependency
class CanNetwork; // circular dependency

class CanSignal : public CanAttributeOwner, public ICanSignal
{
public:
   CanSignal() : CanAttributeOwner(ICanAttribute::IObjectType_e::SIGNAL) {};
   ~CanSignal();

   void Clear(void) override;

   const char* GetName(void) const override;
   void ModifyName(const char* name) override;
   void SetName(const char* name);

   bool IsNotMultiplexed(void) const override;
   bool IsMultiplexerSwitch(void) const override;
   bool IsMultiplexedSignal(void) const override;

   IMuxType_e GetMuxType(void) const override;
   const char* GetMuxTypeStr(void) const override;
   void SetMuxType(IMuxType_e muxType);

   const char* GetMuxIndicator(void) const override;
   void SetMuxIndicator(const char* muxIndicator);

   uint32_t GetMultiplexValue(void) const override;
   void SetMultiplexValue(uint32_t multiplexValue);

   uint32_t GetStartBit(void) const override;
   void ModifyStartBit(uint32_t startBit) override;
   void SetStartBit(uint32_t startBit);

   uint32_t GetSize(void) const override;
   void ModifySize(uint32_t size) override;
   void SetSize(uint32_t size);

   IByteOrder_e GetByteOrder(void) const override;
   void ModifyByteOrder(IByteOrder_e byteOrder) override;
   uint8_t GetByteOrderSymbol(void) const override;
   void SetByteOrderSymbol(uint8_t byteOrderSymbol);

   IValueType_e GetValueType(void) const override;
   void ModifyValueType(IValueType_e valueType) override;
   uint8_t GetValueTypeSymbol(void) const override;
   void SetValueTypeSymbol(uint8_t valueTypeSymbol);

   int32_t GetRawInitialValue(void) const override;
   void ModifyRawInitialValue(int32_t rawInitialValue) override;
   void SetRawInitialValue(int32_t rawInitialValue);

   double GetInitialValue(void) const override;
   void ModifyInitialValue(double initialValue) override;

   double GetFactor(void) const override;
   void ModifyFactor(double factor) override;
   void SetFactor(double factor);

   double GetOffset(void) const override;
   void ModifyOffset(double offset) override;
   void SetOffset(double offset);

   double GetMinimum(void) const override;
   void ModifyMinimum(double minimum) override;
   void SetMinimum(double minimum);

   double GetMaximum(void) const override;
   void ModifyMaximum(double maximum) override;
   void SetMaximum(double maximum);

   const char* GetUnit(void) const override;
   void ModifyUnit(const char* unit) override;
   void SetUnit(const char* unit);

   size_t GetReceiversCount(void) const override;
   const char* GetReceiver(size_t index) const override;
   void AddReceiver(const char* receiver);

   ICanMessage* GetMessage(void) const override;
   void SetMessage(CanMessage* message);

   ICanValueTable* GetValueTable(void) const override;
   void SetValueTable(CanValueTable* valueTable);

   size_t GetAttributesCount(void) const override;
   ICanAttribute* GetAttributeByIndex(size_t index) const override;
   ICanAttribute* GetAttributeByName(const char* name) const override;

   size_t GetAttributesValuesCount(void) const override;
   ICanAttributeValue* GetAttributeValue(const char* attributeName) const override;

   const char* GetComment(void) const override;
   void ModifyComment(const char* comment) override;
   void SetComment(const char* comment);

   ICanNetwork* GetNetwork(void) const override;
   void SetNetwork(CanNetwork* network);

   const char* ToString(void) override;

private:
   std::string name;

   IMuxType_e muxType = IMuxType_e::NOT_MULTIPLEXED;
   std::string muxTypeStr;
   uint32_t multiplexValue = ICanSignal::INVALID_MULTIPLEX_VALUE;
   std::string muxIndicator; // multiplexer_indicator = ' ' | 'M' | m multiplexer_switch_value;

   uint32_t startBit {}; // The startbit has to be in the range of 0 to (8 * message_size - 1). 
   uint32_t size {}; // The signal_size specifies the size of the signal in bits

   uint8_t byteOrderSymbol = ICanSignal::UNDEFINED_BYTE_ORDER_SYMBOL;  // 1 = little-endian, Intel; 0 = big-endian, Motorola
   IByteOrder_e byteOrder = IByteOrder_e::UNDEFINED;

   uint8_t valueTypeSymbol = ICanSignal::UNDEFINED_VALUE_TYPE_SYMBOL; //The value_type defines the signal as being of type unsigned (+) or signed (-). 
   IValueType_e valueType = IValueType_e::UNDEFINED_TYPE;

   uint32_t rawInitialValue { ICanSignal::DEFAULT_RAW_INITIAL_VALUE };

   double factor {};
   double offset {};

   //The minimum and maximum define the range of valid physical values of the signal.
   double minimum {};
   double maximum {};

   std::string unit;

   std::vector<std::string> receivers; //  nodes_names | 'Vector__XXX'
   CanMessage* message { nullptr };
   CanValueTable* valueTable { nullptr };
   std::vector<CanAttribute*> attributes;
   std::string comment;
   CanNetwork* network { nullptr };
   std::string stringRepresentation;
};