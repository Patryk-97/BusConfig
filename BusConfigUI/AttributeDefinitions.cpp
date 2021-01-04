#include "AttributeDefinitions.h"
#include "ui_AttributeDefinitions.h"
#include "helpers.h"
#include "ICanAttribute.h"
#include "ICanIntAttribute.h"
#include "ICanHexAttribute.h"
#include "ICanFloatAttribute.h"
#include "ICanStringAttribute.h"
#include "ICanEnumAttribute.h"
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
         if (const auto attribute = canBusConfig->GetAttributeByIndex(i); attribute)
         {
            this->ui->tableWidget->setItem(i, 0, new QTableWidgetItem { OBJECT_TYPES[static_cast<int>(attribute->GetObjectType())] });
            this->ui->tableWidget->setItem(i, 1, new QTableWidgetItem { attribute->GetName() });
            this->ui->tableWidget->setItem(i, 2, new QTableWidgetItem { VALUE_TYPES[static_cast<int>(attribute->GetValueType())] });

            helpers::typecase(attribute,
               [&tableWidget = this->ui->tableWidget, &i] (ICanIntAttribute* intAttribute)
               {
                  tableWidget->setItem(i, 3, new QTableWidgetItem{ toQString(intAttribute->GetMinimum()) });
                  tableWidget->setItem(i, 4, new QTableWidgetItem{ toQString(intAttribute->GetMaximum()) });
                  tableWidget->setItem(i, 5, new QTableWidgetItem{ toQString(intAttribute->GetDefaultValue()) });
               },
               [&tableWidget = this->ui->tableWidget, &i] (ICanHexAttribute* hexAttribute)
               {
                  tableWidget->setItem(i, 3, new QTableWidgetItem{ toHexQString(hexAttribute->GetMinimum()) });
                  tableWidget->setItem(i, 4, new QTableWidgetItem{ toHexQString(hexAttribute->GetMaximum()) });
                  tableWidget->setItem(i, 5, new QTableWidgetItem{ toHexQString(hexAttribute->GetDefaultValue()) });
               },
               [&tableWidget = this->ui->tableWidget, &i] (ICanFloatAttribute* floatAttribute)
               {
                  tableWidget->setItem(i, 3, new QTableWidgetItem{ toQString(floatAttribute->GetMinimum()) });
                  tableWidget->setItem(i, 4, new QTableWidgetItem{ toQString(floatAttribute->GetMaximum()) });
                  tableWidget->setItem(i, 5, new QTableWidgetItem{ toQString(floatAttribute->GetDefaultValue()) });
               },
               [&tableWidget = this->ui->tableWidget, &i] (ICanStringAttribute* stringAttribute)
               {
                  tableWidget->setItem(i, 3, new QTableWidgetItem{ "-" });
                  tableWidget->setItem(i, 4, new QTableWidgetItem{ "-" });
                  tableWidget->setItem(i, 5, new QTableWidgetItem{ stringAttribute->GetDefaultValue() });
               },
               [&tableWidget = this->ui->tableWidget, &i] (ICanEnumAttribute* enumAttribute)
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
