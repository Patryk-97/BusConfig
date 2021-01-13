#include "CanMessageManager.h"
#include "Conversions.h"

bool CanMessageManager::Validate(ICanBusConfig* canBusConfig, const QString& name, const QString& data, uint8_t column, QString& newData)
{
   // locals
   bool rV { true };

   enum class ValidationType_e
   {
      DOUBLE = 0,
      UINT = 1,
      INT = 2
   };

   auto validate = [&newData] (ValidationType_e validationType, const QString& data)
   {
      try
      {
         if (validationType == ValidationType_e::DOUBLE)
         {
            auto val = std::stod(data.toStdString());
            newData = QString::number(val);
         }
         else if (validationType == ValidationType_e::INT)
         {
            auto val = std::stoi(data.toStdString());
            newData = QString::number(val);
         }
         else if (validationType == ValidationType_e::UINT)
         {
            auto val = std::stoul(data.toStdString());
            newData = QString::number(val);
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
      if (auto canMessage = canBusConfig->GetMessageByName(name.toUtf8()); canMessage)
      {
         if (column < PROPERTIES_COUNT)
         {
            switch (column)
            {
               case NAME_POS:
               {
                  break;
               }
               case ID_POS:
               {
                  rV = validate(ValidationType_e::UINT, data);
                  break;
               }
               case ID_FORMAT_POS:
               {
                  newData = data;
                  break;
               }
               case SIZE_POS:
               {
                  rV = validate(ValidationType_e::UINT, data);
                  break;
               }
               case TX_METHOD_POS:
               {
                  newData = data;
                  break;
               }
               case CYCLE_TIME_POS:
               {
                  rV = validate(ValidationType_e::UINT, data);
                  break;
               }
               case COMMENT_POS:
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

QString CanMessageManager::GetData(ICanBusConfig* canBusConfig, const QString& name, uint8_t column)
{
   // locals
   QString previousData;

   if (canBusConfig)
   {
      if (auto canMessage = canBusConfig->GetMessageByName(name.toUtf8()); canMessage)
      {
         if (column < PROPERTIES_COUNT)
         {
            switch (column)
            {
               case NAME_POS:
               {
                  previousData = canMessage->GetName();
                  break;
               }
               case ID_POS:
               {
                  previousData = toQString(canMessage->GetId());
                  break;
               }
               case ID_FORMAT_POS:
               {
                  previousData = ID_FORMATS[static_cast<int>(canMessage->GetIdFormat())];
                  break;
               }
               case SIZE_POS:
               {
                  previousData = toQString(canMessage->GetSize());
                  break;
               }
               case TX_METHOD_POS:
               {
                  previousData = TX_METHODS[static_cast<int>(canMessage->GetTxMethod())];
                  break;
               }
               case CYCLE_TIME_POS:
               {
                  previousData = toQString(canMessage->GetCycleTime());
                  break;
               }
               case COMMENT_POS:
               {
                  previousData = canMessage->GetComment();
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

void CanMessageManager::Modify(ICanBusConfig* canBusConfig, const QString& name, const QString& data, uint8_t column)
{
   if (canBusConfig)
   {
      if (auto canMessage = canBusConfig->GetMessageByName(name.toUtf8()); canMessage)
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
                  canMessage->ModifyIdFormat(IdFormat::MAP[data.toUtf8().data()]);
                  break;
               }
               case SIZE_POS:
               {
                  canMessage->ModifySize(data.toUInt());
                  break;
               }
               case TX_METHOD_POS:
               {
                  canMessage->ModifyTxMethod(TxMethod::MAP[data.toUtf8().data()]);
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