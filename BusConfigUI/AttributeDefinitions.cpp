#include "AttributeDefinitions.h"
#include "ui_AttributeDefinitions.h"
#include "helpers.h"
#include "ICanEnumAttribute.h"
#include "ICanAttributeManager.h"
#include "Conversions.h"

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
   this->ui->tableWidget->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
}

AttributeDefinitions::~AttributeDefinitions()
{
   delete ui;
}

bool AttributeDefinitions::Create(ICanBusConfig* canBusConfig)
{
   // locals
   bool rV { false };

   if (canBusConfig)
   {
      this->ui->tableWidget->clear();

      QStringList headerLabels;
      headerLabels << "Object type" << "Name" << "Value type" << "Minimum" << "Maximum" << "Default";
       
      size_t attributesCount = canBusConfig->GetAttributesCount();
      this->ui->tableWidget->setRowCount(attributesCount);
      this->ui->tableWidget->setColumnCount(headerLabels.size());
      this->ui->tableWidget->setHorizontalHeaderLabels(headerLabels);

      for (size_t i = 0; i < attributesCount; i++)
      {
         if (auto attribute = canBusConfig->GetAttributeByIndex(i); attribute)
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

            this->ui->tableWidget->setItem(i, 0, new QTableWidgetItem { icon, OBJECT_TYPES[static_cast<int>(attribute->GetObjectType())] });
            this->ui->tableWidget->setItem(i, 1, new QTableWidgetItem { attribute->GetName() });
            this->ui->tableWidget->setItem(i, 2, new QTableWidgetItem { VALUE_TYPES[static_cast<int>(attribute->GetValueType())] });

            helpers::typecase(attribute,
               [&tableWidget = this->ui->tableWidget, &i] (const ICanIntAttribute* intAttribute)
               {
                  tableWidget->setItem(i, 3, new QTableWidgetItem{ toQString(intAttribute->GetMinimum()) });
                  tableWidget->setItem(i, 4, new QTableWidgetItem{ toQString(intAttribute->GetMaximum()) });
                  tableWidget->setItem(i, 5, new QTableWidgetItem{ toQString(intAttribute->GetDefaultValue()) });
               },
               [&tableWidget = this->ui->tableWidget, &i] (const ICanHexAttribute* hexAttribute)
               {
                  tableWidget->setItem(i, 3, new QTableWidgetItem{ toHexQString(hexAttribute->GetMinimum()) });
                  tableWidget->setItem(i, 4, new QTableWidgetItem{ toHexQString(hexAttribute->GetMaximum()) });
                  tableWidget->setItem(i, 5, new QTableWidgetItem{ toHexQString(hexAttribute->GetDefaultValue()) });
               },
               [&tableWidget = this->ui->tableWidget, &i] (const ICanFloatAttribute* floatAttribute)
               {
                  tableWidget->setItem(i, 3, new QTableWidgetItem{ toQString(floatAttribute->GetMinimum()) });
                  tableWidget->setItem(i, 4, new QTableWidgetItem{ toQString(floatAttribute->GetMaximum()) });
                  tableWidget->setItem(i, 5, new QTableWidgetItem{ toQString(floatAttribute->GetDefaultValue()) });
               },
               [&tableWidget = this->ui->tableWidget, &i] (const ICanStringAttribute* stringAttribute)
               {
                  tableWidget->setItem(i, 3, new QTableWidgetItem{ "-" });
                  tableWidget->setItem(i, 4, new QTableWidgetItem{ "-" });
                  tableWidget->setItem(i, 5, new QTableWidgetItem{ stringAttribute->GetDefaultValue() });
               },
               [&tableWidget = this->ui->tableWidget, &i] (const ICanEnumAttribute* enumAttribute)
               {
                  tableWidget->setItem(i, 3, new QTableWidgetItem{ "-" });
                  tableWidget->setItem(i, 4, new QTableWidgetItem{ "-" });
                  tableWidget->setItem(i, 5, new QTableWidgetItem{ enumAttribute->GetDefaultValue() });
               });
         }
      }
   }

   return rV;
}
