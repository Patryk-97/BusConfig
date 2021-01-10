#pragma once

#include <qstring.h>
#include "ICanBusConfig.h"
#include <string_view>
#include <qtableview.h>
#include <qmap.h>
#include <array>

class CanEnvVarManager
{
public:

   constexpr static uint8_t NAME_POS = 0;
   constexpr static uint8_t TYPE_POS = 1;
   constexpr static uint8_t UNIT_POS = 2;
   constexpr static uint8_t MINIMUM_POS = 3;
   constexpr static uint8_t MAXIMUM_POS = 4;
   constexpr static uint8_t INITIAL_VALUE_POS = 5;
   constexpr static uint8_t LENGTH_POS = 6;
   constexpr static uint8_t ACCESS_POS = 7;
   constexpr static uint8_t VALUE_TABLE_POS = 8;
   constexpr static uint8_t COMMENT_POS = 9;
   constexpr static uint8_t PROPERTIES_COUNT = COMMENT_POS + 1;

   constexpr static std::string_view NAME = "Name";
   constexpr static std::string_view TYPE = "Type";
   constexpr static std::string_view UNIT = "Unit";
   constexpr static std::string_view MINIMUM = "Minimum";
   constexpr static std::string_view MAXIMUM = "Maximum";
   constexpr static std::string_view INITIAL_VALUE = "Initial value";
   constexpr static std::string_view LENGTH = "Length (Bytes)";
   constexpr static std::string_view ACCESS = "Access";
   constexpr static std::string_view VALUE_TABLE = "Value table";
   constexpr static std::string_view COMMENT = "Comment";

   constexpr static std::array<std::string_view, PROPERTIES_COUNT> PROPERTIES =
   {
      NAME, TYPE, UNIT, MINIMUM, MAXIMUM, INITIAL_VALUE, LENGTH, ACCESS, VALUE_TABLE, COMMENT
   };

   class Type
   {
   public:
      Type() = delete;

      constexpr static std::string_view INTEGER = "Integer";
      constexpr static std::string_view FLOAT = "Float";
      constexpr static std::string_view STRING = "String";
      constexpr static std::string_view DATA = "Data";

      constexpr static std::string_view DEFAULT = INTEGER;

      const static inline QMap<QString, ICanEnvVar::Type_e> MAP
      {
         { INTEGER.data(), ICanEnvVar::Type_e::INTEGER },
         { FLOAT.data(), ICanEnvVar::Type_e::FLOAT },
         { STRING.data(), ICanEnvVar::Type_e::STRING },
         { DATA.data(), ICanEnvVar::Type_e::DATA }
      };
   };

   class AccessType
   {
   public:
      AccessType() = delete;

      constexpr static std::string_view UNRESTRICTED = "Unrestricted";
      constexpr static std::string_view READ = "Read";
      constexpr static std::string_view WRITE = "Write";
      constexpr static std::string_view READ_WRITE = "Read & Write";

      constexpr static std::string_view DEFAULT = UNRESTRICTED;

      const static inline QMap<QString, ICanEnvVar::AccessType_e> MAP
      {
         { UNRESTRICTED.data(), ICanEnvVar::AccessType_e::UNRESTRICTED },
         { READ.data(), ICanEnvVar::AccessType_e::READ },
         { WRITE.data(), ICanEnvVar::AccessType_e::WRITE },
         { READ_WRITE.data(), ICanEnvVar::AccessType_e::READ_WRITE }
      };
   };

   const static inline QStringList TYPES { Type::INTEGER.data(), Type::FLOAT.data(), Type::STRING.data(), Type::DATA.data() };

   const static inline QStringList ACCESS_TYPES { AccessType::UNRESTRICTED.data(), AccessType::READ.data(),
      AccessType::WRITE.data(), AccessType::READ_WRITE.data() };

   static bool Validate(ICanBusConfig* canBusConfig, const QString& name, const QString& data, uint8_t column, QString& newData);
   static QString GetData(ICanBusConfig* canBusConfig, const QString& name, uint8_t column);
   static void Modify(ICanBusConfig* canBusConfig, const QString& name, const QString& data, uint8_t column);
};