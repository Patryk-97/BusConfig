#pragma once

#include "ICanNetwork.h"
#include <QDialog>
#include <string_view>
#include <qmap.h>
#include <qstringlist>

namespace Ui
{
   class AttributeDefinitions;
}

class AttributeDefinitions : public QDialog
{
   Q_OBJECT
public:
   explicit AttributeDefinitions(QWidget* parent = nullptr);
   ~AttributeDefinitions();

   bool Create(ICanNetwork* canNetwork);

private:
   Ui::AttributeDefinitions* ui;

   class ValueType
   {
   public:
      ValueType() = delete;

      static constexpr std::string_view UNDEFINED = "Undefined";
      static constexpr std::string_view INTEGER = "Integer";
      static constexpr std::string_view HEX = "Hex";
      static constexpr std::string_view FLOAT = "Float";
      static constexpr std::string_view STRING = "String";
      static constexpr std::string_view ENUMERATION = "Enumaration";
   };

   class ObjectType
   {
   public:
      ObjectType() = delete;

      static constexpr std::string_view UNDEFINED = "Undefined";
      static constexpr std::string_view NETWORK = "Network";
      static constexpr std::string_view NETWORK_NODE = "Network node";
      static constexpr std::string_view MESSAGE = "Message";
      static constexpr std::string_view SIGNAL = "Signal";
      static constexpr std::string_view ENVIRONMENT_VARIABLE = "Environment variable";
   };

   static const inline QStringList VALUE_TYPES
   {
      ValueType::UNDEFINED.data(), ValueType::INTEGER.data(), ValueType::HEX.data(), ValueType::FLOAT.data(),
      ValueType::STRING.data(), ValueType::ENUMERATION.data()
   };

   static const inline QStringList OBJECT_TYPES
   {
      ObjectType::UNDEFINED.data(), ObjectType::NETWORK.data(), ObjectType::NETWORK_NODE.data(), ObjectType::MESSAGE.data(),
      ObjectType::SIGNAL.data(), ObjectType::ENVIRONMENT_VARIABLE.data()
   };
};
