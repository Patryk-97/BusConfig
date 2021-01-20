#pragma once

#include "ICanSignal.h"

class ICanSignalBuilder
{
protected:
   ICanSignalBuilder() {};
   virtual ~ICanSignalBuilder() {};
public:
   virtual ICanSignalBuilder* WithName(const char* name) = 0;
   virtual ICanSignalBuilder* WithStartBit(uint32_t startBit) = 0;
   virtual ICanSignalBuilder* WithSize(uint32_t size) = 0;
   virtual ICanSignalBuilder* WithByteOrder(ICanSignal::IByteOrder_e byteOrder) = 0;
   virtual ICanSignalBuilder* WithValueType(ICanSignal::IValueType_e valueType) = 0;
   virtual ICanSignalBuilder* WithFactor(double factor) = 0;
   virtual ICanSignalBuilder* WithOffset(double offset) = 0;
   virtual ICanSignalBuilder* WithMinimum(double minimum) = 0;
   virtual ICanSignalBuilder* WithMaximum(double maximum) = 0;
   virtual ICanSignalBuilder* WithUnit(const char* unit) = 0;
   virtual ICanSignalBuilder* WithComment(const char* comment) = 0;
   virtual ICanSignalBuilder* AddToMessage(ICanMessage* canMessage) = 0;
   virtual ICanSignal* Build(void) = 0;
};