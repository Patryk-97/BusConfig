#pragma once

#include <qstring.h>
#include "ICanBusConfig.h"
#include <string_view>
#include <array>

class CanSignalManager
{
public:
   
   constexpr static uint8_t NAME_POS = 0;
   constexpr static uint8_t START_BIT_POS = 1;
   constexpr static uint8_t SIZE_POS = 2;
   constexpr static uint8_t BYTE_ORDER_POS = 3;
   constexpr static uint8_t VALUE_TYPE_POS = 4;
   constexpr static uint8_t FACTOR_POS = 5;
   constexpr static uint8_t OFFSET_POS = 6;
   constexpr static uint8_t MINIMUM_POS = 7;
   constexpr static uint8_t MAXIMUM_POS = 8;
   constexpr static uint8_t UNIT_POS = 9;
   constexpr static uint8_t PROPERTIES_COUNT = UNIT_POS + 1;

   constexpr static std::string_view NAME = "Name";
   constexpr static std::string_view START_BIT = "Start bit";
   constexpr static std::string_view SIZE = "Size";
   constexpr static std::string_view BYTE_ORDER = "Byte order";
   constexpr static std::string_view VALUE_TYPE = "Value type";
   constexpr static std::string_view FACTOR = "Factor";
   constexpr static std::string_view OFFSET = "Offset";
   constexpr static std::string_view MINIMUM = "Minimum";
   constexpr static std::string_view MAXIMUM = "Maximum";
   constexpr static std::string_view UNIT = "Unit";

   constexpr static std::array<std::string_view, PROPERTIES_COUNT> PROPERTIES = 
   {
      NAME, START_BIT, SIZE, BYTE_ORDER, VALUE_TYPE, FACTOR, OFFSET, MINIMUM, MAXIMUM, UNIT
   };

   class ByteOrder
   {
   public:
      ByteOrder() = delete;

      constexpr static std::string_view LITTLE_ENDIAN = "Little endian";
      constexpr static std::string_view BIG_ENDIAN = "Big endian";

      constexpr static std::string_view DEFAULT = LITTLE_ENDIAN;
   };

   class ValueType
   {
   public:
      ValueType() = delete;

      constexpr static std::string_view UNSIGNED = "Unsigned";
      constexpr static std::string_view SIGNED = "Signed";

      constexpr static std::string_view DEFAULT = SIGNED;
   };

   static void Modify(ICanBusConfig* canBusConfig, const QString& canSignalName,
      const QString& data, uint8_t column);
};