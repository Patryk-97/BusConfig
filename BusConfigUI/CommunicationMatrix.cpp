#include "CommunicationMatrix.h"
#include "ui_CommunicationMatrix.h"
#include "ICanManager.h"
#include <QtWidgets/qabstractbutton.h>
#include <qmessagebox.h>
#include <qlabel.h>

CommunicationMatrix::CommunicationMatrix(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::CommunicationMatrix)
{
   ui->setupUi(this);
}

CommunicationMatrix::~CommunicationMatrix()
{
   delete ui;
}

bool CommunicationMatrix::Create(ICanBusConfig* canBusConfig)
{
   // locals
   bool rV { false };
   if (canBusConfig)
   {
      this->ui->tableWidget->clear();
      uint8_t col = 0, row = 0;
      this->ui->tableWidget->setRowCount(canBusConfig->GetSignalsCount());
      this->ui->tableWidget->setColumnCount(canBusConfig->GetNodesCount());
      ICanManager::ForEachNetworkNode(canBusConfig, [this, &col]
         (const ICanNode* canNetworkNode)
      {
         if (canNetworkNode)
         {
            this->ui->tableWidget->setHorizontalHeaderItem(col++, 
               new QTableWidgetItem { QIcon { ":/BusConfigUI/Icons/network-node.png" },canNetworkNode->GetName() });
         }
      });

      ICanManager::ForEachSignal(canBusConfig, [this, &row]
         (const ICanSignal* canSignal)
      {
         if (canSignal)
         {
            this->ui->tableWidget->setVerticalHeaderItem(row++,
               new QTableWidgetItem { QIcon { ":/BusConfigUI/Icons/signal.png" }, canSignal->GetName() });
         }
      });

      QAbstractButton* button = this->ui->tableWidget->findChild<QAbstractButton*>();
      if (button)
      {
         QVBoxLayout* lay = new QVBoxLayout { button };
         lay->setContentsMargins(0, 0, 0, 0);
         QLabel* label = new QLabel { "Signals/node" };
         label->setAlignment(Qt::AlignCenter);
         label->setContentsMargins(2, 2, 2, 2);
         lay->addWidget(label);
      }
   }
   
   return rV;
}