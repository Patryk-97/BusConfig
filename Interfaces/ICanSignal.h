#pragma once

#include "ICanValueTable.h"
#include "ICanAttributeOwner.h"
#include <stdint.h>

class ICanMessage; // circular dependency
class ICanNetwork; // circular dependency

class ICanSignal : public ICanAttributeOwner
{
protected:
   ICanSignal() {};
   virtual ~ICanSignal() {};
public:

   enum class IMuxType_e
   {
      NOT_MULTIPLEXED = 0,
      MULTIPLEXER_SWITCH = 1,
      MULTIPLEXED_SIGNAL = 2
   };

   enum class IByteOrder_e
   {
      UNDEFINED = -1,
      BIG_ENDIAN = 0,
      LITTLE_ENDIAN = 1
   };

   enum class IValueType_e
   {
      UNDEFINED_TYPE = -1,
      UNSIGNED_TYPE = 0,
      SIGNED_TYPE = 1
   };

   virtual void Clear(void) = 0;

   virtual const char* GetName(void) const = 0;
   virtual void ModifyName(const char* name) = 0;

   virtual bool IsNotMultiplexed(void) const = 0;
   virtual bool IsMultiplexerSwitch(void) const = 0;
   virtual bool IsMultiplexedSignal(void) const = 0;
   virtual IMuxType_e GetMuxType(void) const = 0;
   virtual const char* GetMuxTypeStr(void) const = 0;
   virtual const char* GetMuxIndicator(void) const = 0;
   virtual uint32_t GetMultiplexValue(void) const = 0;

   virtual uint32_t GetStartBit(void) const = 0;
   virtual void ModifyStartBit(uint32_t startBit) = 0;

   virtual uint32_t GetSize(void) const = 0;
   virtual void ModifySize(uint32_t size) = 0;

   virtual IByteOrder_e GetByteOrder(void) const = 0;
   virtual void ModifyByteOrder(IByteOrder_e byteOrder) = 0;
   virtual uint8_t GetByteOrderSymbol(void) const = 0;

   virtual IValueType_e GetValueType(void) const = 0;
   virtual void ModifyValueType(IValueType_e valueType) = 0;
   virtual uint8_t GetValueTypeSymbol(void) const = 0;

   virtual int32_t GetRawInitialValue(void) const = 0;
   virtual void ModifyRawInitialValue(int32_t initialValue) = 0;

   virtual double GetInitialValue(void) const = 0;
   virtual void ModifyInitialValue(double initialValue) = 0;

   virtual double GetFactor(void) const = 0;
   virtual void ModifyFactor(double factor) = 0;

   virtual double GetOffset(void) const = 0;
   virtual void ModifyOffset(double offset) = 0;

   virtual double GetMinimum(void) const = 0;
   virtual void ModifyMinimum(double minimum) = 0;

   virtual double GetMaximum(void) const = 0;
   virtual void ModifyMaximum(double maximum) = 0;

   virtual const char* GetUnit(void) const = 0;
   virtual void ModifyUnit(const char * unit) = 0;

   virtual size_t GetReceiversCount(void) const = 0;
   virtual const char* GetReceiver(size_t index) const = 0;
   virtual ICanMessage* GetMessage(void) const = 0;
   virtual ICanValueTable* GetValueTable(void) const = 0;

   virtual const char* GetComment(void) const = 0;
   virtual void ModifyComment(const char* comment) = 0;

   virtual ICanNetwork* GetNetwork(void) const = 0;

   virtual const char* ToString(void) = 0;

   // static variables
   static constexpr uint32_t INVALID_MULTIPLEX_VALUE = 0xFFFFFFFF;
   static constexpr uint8_t UNDEFINED_BYTE_ORDER_SYMBOL = 0xFF;
   static constexpr uint8_t UNDEFINED_VALUE_TYPE_SYMBOL = 0x00;
   static constexpr uint32_t DEFAULT_RAW_INITIAL_VALUE = 0;

   static constexpr const char * RAW_INITIAL_VALUE = "GenSigStartValue";
};