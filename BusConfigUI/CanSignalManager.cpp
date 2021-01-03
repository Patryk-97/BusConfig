#include "CanSignalManager.h"
#include "Conversions.h"

bool CanSignalManager::Validate(ICanBusConfig* canBusConfig, size_t index, const QString& data, uint8_t column)
{
   // locals
   bool rV { true };

   enum class ValidationType_e
   {
      DOUBLE = 0,
      UINT = 1,
      INT = 2
   };

   auto validate = [] (ValidationType_e validationType, const QString& data)
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
               case NAME_POS:
               {
                  break;
               }
               case START_BIT_POS: case SIZE_POS:
               {
                  rV = validate(ValidationType_e::UINT, data);
                  break;
               }
               case BYTE_ORDER_POS:
               {
                  break;
               }
               case VALUE_TYPE_POS:
               {

                  break;
               }
               case FACTOR_POS: case OFFSET_POS: case MINIMUM_POS: case MAXIMUM_POS:
               {
                  rV = validate(ValidationType_e::DOUBLE, data);
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

QString CanSignalManager::GetData(ICanBusConfig* canBusConfig, size_t index, uint8_t column)
{
   // locals
   QString previousData;

   if (canBusConfig)
   {
      if (auto canSignal = canBusConfig->GetSignalByIndex(index); canSignal)
      {
         if (column < PROPERTIES_COUNT)
         {
            switch (column)
            {
               case NAME_POS:
               {
                  previousData = canSignal->GetName();
                  break;
               }
               case START_BIT_POS:
               {
                  previousData = toQString(canSignal->GetStartBit());
                  break;
               }
               case SIZE_POS:
               {
                  previousData = toQString(canSignal->GetSize());
                  break;
               }
               case BYTE_ORDER_POS:
               {
                  break;
               }
               case VALUE_TYPE_POS:
               {

                  break;
               }
               case FACTOR_POS:
               {
                  previousData = toQString(canSignal->GetFactor());
                  break;
               }
               case OFFSET_POS:
               {
                  previousData = toQString(canSignal->GetMaximum());
                  break;
               }
               case MINIMUM_POS:
               {
                  previousData = toQString(canSignal->GetMinimum());
                  break;
               }
               case MAXIMUM_POS:
               {
                  previousData = toQString(canSignal->GetMaximum());
                  break;
               }
               case UNIT_POS:
               {
                  previousData = canSignal->GetUnit();
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

void CanSignalManager::Modify(ICanBusConfig* canBusConfig, size_t index, const QString& data, uint8_t column)
{
   if (canBusConfig)
   {
      if (auto canSignal = canBusConfig->GetSignalByIndex(index); canSignal)
      {
         if (column < PROPERTIES_COUNT)
         {
            switch (column)
            {
               case NAME_POS:
               {
                  canSignal->ModifyName(data.toUtf8());
                  break;
               }
               case START_BIT_POS:
               {
                  canSignal->ModifyStartBit(data.toUInt());
                  break;
               }
               case SIZE_POS:
               {
                  canSignal->ModifySize(data.toUInt());
                  break;
               }
               case BYTE_ORDER_POS:
               {
                  canSignal->ModifyByteOrder(ByteOrder::MAP[data]);
                  break;
               }
               case VALUE_TYPE_POS:
               {
                  canSignal->ModifyValueType(ValueType::MAP[data]);
                  break;
               }
               case FACTOR_POS:
               {
                  canSignal->ModifyFactor(data.toDouble());
                  break;
               }
               case OFFSET_POS:
               {
                  canSignal->ModifyOffset(data.toDouble());
                  break;
               }
               case MINIMUM_POS:
               {
                  canSignal->ModifyMinimum(data.toDouble());
                  break;
               }
               case MAXIMUM_POS:
               {
                  canSignal->ModifyMaximum(data.toDouble());
                  break;
               }
               case UNIT_POS:
               {
                  canSignal->ModifyUnit(data.toUtf8());
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