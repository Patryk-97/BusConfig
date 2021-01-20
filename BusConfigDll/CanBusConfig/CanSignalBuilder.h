#pragma once

#include "ICanSignalBuilder.h"
#include "CanSignal.h"

class CanSignalBuilder : public ICanSignalBuilder
{
public:
   CanSignalBuilder() = default;
   ~CanSignalBuilder();

   void WithName(const char* name) override;
   void WithStartBit(uint32_t startBit) override;
   void WithSize(uint32_t size) override;
   void WithByteOrder(ICanSignal::IByteOrder_e byteOrder) override;
   void WithValueType(ICanSignal::IValueType_e valueType) override;
   void WithFactor(double factor) override;
   void WithOffset(double offset) override;
   void WithMinimum(double minimum) override;
   void WithMaximum(double maximum) override;
   void WithUnit(const char* unit) override;
   void WithComment(const char* comment) override;
   void AddToMessage(ICanMessage* canMessage) override;
   ICanSignal* Build(void) override;

private:
   CanSignal* signal { new CanSignal {} };
};