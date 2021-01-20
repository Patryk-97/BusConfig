#pragma once

#include "ICanSignal.h"

class ICanSignalBuilder
{
protected:
   ICanSignalBuilder() {};
   virtual ~ICanSignalBuilder() {};
public:
   virtual void WithName(const char* name) = 0;
   virtual void WithStartBit(uint32_t startBit) = 0;
   virtual void WithSize(uint32_t size) = 0;
   virtual void WithByteOrder(ICanSignal::IByteOrder_e byteOrder) = 0;
   virtual void WithValueType(ICanSignal::IValueType_e valueType) = 0;
   virtual void WithFactor(double factor) = 0;
   virtual void WithOffset(double offset) = 0;
   virtual void WithMinimum(double minimum) = 0;
   virtual void WithMaximum(double maximum) = 0;
   virtual void WithUnit(const char* unit) = 0;
   virtual void WithComment(const char* comment) = 0;
   virtual void AddToMessage(ICanMessage* canMessage) = 0;
   virtual ICanSignal* Build(void) = 0;
};