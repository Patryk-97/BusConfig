#include "AttributeDefinitions.h"
#include "ui_AttributeDefinitions.h"
#include "helpers.h"
#include "ICanEnumAttribute.h"
#include "ICanAttributeManager.h"
#include "Conversions.h"
#include "CellComboDelegate.h"
#include "TableWidgetItem.h"

AttributeDefinitions::AttributeDefinitions(QWidget* parent) : 
   QDialog(parent),
   ui(new Ui::AttributeDefinitions)
{
   ui->setupUi(this);
   Qt::WindowFlags flags{ Qt::Window | Qt::WindowContextHelpButtonHint |
      Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint };
   this->ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
   this->setWindowFlags(flags);

   this->ui->tableWidget->setStyleSheet("QTableWidget::item { padding: 0 10px; border: 0; }");
   this->ui->tableWidget->horizontalHeader()->setStyleSheet(
      "QHeaderView::section { padding: 0 10px; border: 0; }");
   this->ui->tableWidget->setFocusPolicy(Qt::FocusPolicy::NoFocus);
   // because without it is some border after clicking on item in table widget
   //this->ui->tableWidget->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
}

AttributeDefinitions::~AttributeDefinitions()
{
   delete ui;
}

bool AttributeDefinitions::Create(ICanNetwork* canNetwork)
{
   // locals
   bool rV { false };

   if (canNetwork)
   {
      this->ui->tableWidget->clear();

      QStringList headerLabels;
      headerLabels << "Object type" << "Name" << "Value type" << "Minimum" << "Maximum" << "Enumarators" << "Default";
       
      size_t attributesCount = canNetwork->GetAttributesCount();
      this->ui->tableWidget->setRowCount(attributesCount);
      this->ui->tableWidget->setColumnCount(headerLabels.size());
      this->ui->tableWidget->setHorizontalHeaderLabels(headerLabels);

      CellComboDelegate* enumaratorsDelegate = new CellComboDelegate { 25, 150 };

      for (size_t i = 0; i < attributesCount; i++)
      {
         if (auto attribute = canNetwork->GetAttributeByIndex(i); attribute)
         {
            const QIcon icon = std::invoke([this, &attribute]
            {
               switch (attribute->GetObjectType())
               {
                  case ICanAttribute::IObjectType_e::NETWORK:
                  {
                     return QIcon{ ":/BusConfigUI/Icons/network.png" };
                  }
                  case ICanAttribute::IObjectType_e::NODE:
                  {
                     return QIcon{ ":/BusConfigUI/Icons/network-node.png" };
                  }
                  case ICanAttribute::IObjectType_e::MESSAGE:
                  {
                     return QIcon{ ":/BusConfigUI/Icons/message.png" };
                  }
                  case ICanAttribute::IObjectType_e::SIGNAL:
                  {
                     return QIcon{ ":/BusConfigUI/Icons/signal.png" };
                  }
                  case ICanAttribute::IObjectType_e::ENVIRONMENT_VARIABLE:
                  {
                     return QIcon{ ":/BusConfigUI/Icons/environment-variable.png" };
                  }
               }
               return QIcon {};
            });

            this->ui->tableWidget->setItem(i, 0, new TableWidgetItem<QString, false> { icon, OBJECT_TYPES[static_cast<int>(attribute->GetObjectType())] });
            this->ui->tableWidget->setItem(i, 1, new TableWidgetItem<QString, false> { attribute->GetName() });
            this->ui->tableWidget->setItem(i, 2, new TableWidgetItem<QString, false> { VALUE_TYPES[static_cast<int>(attribute->GetValueType())] });

            helpers::typecase(attribute,
               [&tableWidget = this->ui->tableWidget, &i, &enumaratorsDelegate] (const ICanIntAttribute* intAttribute)
               {
                  tableWidget->setItem(i, 3, new TableWidgetItem<int32_t, false> { toQString(intAttribute->GetMinimum()) });
                  tableWidget->setItem(i, 4, new TableWidgetItem<int32_t, false> { toQString(intAttribute->GetMaximum()) });

                  enumaratorsDelegate->addItems(QStringList {});

                  tableWidget->setItem(i, 5, new TableWidgetItem<QString, false> { "-" });
                  tableWidget->setItem(i, 6, new TableWidgetItem<int32_t, false> { toQString(intAttribute->GetDefaultValue()) });
               },
               [&tableWidget = this->ui->tableWidget, &i, &enumaratorsDelegate] (const ICanHexAttribute* hexAttribute)
               {
                  tableWidget->setItem(i, 3, new TableWidgetItem<int32_t, false> { toHexQString(hexAttribute->GetMinimum()) });
                  tableWidget->setItem(i, 4, new TableWidgetItem<int32_t, false> { toHexQString(hexAttribute->GetMaximum()) });

                  enumaratorsDelegate->addItems(QStringList{});

                  tableWidget->setItem(i, 5, new TableWidgetItem<QString, false> { "-" });
                  tableWidget->setItem(i, 6, new TableWidgetItem<int32_t, false> { toHexQString(hexAttribute->GetDefaultValue()) });
               },
               [&tableWidget = this->ui->tableWidget, &i, &enumaratorsDelegate] (const ICanFloatAttribute* floatAttribute)
               {
                  tableWidget->setItem(i, 3, new TableWidgetItem<double, false> { toQString(floatAttribute->GetMinimum()) });
                  tableWidget->setItem(i, 4, new TableWidgetItem<double, false> { toQString(floatAttribute->GetMaximum()) });

                  enumaratorsDelegate->addItems(QStringList{});

                  tableWidget->setItem(i, 5, new TableWidgetItem<QString, false> { "-" });
                  tableWidget->setItem(i, 6, new TableWidgetItem<double, false> { toQString(floatAttribute->GetDefaultValue()) });
               },
               [&tableWidget = this->ui->tableWidget, &i, &enumaratorsDelegate] (const ICanStringAttribute* stringAttribute)
               {
                  tableWidget->setItem(i, 3, new TableWidgetItem<QString, false> { "-" });
                  tableWidget->setItem(i, 4, new TableWidgetItem<QString, false> { "-" });

                  enumaratorsDelegate->addItems(QStringList{});

                  tableWidget->setItem(i, 5, new TableWidgetItem<QString, false> { "-" });
                  tableWidget->setItem(i, 6, new TableWidgetItem<QString, false> { stringAttribute->GetDefaultValue() });
               },
               [&tableWidget = this->ui->tableWidget, &i, &enumaratorsDelegate] (const ICanEnumAttribute* enumAttribute)
               {
                  tableWidget->setItem(i, 3, new TableWidgetItem<QString, false> { "-" });
                  tableWidget->setItem(i, 4, new TableWidgetItem<QString, false> { "-" });
                  
                  QStringList enumaratorsList;
                  for (size_t j = 0; j < enumAttribute->GetEnumaratorsCount(); j++)
                  {
                     if (const auto enumarator = enumAttribute->GetEnumarator(j); enumarator)
                     {
                        enumaratorsList.append(enumarator);
                     }
                  }
                  enumaratorsDelegate->addItems(enumaratorsList);

                  tableWidget->setItem(i, 5, new QTableWidgetItem { enumaratorsList[0] });
                  tableWidget->setItem(i, 6, new TableWidgetItem<QString, false> { enumAttribute->GetDefaultValue() });
               });
         }
      }

      this->ui->tableWidget->setItemDelegateForColumn(5, enumaratorsDelegate);
   }

   return rV;
}
