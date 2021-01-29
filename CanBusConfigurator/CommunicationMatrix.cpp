#include "CommunicationMatrix.h"
#include "ui_CommunicationMatrix.h"
#include "ICanManager.h"
#include "TableWidgetItem.h"
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

   this->ui->tableWidget->setStyleSheet("QTableWidget::item { padding: 0 10px; border: 0; }");
   this->ui->tableWidget->horizontalHeader()->setStyleSheet(
      "QHeaderView::section { padding: 0 10px; border: 0; }");
   this->ui->tableWidget->setFocusPolicy(Qt::FocusPolicy::NoFocus);
   // because without it is some border after clicking on item in table widget
   this->ui->tableWidget->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
}

CommunicationMatrix::~CommunicationMatrix()
{
   delete ui;
}

bool CommunicationMatrix::Create(ICanNetwork* canNetwork)
{
   // locals
   bool rV { false };

   if (canNetwork)
   {
      this->ui->tableWidget->clear();
      uint16_t col = 0, row = 0;
      this->ui->tableWidget->setRowCount(canNetwork->GetSignalsCount());
      this->ui->tableWidget->setColumnCount(canNetwork->GetNodesCount());
      ICanManager::ForEachNetworkNode(canNetwork, [this, &col]
         (const ICanNode* canNetworkNode)
      {
         if (canNetworkNode)
         {
            this->ui->tableWidget->setHorizontalHeaderItem(col++, 
               new QTableWidgetItem { QIcon { ":/CanBusConfigurator/Icons/network-node.png" }, canNetworkNode->GetName() });
         }
      });

      ICanManager::ForEachSignal(canNetwork, [this, &row]
         (const ICanSignal* canSignal)
      {
         if (canSignal)
         {
            this->ui->tableWidget->setVerticalHeaderItem(row++,
               new QTableWidgetItem { QIcon { ":/CanBusConfigurator/Icons/signal.png" }, canSignal->GetName() });
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

      for (size_t i = 0; i < canNetwork->GetNodesCount(); ++i)
      {
         if (const auto canNetworkNode = canNetwork->GetNodeByIndex(i); canNetworkNode)
         {
            for (size_t j = 0; j < canNetworkNode->GetMappedTxSignalsCount(); ++j)
            {
               if (const auto mappedTxSignal = canNetworkNode->GetMappedTxSignalByIndex(j); mappedTxSignal)
               {
                  size_t signalIndex = canNetwork->GetSignalIndex(mappedTxSignal->GetName());
                  const auto mappedTxMessage = mappedTxSignal->GetMessage();
                  if (signalIndex != ICanNetwork::INVALID_INDEX && mappedTxMessage)
                  {
                     auto item = new TableWidgetItem{ QString { "<Tx> " } + mappedTxMessage->GetName() };
                     item->setForeground(QBrush { QColor { 0, 0, 255 }});
                     this->ui->tableWidget->setItem(signalIndex, i, item);
                  }
               }
            }

            for (size_t j = 0; j < canNetworkNode->GetMappedRxSignalsCount(); ++j)
            {
               if (const auto mappedRxSignal = canNetworkNode->GetMappedRxSignalByIndex(j); mappedRxSignal)
               {
                  size_t signalIndex = canNetwork->GetSignalIndex(mappedRxSignal->GetName());
                  const auto mappedRxMessage = mappedRxSignal->GetMessage();
                  if (signalIndex != ICanNetwork::INVALID_INDEX && mappedRxMessage)
                  {
                     auto item = new TableWidgetItem{ QString { "<Rx> " } + mappedRxMessage->GetName() };
                     item->setForeground(QBrush{ QColor { 134, 67, 67 } });
                     this->ui->tableWidget->setItem(signalIndex, i, item);
                  }
               }
            }
         }
      }
   }
   
   return rV;
}