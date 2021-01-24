#pragma once

#include "ICanSignalBuilder.h"
#include "CanSignal.h"

class CanSignalBuilder : public ICanSignalBuilder
{
public:
   CanSignalBuilder(CanNetwork* network) : network { network } {}
   ~CanSignalBuilder();

   void Clear(void);

   ICanSignalBuilder* WithName(const char* name) override;
   ICanSignalBuilder* WithStartBit(uint32_t startBit) override;
   ICanSignalBuilder* WithSize(uint32_t size) override;
   ICanSignalBuilder* WithByteOrder(ICanSignal::IByteOrder_e byteOrder) override;
   ICanSignalBuilder* WithValueType(ICanSignal::IValueType_e valueType) override;
   ICanSignalBuilder* WithFactor(double factor) override;
   ICanSignalBuilder* WithOffset(double offset) override;
   ICanSignalBuilder* WithMinimum(double minimum) override;
   ICanSignalBuilder* WithMaximum(double maximum) override;
   ICanSignalBuilder* WithUnit(const char* unit) override;
   ICanSignalBuilder* WithReceiver(ICanNode* receiver) override;
   ICanSignalBuilder* WithValueTable(ICanValueTable* valueTable) override;
   ICanSignalBuilder* WithComment(const char* comment) override;
   ICanSignalBuilder* AddToMessage(ICanMessage* message) override;
   ICanSignal* Build(void) override;

private:
   CanNetwork* network { nullptr };
   std::string name;
   uint32_t startBit { ICanSignal::INVALID_START_BIT };
   uint32_t size { ICanSignal::INVALID_SIZE };
   ICanSignal::IByteOrder_e byteOrder { ICanSignal::IByteOrder_e::UNDEFINED };
   ICanSignal::IValueType_e valueType { ICanSignal::IValueType_e::UNDEFINED_TYPE };
   double factor { ICanSignal::INVALID_FACTOR };
   double offset {};
   double minimum {};
   double maximum {};
   std::string unit;
   CanNode* receiver { nullptr };
   CanValueTable* valueTable { nullptr };
   std::string comment;
   CanMessage* message { nullptr };
};