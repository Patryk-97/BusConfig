#include "CanNetworkManager.h"
#include "Conversions.h"

bool CanNetworkManager::Validate(ICanNetwork* canNetwork, const QString& name, const QString& data, uint8_t column, QString& newData)
{
   // locals
   bool rV { true };

   enum class ValidationType_e
   {
      DOUBLE = 0,
      UINT = 1,
      INT = 2
   };

   auto validate = [&newData](ValidationType_e validationType, const QString& data)
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

   if (canNetwork)
   {
      if (column < PROPERTIES_COUNT)
      {
         switch (column)
         {
            case NAME_POS:
            {
               break;
            }
            case PROTOCOL_POS: case COMMENT_POS:
            {
               newData = data;
               break;
            }
            default:
            {
               break;
            }
         }
      }
   }

   return rV;
}

QString CanNetworkManager::GetData(ICanNetwork* canNetwork, const QString& name, uint8_t column)
{
   // locals
   QString previousData;

   if (canNetwork)
   {
      if (column < PROPERTIES_COUNT)
      {
         switch (column)
         {
            case NAME_POS:
            {
               previousData = canNetwork->GetName();
               break;
            }
            case PROTOCOL_POS:
            {
               previousData = canNetwork->GetProtocol();
               break;
            }
            case COMMENT_POS:
            {
               previousData = canNetwork->GetComment();
               break;
            }
            default:
            {
               break;
            }
         }
      }
   }

   return previousData;
}

void CanNetworkManager::Modify(ICanNetwork* canNetwork, const QString& name, const QString& data, uint8_t column)
{
   if (canNetwork)
   {
      if (column < PROPERTIES_COUNT)
      {
         switch (column)
         {
            case NAME_POS:
            {
               canNetwork->ModifyName(data.toUtf8());
               break;
            }
            case PROTOCOL_POS:
            {
               canNetwork->ModifyProtocol(data.toUtf8());
               break;
            }
            case COMMENT_POS:
            {
               canNetwork->ModifyComment(data.toUtf8());
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