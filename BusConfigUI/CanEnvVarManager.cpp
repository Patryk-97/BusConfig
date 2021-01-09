#include "CanEnvVarManager.h"
#include "Conversions.h"

bool CanEnvVarManager::Validate(ICanBusConfig* canBusConfig, size_t index, const QString& data, uint8_t column)
{
   // locals
   bool rV { true };

   enum class ValidationType_e
   {
      DOUBLE = 0,
      UINT = 1,
      INT = 2
   };

   auto validate = [](ValidationType_e validationType, const QString& data)
   {
      try
      {
         if (validationType == ValidationType_e::DOUBLE)
         {
            auto val = std::stod(data.toStdString());
         }
         else if (validationType == ValidationType_e::INT)
         {
            auto val = std::stoi(data.toStdString());
         }
         else if (validationType == ValidationType_e::UINT)
         {
            auto val = std::stoul(data.toStdString());
         }
         return true;
      }
      catch (...)
      {
         return false;
      }
   };

   if (canBusConfig)
   {
      if (auto canSignal = canBusConfig->GetSignalByIndex(index); canSignal)
      {
         if (column < PROPERTIES_COUNT)
         {
            switch (column)
            {
               case NAME_POS: case TYPE_POS: case UNIT_POS:
               {
                  break;
               }
               case MINIMUM_POS:
               {
                  //canSignal->ModifyByteOrder(ByteOrder::MAP[data]);
                  break;
               }
               case MAXIMUM_POS:
               {
                  //canSignal->ModifyValueType(ValueType::MAP[data]);
                  break;
               }
               case INITIAL_VALUE_POS:
               {
                  //canSignal->ModifyFactor(data.toDouble());
                  break;
               }
               case LENGTH_POS:
               {
                  //canSignal->ModifyOffset(data.toDouble());
                  break;
               }
               case ACCESS_POS: COMMENT_POS:
               {
                  break;
               }
               default:
               {
                  break;
               }
            }
         }
      }
   }

   return rV;
}

QString CanEnvVarManager::GetData(ICanBusConfig* canBusConfig, size_t index, uint8_t column)
{
   // locals
   QString previousData;

   if (canBusConfig)
   {
      if (const auto canEnvVar = canBusConfig->GetEnvVarByIndex(index); canEnvVar)
      {
         if (column < PROPERTIES_COUNT)
         {
            switch (column)
            {
               case NAME_POS:
               {
                  previousData = canEnvVar->GetName();
                  break;
               }
               case TYPE_POS:
               {
                  previousData = TYPES[static_cast<int>(canEnvVar->GetType())];
                  break;
               }
               case UNIT_POS:
               {
                  previousData = canEnvVar->GetUnit();
                  break;
               }
               case MINIMUM_POS:
               {
                  //canSignal->ModifyByteOrder(ByteOrder::MAP[data]);
                  break;
               }
               case MAXIMUM_POS:
               {
                  //canSignal->ModifyValueType(ValueType::MAP[data]);
                  break;
               }
               case INITIAL_VALUE_POS:
               {
                  //canSignal->ModifyFactor(data.toDouble());
                  break;
               }
               case LENGTH_POS:
               {
                  //canSignal->ModifyOffset(data.toDouble());
                  break;
               }
               case ACCESS_POS:
               {
                  previousData = ACCESS_TYPES[static_cast<int>(canEnvVar->GetAccessType())];
                  break;
               }
               case COMMENT_POS:
               {
                  previousData = canEnvVar->GetComment();
                  break;
               }
               default:
               {
                  break;
               }
            }
         }
      }
   }

   return previousData;
}

void CanEnvVarManager::Modify(ICanBusConfig* canBusConfig, size_t index, const QString& data, uint8_t column)
{
   if (canBusConfig)
   {
      if (auto canEnvVar = canBusConfig->GetEnvVarByIndex(index); canEnvVar)
      {
         if (column < PROPERTIES_COUNT)
         {
            switch (column)
            {
               case NAME_POS:
               {
                  canEnvVar->ModifyName(data.toUtf8());
                  break;
               }
               case TYPE_POS:
               {
                  canEnvVar->ModifyType(Type::MAP[data]);
                  break;
               }
               case UNIT_POS:
               {
                  canEnvVar->ModifyUnit(data.toUtf8());
                  break;
               }
               case MINIMUM_POS:
               {
                  //canSignal->ModifyByteOrder(ByteOrder::MAP[data]);
                  break;
               }
               case MAXIMUM_POS:
               {
                  //canSignal->ModifyValueType(ValueType::MAP[data]);
                  break;
               }
               case INITIAL_VALUE_POS:
               {
                  //canSignal->ModifyFactor(data.toDouble());
                  break;
               }
               case LENGTH_POS:
               {
                  //canSignal->ModifyOffset(data.toDouble());
                  break;
               }
               case ACCESS_POS:
               {
                  canEnvVar->ModifyAccessType(AccessType::MAP[data]);
                  break;
               }
               case COMMENT_POS:
               {
                  canEnvVar->ModifyComment(data.toUtf8());
                  break;
               }
               default:
               {
                  break;
               }
            }
         }
      }
   }
}