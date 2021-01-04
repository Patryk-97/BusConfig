#include "CommunicationMatrix.h"
#include "ui_CommunicationMatrix.h"
#include "ICanManager.h"
#include <QtWidgets/qabstractbutton.h>
#include <qlabel.h>

CommunicationMatrix::CommunicationMatrix(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::CommunicationMatrix)
{
   ui->setupUi(this);
   Qt::WindowFlags flags { Qt::Window | Qt::WindowContextHelpButtonHint |
      Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint };
   this->ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
   this->setWindowFlags(flags);
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
      uint16_t col = 0, row = 0;
      this->ui->tableWidget->setRowCount(canBusConfig->GetSignalsCount());
      this->ui->tableWidget->setColumnCount(canBusConfig->GetNodesCount());
      ICanManager::ForEachNetworkNode(canBusConfig, [this, &col]
         (const ICanNode* canNetworkNode)
      {
         if (canNetworkNode)
         {
            this->ui->tableWidget->setHorizontalHeaderItem(col++, 
               new QTableWidgetItem { QIcon { ":/BusConfigUI/Icons/network-node.png" }, canNetworkNode->GetName() });
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

      for (size_t i = 0; i < canBusConfig->GetNodesCount(); ++i)
      {
         if (const auto canNetworkNode = canBusConfig->GetNodeByIndex(i); canNetworkNode)
         {
            for (size_t j = 0; j < canNetworkNode->GetMappedTxSignalsCount(); ++j)
            {
               if (const auto mappedTxSignal = canNetworkNode->GetMappedTxSignalByIndex(j); mappedTxSignal)
               {
                  size_t signalIndex = canBusConfig->GetSignalIndex(mappedTxSignal->GetName());
                  const auto mappedTxMessage = mappedTxSignal->GetMessage();
                  if (signalIndex != ICanBusConfig::INVALID_INDEX && mappedTxMessage)
                  {
                     auto item = new QTableWidgetItem{ QString { "<Tx> " } + mappedTxMessage->GetName() };
                     item->setForeground(QBrush { QColor { 0, 0, 255 }});
                     this->ui->tableWidget->setItem(signalIndex, i, item);
                  }
               }
            }

            for (size_t j = 0; j < canNetworkNode->GetMappedRxSignalsCount(); ++j)
            {
               if (const auto mappedRxSignal = canNetworkNode->GetMappedRxSignalByIndex(j); mappedRxSignal)
               {
                  size_t signalIndex = canBusConfig->GetSignalIndex(mappedRxSignal->GetName());
                  const auto mappedRxMessage = mappedRxSignal->GetMessage();
                  if (signalIndex != ICanBusConfig::INVALID_INDEX && mappedRxMessage)
                  {
                     auto item = new QTableWidgetItem{ mappedRxMessage->GetName() };
                     this->ui->tableWidget->setItem(signalIndex, i, item);
                  }
               }
            }
         }
      }
   }
   
   return rV;
}