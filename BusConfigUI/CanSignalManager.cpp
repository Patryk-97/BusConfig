#include "CanSignalManager.h"

void CanSignalManager::Modify(ICanBusConfig* canBusConfig, const QString& canSignalName,
   const QString& data, uint8_t column)
{
   if (canBusConfig)
   {
      if (auto canSignal = canBusConfig->GetSignalByName(canSignalName.toUtf8()); canSignal)
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
                  break;
               }
               case VALUE_TYPE_POS:
               {

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