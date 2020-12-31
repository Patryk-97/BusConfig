#include "CanMessageManager.h"

void CanMessageManager::Modify(ICanBusConfig* canBusConfig, const QString& canMessageName,
   const QString& data, uint8_t column)
{
   if (canBusConfig)
   {
      if (auto canMessage = canBusConfig->GetMessageByName(canMessageName.toUtf8()); canMessage)
      {
         if (column < PROPERTIES_COUNT)
         {
            switch (column)
            {
               case NAME_POS:
               {
                  canMessage->ModifyName(data.toUtf8());
                  break;
               }
               case ID_POS:
               {
                  canMessage->ModifyId(data.toUInt());
                  break;
               }
               case ID_FORMAT_POS:
               {

                  break;
               }
               case SIZE_POS:
               {
                  canMessage->ModifySize(data.toUInt());
                  break;
               }
               case TX_METHOD_POS:
               {

                  break;
               }
               case CYCLE_TIME_POS:
               {
                  canMessage->ModifyCycleTime(data.toUShort());
                  break;
               }
               case COMMENT_POS:
               {
                  canMessage->ModifyComment(data.toUtf8());
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