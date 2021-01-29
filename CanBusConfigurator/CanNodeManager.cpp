#include "CanNodeManager.h"
#include "Conversions.h"

bool CanNodeManager::Validate(ICanNetwork* canNetwork, const QString& name, const QString& data, uint8_t column, QString& newData)
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

   if (canNetwork)
   {
      if (const auto canNode = canNetwork->GetNodeByName(name.toUtf8()); canNode)
      {
         if (column < PROPERTIES_COUNT)
         {
            switch (column)
            {
               case NAME_POS:
               {
                  break;
               }
               case NETWORK_POS:
               {
                  newData = data;
                  break;
               }
               case ADDRESS_POS:
               {
                  rV = validate(ValidationType_e::INT, data);
                  break;
               }
               case COMMENT_POS:
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
   }

   return rV;
}

QString CanNodeManager::GetData(ICanNetwork* canNetwork, const QString& name, uint8_t column)
{
   // locals
   QString previousData;

   if (canNetwork)
   {
      if (const auto canNode = canNetwork->GetNodeByName(name.toUtf8()); canNode)
      {
         if (column < PROPERTIES_COUNT)
         {
            switch (column)
            {
               case NAME_POS:
               {
                  previousData = canNode->GetName();
                  break;
               }
               case NETWORK_POS:
               {
                  break;
               }
               case ADDRESS_POS:
               {
                  previousData = toIntQString(canNode->GetAddress(), true);
                  break;
               }
               case COMMENT_POS:
               {
                  previousData = canNode->GetComment();
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

void CanNodeManager::Modify(ICanNetwork* canNetwork, const QString& name, const QString& data, uint8_t column)
{
   if (canNetwork)
   {
      if (const auto canNode = canNetwork->GetNodeByName(name.toUtf8()); canNode)
      {
         if (column < PROPERTIES_COUNT)
         {
            switch (column)
            {
               case NAME_POS:
               {
                  canNode->ModifyName(data.toUtf8());
                  break;
               }
               case NETWORK_POS:
               {
                  break;
               }
               case ADDRESS_POS:
               {
                  canNode->ModifyAddress(data.toInt(nullptr, 16));
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
}