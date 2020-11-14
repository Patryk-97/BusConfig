#pragma once

#include <stdint.h>

class ICanSignal
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
      UNDEFINED = 0,
      BIG_ENDIAN = 1,
      LITTLE_ENDIAN = 2
   };

   enum class IValueType_e
   {
      UNDEFINED_TYPE = 0,
      UNSIGNED_TYPE = 1,
      SIGNED_TYPE = 2
   };

   virtual void Clear(void) = 0;
   virtual const char* GetName(void) const = 0;
   virtual bool IsNotMultiplexed(void) const = 0;
   virtual bool IsMultiplexerSwitch(void) const = 0;
   virtual bool IsMultiplexedSignal(void) const = 0;
   virtual IMuxType_e GetMuxType(void) const = 0;
   virtual const char* GetMuxTypeStr(void) const = 0;
   virtual const char* GetMuxIndicator(void) const = 0;
   virtual uint32_t GetMultiplexValue(void) const = 0;
   virtual uint32_t GetStartBit(void) const = 0;
   virtual uint32_t GetSize(void) const = 0;
   virtual IByteOrder_e GetByteOrder(void) const = 0;
   virtual const char* GetByteOrderStr(void) const = 0;
   virtual uint8_t GetValueTypeSymbol(void) const = 0;
   virtual IValueType_e GetValueType(void) const = 0;
   virtual const char* GetValueTypeStr(void) const = 0;
   virtual double GetFactor(void) const = 0;
   virtual double GetOffset(void) const = 0;
   virtual double GetMinimum(void) const = 0;
   virtual double GetMaximum(void) const = 0;
   virtual const char* GetUnit(void) const = 0;
   virtual size_t GetReceiversCount(void) const = 0;
   virtual const char* GetReceiver(size_t index) const = 0;

   // static variables
   static constexpr uint32_t NOT_MULTIPLEX_VALUE = 0xFFFFFFFF;
   static constexpr uint8_t UNDEFINED_BYTE_ORDER_SYMBOL = 0xFF;
   static constexpr uint8_t UNDEFINED_VALUE_TYPE_SYMBOL = 0x00;
};