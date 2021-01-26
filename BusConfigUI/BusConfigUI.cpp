#include "BusConfigUI.h"
#include "DllLoader.h"
#include "ICanBusConfig.h"
#include <qmessagebox.h>
#include <QFileDialog>
#include <qtoolbutton.h>
#include <sstream>
#include <concepts>
#include <qlabel.h>
#include <qlist.h>
#include <variant>
#include "ICanAttributeManager.h"
#include "ICanIntEnvVar.h"
#include "ICanFloatEnvVar.h"
#include "ICanStringEnvVar.h"
#include "ICanDataEnvVar.h"
#include "CanSignalManager.h"
#include "CanMessageManager.h"
#include "CanEnvVarManager.h"
#include "CanNetworkManager.h"
#include "CanNodeManager.h"
#include <algorithm>
#include <qlocale.h>
#include "LineEditFactory.h"
#include "Conversions.h"
#include "ComboDelegate.h"
#include "TableWidgetItem.h"

namespace ranges = std::ranges;

DllLoader<ICanBusConfig> dllLoader{ "BusConfigDll", "CanBusConfigInstanceCreate", "CanBusConfigInstanceDelete" };

#if defined(GetMessage)
#define GET_MESSAGE GetMessage
#undef GetMessage
#endif

bool LoadBusConfigDll(void)
{
   // locals
   bool rV { false };

   if (dllLoader.Load())
   {
      if (auto busConfig = dllLoader.pfCreate(); busConfig)
      {
         rV = true;
         dllLoader.pfDelete(busConfig);
      }
   }

   return rV;
}

BusConfigUI::BusConfigUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    if (LoadBusConfigDll())
    {
       QLocale englishLocale { QLocale::English, QLocale::UnitedKingdom };
       QLocale::setDefault(englishLocale);

       this->AddLog("Successfully loaded BusConfigDll.dll");
       this->setWindowState(Qt::WindowMaximized);
       this->canBusConfig = dllLoader.pfCreate();

       this->icons[Icon_e::MESSAGE] = QIcon{ ":/BusConfigUI/Icons/message.png" };
       this->icons[Icon_e::SIGNAL] = QIcon{ ":/BusConfigUI/Icons/signal.png" };
       this->icons[Icon_e::NETWORK_NODE] = QIcon{ ":/BusConfigUI/Icons/network-node.png" };
       this->icons[Icon_e::NETWORK] = QIcon{ ":/BusConfigUI/Icons/network.png" };
       this->icons[Icon_e::ENVIRONMENT_VARIABLE] = QIcon{ ":/BusConfigUI/Icons/environment-variable.png" };
       this->icons[Icon_e::VALUE_TABLE] = QIcon{ ":/BusConfigUI/Icons/value-table.png" };
       this->icons[Icon_e::ATTRIBUTES] = QIcon{ ":/BusConfigUI/Icons/attributes-list.png" };
       this->icons[Icon_e::HEX] = QIcon{ ":/BusConfigUI/Icons/hex.ico" };
       this->icons[Icon_e::DEC] = QIcon{ ":/BusConfigUI/Icons/dec.ico" };
       this->icons[Icon_e::COMMUNICATION_MATRIX] = QIcon{ ":/BusConfigUI/Icons/communication-matrix.png" };
       this->icons[Icon_e::CAN_MESSAGE_SIMULATOR] = QIcon{ ":/BusConfigUI/Icons/can-message-simulator.png" };

       foreach(QToolButton * button, ui.mainToolBar->findChildren<QToolButton*>())
       {
          if (button->text() == "Hex base" || button->text() == "Dec base")
          {
             button->setText("");
          }
          if (button->text() == "Communication matrix")
          {
             button->setText("");
             button->setIcon(this->icons[Icon_e::COMMUNICATION_MATRIX]);
          }
          if (button->text() == "Attribute definitions")
          {
             button->setText("");
             button->setIcon(this->icons[Icon_e::ATTRIBUTES]);
          }
          if (button->text() == "Can message simulator")
          {
             button->setText("");
             button->setIcon(this->icons[Icon_e::CAN_MESSAGE_SIMULATOR]);
          }
       }

       // add menu for right click for table widget
       this->ui.tableWidget_Properties->setContextMenuPolicy(Qt::CustomContextMenu);
       connect(this->ui.tableWidget_Properties, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowMenuForTableWidgetItem(const QPoint&)));

       // add menu for right click for tree widget
       this->ui.treeWidget_MainView->setContextMenuPolicy(Qt::CustomContextMenu);
       connect(this->ui.treeWidget_MainView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowMenuForTreeWidgetItem(const QPoint&)));

       this->ui.tableWidget_Properties->setStyleSheet("QTableWidget::item { padding: 0px 10px; border: 0; }");
       this->ui.tableWidget_Properties->horizontalHeader()->setStyleSheet(
         "QHeaderView::section { padding: 0 10px; border: 0; }");
       this->ui.tableWidget_Properties->setFocusPolicy(Qt::FocusPolicy::NoFocus);
       // because without it is some border after clicking on item in table widget

       // golden ratio proportion in splitter
       this->ui.splitter->setSizes({ static_cast<int>(10000 - 10000 / 1.618), static_cast<int>(10000 / 1.618) });
    }
    else
    {
      this->AddLog("Loading BusConfigDll.dll failed");
    }
}

BusConfigUI::~BusConfigUI()
{
   this->communicationMatrix->reject();
   delete this->communicationMatrix;
   this->attributeDefinitions->reject();
   delete this->attributeDefinitions;
   this->canMessageSimulator->reject();
   delete this->canMessageSimulator;
   this->canSignalCreator->reject();
   delete this->canSignalCreator;
   this->canMessageCreator->reject();
   delete this->canMessageCreator;
   this->canNodeCreator->reject();
   delete this->canNodeCreator;
}

void BusConfigUI::on_actionClear_triggered()
{
   this->Clear();
   this->AddLog("Database cleaned");
}

void BusConfigUI::on_actionOpen_triggered()
{
   QFileDialog dlgOpen { this };
   dlgOpen.setWindowTitle("Open bus configuration file");
   dlgOpen.setFileMode(QFileDialog::ExistingFile);
   dlgOpen.setNameFilter(trUtf8("Bus configuration files (*.dbc *.ldf)"));
   QStringList fileName;
   if (dlgOpen.exec())
   {
      this->Clear();
      fileName = dlgOpen.selectedFiles();
      this->LoadFile(fileName[0]);
   }
}

void BusConfigUI::on_actionSave_triggered()
{
   bool rV = this->canBusConfig->Save();
}

void BusConfigUI::on_actionExport_triggered()
{
   QString path = QFileDialog::getSaveFileName(0, tr("Save file (dbc)"), "", tr("dbc files (*.dbc)"));
   bool rV = this->canBusConfig->Export(path.toUtf8());
}

void BusConfigUI::on_actionBase_triggered()
{
   if (this->base == Base_e::DEC)
   {
      this->base = Base_e::HEX;
      this->ui.actionBase->setIcon(this->icons[Icon_e::DEC]);
      this->ui.actionBase->setText(QString { "Dec base" });
   }
   else // (this->base = Base_e::HEX)
   {
      this->base = Base_e::DEC;
      this->ui.actionBase->setIcon(this->icons[Icon_e::HEX]);
      this->ui.actionBase->setText(QString { "Hex base" });
   }

   for (auto toolButton : this->ui.mainToolBar->findChildren<QToolButton*>())
   {
      if (toolButton->text() == "Hex base" || toolButton->text() == "Dec base")
      {
         toolButton->setText("");
      }
   }
}

void BusConfigUI::on_actionAttribute_definitions_triggered()
{
   this->attributeDefinitions->Create(this->canBusConfig->GetNetworkFront());
   this->attributeDefinitions->show();
}

void BusConfigUI::on_actionCommunication_matrix_triggered()
{
   this->communicationMatrix->Create(this->canBusConfig->GetNetworkFront());
   this->communicationMatrix->show();
}

void BusConfigUI::on_actionCan_message_simulator_triggered()
{
   this->canMessageSimulator->Create(this->canBusConfig->GetNetworkFront());
   this->canMessageSimulator->show();
}

void BusConfigUI::on_actionExit_triggered()
{
   const auto buttonResult = QMessageBox::question(this, "BusConfigUI", tr("Are you sure to close application?\n"), QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
   if (buttonResult == QMessageBox::Yes)
   {
      this->Clear();
      QApplication::quit();
   }
}

void BusConfigUI::on_treeWidget_MainView_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
   this->currentTreeWidgetItem = current;
   this->BuildTable();

   this->isTableWidgetFilled = true;
}

void BusConfigUI::on_tableWidget_Properties_itemChanged(QTableWidgetItem* item)
{   
   if (this->isTableWidgetFilled)
   {
      const int row = item->row();
      const int column = item->column();
      const auto data = this->ui.tableWidget_Properties->item(row, column)->text();
      const auto itemType = this->ui.tableWidget_Properties->whatsThis();
      const auto itemName = this->ui.tableWidget_Properties->item(row, 0)->text();
      const auto canNetworkTreeItem = this->GetTreeItem(ItemId::NETWORK.data(), currentTreeWidgetItem);
      const auto canNetworkName = canNetworkTreeItem ? canNetworkTreeItem->text(0) : "";
      const auto canNetwork = this->canBusConfig->GetNetworkByName(canNetworkName.toUtf8());

      if (canNetwork)
      {
         if (itemType == ItemId::CAN_MESSAGE.data() || itemType == ItemId::CAN_MESSAGES.data() ||
            itemType == ItemId::CAN_TX_MESSAGES.data() || itemType == ItemId::CAN_RX_MESSAGES.data())
         {
            QString newData;
            if (CanMessageManager::Validate(canNetwork, itemName, data, column, newData))
            {
               if (column == 0)
               {
                  const QString name = CanMessageManager::GetData(canNetwork, itemName, column);
                  this->ChangeTreeWidgetItemName(name, data);
               }
               CanMessageManager::Modify(canNetwork, itemName, data, column);
            }
            else
            {
               newData = CanMessageManager::GetData(canNetwork, itemName, column);
            }
            this->ui.tableWidget_Properties->item(row, column)->setText(newData);
         }
         else if (itemType == ItemId::CAN_SIGNAL.data() || itemType == ItemId::CAN_SIGNALS.data() ||
            itemType == ItemId::CAN_MESSAGE_SIGNALS.data() || itemType == ItemId::CAN_MAPPED_TX_SIGNALS.data() ||
            itemType == ItemId::CAN_MAPPED_RX_SIGNALS.data() || itemType == ItemId::CAN_MAPPED_TX_MESSAGE_SIGNALS.data() ||
            itemType == ItemId::CAN_MAPPED_RX_MESSAGE_SIGNALS.data())
         {
            QString newData;
            if (CanSignalManager::Validate(canNetwork, itemName, data, column, newData))
            {
               if (column == 0)
               {
                  const QString name = CanSignalManager::GetData(canNetwork, itemName, column);
                  this->ChangeTreeWidgetItemName(name, data);
               }
               CanSignalManager::Modify(canNetwork, itemName, data, column);
            }
            else
            {
               newData = CanSignalManager::GetData(canNetwork, itemName, column);
            }
            this->ui.tableWidget_Properties->item(row, column)->setText(newData);
         }
         else if (itemType == ItemId::CAN_ENVIRONMENT_VARIABLE.data() || itemType == ItemId::CAN_ENVIRONMENT_VARIABLES.data())
         {
            QString newData;
            if (CanEnvVarManager::Validate(canNetwork, itemName, data, column, newData))
            {
               if (column == 0)
               {
                  const QString name = CanEnvVarManager::GetData(canNetwork, itemName, column);
                  this->ChangeTreeWidgetItemName(name, data);
               }
               CanEnvVarManager::Modify(canNetwork, itemName, data, column);
            }
            else
            {
               newData = CanEnvVarManager::GetData(canNetwork, itemName, column);
            }
            this->ui.tableWidget_Properties->item(row, column)->setText(newData);
         }
      }
   }

}

void BusConfigUI::on_tableWidget_Properties_cellChanged(int row, int column)
{
   if (this->isTableWidgetFilled)
   {
      const auto data = this->ui.tableWidget_Properties->item(row, column)->text();
      const auto itemType = this->ui.tableWidget_Properties->whatsThis();
      const auto itemName = this->ui.tableWidget_Properties->item(row, 0)->text();
      const auto canNetworkTreeItem = this->GetTreeItem(ItemId::NETWORK.data(), currentTreeWidgetItem);
      const auto canNetworkName = canNetworkTreeItem ? canNetworkTreeItem->text(0) : "";
      const auto canNetwork = this->canBusConfig->GetNetworkByName(canNetworkName.toUtf8());

      if (canNetwork)
      {
         if (itemType == ItemId::CAN_MESSAGE.data() || itemType == ItemId::CAN_MESSAGES.data() ||
            itemType == ItemId::CAN_TX_MESSAGES.data() || itemType == ItemId::CAN_RX_MESSAGES.data())
         {
            CanMessageManager::Modify(canNetwork, itemName, data, column);
         }
         else if (itemType == ItemId::CAN_SIGNAL.data() || itemType == ItemId::CAN_SIGNALS.data() ||
            itemType == ItemId::CAN_MESSAGE_SIGNALS.data() || itemType == ItemId::CAN_MAPPED_TX_SIGNALS.data() ||
            itemType == ItemId::CAN_MAPPED_RX_SIGNALS.data() || itemType == ItemId::CAN_MAPPED_TX_MESSAGE_SIGNALS.data() ||
            itemType == ItemId::CAN_MAPPED_RX_MESSAGE_SIGNALS.data())
         {
            CanSignalManager::Modify(canNetwork, itemName, data, column);
         }
         else if (itemType == ItemId::CAN_ENVIRONMENT_VARIABLE.data() || itemType == ItemId::CAN_ENVIRONMENT_VARIABLES.data())
         {
            CanEnvVarManager::Modify(canNetwork, itemName, data, column);
         }
      }
   }
}

// menu for right click for table widget
void BusConfigUI::ShowMenuForTableWidgetItem(const QPoint& pos)
{
   QPoint globalPos = this->ui.tableWidget_Properties->mapToGlobal(pos);

   if (const auto item = this->ui.tableWidget_Properties->itemAt(pos); item)
   {
      const auto row = item->row();
      const auto itemType = this->ui.tableWidget_Properties->whatsThis();
      QString networkName = this->ui.tableWidget_Properties->item(row, 1)->text();
      QString name = this->ui.tableWidget_Properties->item(row, 0)->text();
      if (auto canNetwork = this->canBusConfig->GetNetworkByName(networkName.toUtf8()); canNetwork)
      {
         //create right click menu item
         QMenu* menu = new QMenu { this->ui.tableWidget_Properties };

         if (itemType == ItemId::CAN_MESSAGE_SIGNALS.data())
         {
            this->ShowMenuForCanMessageSignalsTableItem(menu, canNetwork, name);
         }
         else if (itemType == ItemId::CAN_SIGNALS.data())
         {
            this->ShowMenuForCanSignalsTableItem(menu, canNetwork, name);
         }
         else if (itemType == ItemId::CAN_MESSAGES.data())
         {
            this->ShowMenuForCanMessagesTableItem(menu, canNetwork, name);
         }
         else if (itemType == ItemId::CAN_NETWORK_NODES.data())
         {
            this->ShowMenuForCanNodesTableItem(menu, canNetwork, name);
         }

         QAction* input = menu->exec(globalPos);
      }
   }
}

void BusConfigUI::ShowMenuForTreeWidgetItem(const QPoint& pos)
{
   QPoint globalPos = this->ui.treeWidget_MainView->mapToGlobal(pos);

   if (const auto item = this->ui.treeWidget_MainView->itemAt(pos); item)
   {
      const auto itemType = item->whatsThis(0);
      auto networkItem = GetTreeItem(ItemId::NETWORK.data(), item);
      QString networkName = networkItem ? networkItem->text(0) : "";
      QString name = item->text(0);
      if (auto canNetwork = this->canBusConfig->GetNetworkByName(networkName.toUtf8()); canNetwork)
      {
         //create right click menu item
         QMenu* menu = new QMenu { this->ui.treeWidget_MainView };

         if (itemType == ItemId::CAN_MESSAGE_SIGNALS.data())
         {
            this->ShowMenuForCanMessageSignalsTreeItem(menu, canNetwork);
         }
         else if (itemType == ItemId::CAN_SIGNALS.data())
         {
            this->ShowMenuForCanSignalsTreeItem(menu, canNetwork);
         }
         else if (itemType == ItemId::CAN_SIGNAL.data())
         {
            this->ShowMenuForCanSignalTreeItem(menu, canNetwork, name);
         }
         else if (itemType == ItemId::CAN_MESSAGES.data())
         {
            this->ShowMenuForCanMessagesTreeItem(menu, canNetwork);
         }
         else if (itemType == ItemId::CAN_MESSAGE.data())
         {
            this->ShowMenuForCanMessageTreeItem(menu, canNetwork, name);
         }
         else if (itemType == ItemId::CAN_NETWORK_NODES.data())
         {
            this->ShowMenuForCanNodesTreeItem(menu, canNetwork);
         }
         else if (itemType == ItemId::CAN_NETWORK_NODE.data())
         {
            this->ShowMenuForCanNodeTreeItem(menu, canNetwork, name);
         }

         QAction* input = menu->exec(globalPos);
      }
   }
}

void BusConfigUI::closeEvent(QCloseEvent* closeEvent)
{
   const auto buttonResult = QMessageBox::question(this, "BusConfigUI", tr("Are you sure to close application?\n"), QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
   if (buttonResult == QMessageBox::Yes)
   {
      this->Clear();
      QApplication::quit();
   }
   else
   {
      closeEvent->ignore();
   }
}

bool BusConfigUI::LoadFile(const QString& fileName)
{
   // locals
   bool rV { true };

   if (fileName.endsWith(".dbc"))
   {
      this->LoadDbcFile(fileName);
   }
   else if (fileName.endsWith(".ldf"))
   {
      this->AddLog(QString{ "Ldf bus configuration file has not supported yet." });
   }

   return rV;
}

bool BusConfigUI::LoadDbcFile(const QString& fileName)
{
   // locals
   bool rV { true };

   this->canBusConfig->Clear();
   if (this->canBusConfig->Load(fileName.toUtf8()))
   {
      this->AddLog(QString{ "Successfully loaded file: " } + fileName);
      if (const std::string log = this->canBusConfig->GetLog(); log != "")
      {
         this->AddLog(log.c_str());
      }
      this->BuildTree();
   }
   else
   {
      this->AddLog(QString{ "Loading file: " } + fileName + QString{ " failed" });
   }

   return rV;
}

void BusConfigUI::AddLog(const QString& log)
{
   this->ui.textBrowser_Log->append(log);
}

void BusConfigUI::BuildTree(void)
{
   this->ui.treeWidget_MainView->clear();
   auto networksTreeItem = new QTreeWidgetItem{ this->ui.treeWidget_MainView };
   this->ui.treeWidget_MainView->addTopLevelItem(networksTreeItem);
   networksTreeItem->setText(0, ItemId::NETWORKS.data());
   networksTreeItem->setIcon(0, this->icons[Icon_e::NETWORK]);
   networksTreeItem->setWhatsThis(0, ItemId::NETWORKS.data());
   networksTreeItem->setToolTip(0, "Networks");

   for (size_t i = 0; i < this->canBusConfig->GetNetworksCount(); i++)
   {
      if (const auto canNetwork = this->canBusConfig->GetNetworkByIndex(i); canNetwork)
      {
         auto networkTreeItem = new QTreeWidgetItem{ networksTreeItem };
         networkTreeItem->setText(0, canNetwork->GetName());
         networkTreeItem->setIcon(0, this->icons[Icon_e::NETWORK]);
         networkTreeItem->setWhatsThis(0, ItemId::NETWORK.data());
         networkTreeItem->setToolTip(0, "Can Network");

         auto canNetworkNodesTreeItem = new QTreeWidgetItem{ networkTreeItem };
         canNetworkNodesTreeItem->setText(0, "Network nodes");
         canNetworkNodesTreeItem->setIcon(0, this->icons[Icon_e::NETWORK_NODE]);
         canNetworkNodesTreeItem->setWhatsThis(0, ItemId::CAN_NETWORK_NODES.data());
         canNetworkNodesTreeItem->setToolTip(0, "Can network nodes");
         size_t canNodesCount = canNetwork->GetNodesCount();
         for (size_t j = 0; j < canNodesCount; j++)
         {
            if (const auto canNetworkNode = canNetwork->GetNodeByIndex(j); canNetworkNode)
            {
               auto canNetworkNodeTreeItem = new QTreeWidgetItem{ canNetworkNodesTreeItem };
               canNetworkNodeTreeItem->setText(0, canNetworkNode->GetName());
               canNetworkNodeTreeItem->setWhatsThis(0, ItemId::CAN_NETWORK_NODE.data());
               canNetworkNodeTreeItem->setToolTip(0, "Can network node");
               canNetworkNodeTreeItem->setIcon(0, this->icons[Icon_e::NETWORK_NODE]);

               // Tx Messages
               auto canTxMessagesTreeItem = new QTreeWidgetItem{ canNetworkNodeTreeItem };
               canTxMessagesTreeItem->setText(0, "Tx Messages");
               canTxMessagesTreeItem->setIcon(0, this->icons[Icon_e::MESSAGE]);
               canTxMessagesTreeItem->setWhatsThis(0, ItemId::CAN_TX_MESSAGES.data());
               canTxMessagesTreeItem->setToolTip(0, "Tx Messages");
               size_t canTxMessagesCount = canNetworkNode->GetTxMessagesCount();
               for (size_t k = 0; k < canTxMessagesCount; k++)
               {
                  if (const auto canTxMessage = canNetworkNode->GetTxMessageByIndex(k); canTxMessage)
                  {
                     auto canTxMessageTreeItem = new QTreeWidgetItem{ canTxMessagesTreeItem };
                     canTxMessageTreeItem->setText(0, canTxMessage->GetName());
                     canTxMessageTreeItem->setIcon(0, this->icons[Icon_e::MESSAGE]);
                     canTxMessageTreeItem->setWhatsThis(0, ItemId::CAN_MESSAGE.data());
                     canTxMessageTreeItem->setToolTip(0, "Tx Message");

                     // Mapped Tx Signals
                     auto canMappedTxSignalsTreeItem = new QTreeWidgetItem{ canTxMessageTreeItem };
                     canMappedTxSignalsTreeItem->setText(0, "Mapped Tx Signals");
                     canMappedTxSignalsTreeItem->setIcon(0, this->icons[Icon_e::SIGNAL]);
                     canMappedTxSignalsTreeItem->setWhatsThis(0, ItemId::CAN_MAPPED_TX_MESSAGE_SIGNALS.data());
                     canMappedTxSignalsTreeItem->setToolTip(0, "Mapped Tx Signals");
                     size_t canMappedTxSignalsCount = canTxMessage->GetSignalsCount();
                     for (size_t l = 0; l < canMappedTxSignalsCount; l++)
                     {
                        if (const auto canMappedTxSignal = canTxMessage->GetSignalByIndex(l); canMappedTxSignal)
                        {
                           auto canMappedTxSignalTreeItem = new QTreeWidgetItem{ canMappedTxSignalsTreeItem };
                           canMappedTxSignalTreeItem->setText(0, canMappedTxSignal->GetName());
                           canMappedTxSignalTreeItem->setIcon(0, this->icons[Icon_e::SIGNAL]);
                           canMappedTxSignalTreeItem->setWhatsThis(0, ItemId::CAN_SIGNAL.data());
                           canMappedTxSignalTreeItem->setToolTip(0, "Mapped Tx Signal");

                           this->AttachAttributesToTree(canMappedTxSignalTreeItem);
                           this->AttachValueTableToTree(canMappedTxSignalTreeItem);
                        }
                     }
                  }
               }

               // Rx Messages
               auto canRxMessagesTreeItem = new QTreeWidgetItem{ canNetworkNodeTreeItem };
               canRxMessagesTreeItem->setText(0, "Rx Messages");
               canRxMessagesTreeItem->setIcon(0, this->icons[Icon_e::MESSAGE]);
               canRxMessagesTreeItem->setWhatsThis(0, ItemId::CAN_RX_MESSAGES.data());
               canRxMessagesTreeItem->setToolTip(0, "Rx Messages");
               size_t canRxMessagesCount = canNetworkNode->GetRxMessagesCount();
               for (size_t k = 0; k < canRxMessagesCount; k++)
               {
                  if (const auto canRxMessage = canNetworkNode->GetRxMessageByIndex(k); canRxMessage)
                  {
                     auto rxMessageTreeItem = new QTreeWidgetItem{ canRxMessagesTreeItem };
                     rxMessageTreeItem->setText(0, canRxMessage->GetName());
                     rxMessageTreeItem->setIcon(0, this->icons[Icon_e::MESSAGE]);
                     rxMessageTreeItem->setWhatsThis(0, ItemId::CAN_MESSAGE.data());
                     rxMessageTreeItem->setToolTip(0, "Rx Message");

                     // Mapped Rx Signals
                     auto canMappedRxSignalsTreeItem = new QTreeWidgetItem{ rxMessageTreeItem };
                     canMappedRxSignalsTreeItem->setText(0, "Mapped Rx Signals");
                     canMappedRxSignalsTreeItem->setIcon(0, this->icons[Icon_e::SIGNAL]);
                     canMappedRxSignalsTreeItem->setWhatsThis(0, ItemId::CAN_MAPPED_RX_MESSAGE_SIGNALS.data());
                     canMappedRxSignalsTreeItem->setToolTip(0, "Mapped Rx Signals");
                     size_t canMappedRxSignalsCount = canNetworkNode->GetMappedRxSignalsCount();
                     for (size_t l = 0; l < canMappedRxSignalsCount; l++)
                     {
                        if (const auto canMappedRxSignal = canNetworkNode->GetMappedRxSignalByIndex(l); canMappedRxSignal)
                        {
                           if (const auto canReceiver = canMappedRxSignal->GetMessage(); canReceiver)
                           {
                              if (!std::strcmp(canReceiver->GetName(), canRxMessage->GetName()))
                              {
                                 auto canMappedRxSignalTreeItem = new QTreeWidgetItem{ canMappedRxSignalsTreeItem };
                                 canMappedRxSignalTreeItem->setText(0, canMappedRxSignal->GetName());
                                 canMappedRxSignalTreeItem->setIcon(0, this->icons[Icon_e::SIGNAL]);
                                 canMappedRxSignalTreeItem->setWhatsThis(0, ItemId::CAN_SIGNAL.data());
                                 canMappedRxSignalTreeItem->setToolTip(0, "Mapped Rx Signal");

                                 this->AttachAttributesToTree(canMappedRxSignalTreeItem);
                                 this->AttachValueTableToTree(canMappedRxSignalTreeItem);
                              }
                           }
                        }
                     }
                  }
               }

               // Mapped Tx Signals
               auto canMappedTxSignalsTreeItem = new QTreeWidgetItem{ canNetworkNodeTreeItem };
               canMappedTxSignalsTreeItem->setText(0, "Mapped Tx Signals");
               canMappedTxSignalsTreeItem->setIcon(0, this->icons[Icon_e::SIGNAL]);
               canMappedTxSignalsTreeItem->setWhatsThis(0, ItemId::CAN_MAPPED_TX_SIGNALS.data());
               canMappedTxSignalsTreeItem->setToolTip(0, "Mapped Tx Signals");
               size_t canMappedTxSignalsCount = canNetworkNode->GetMappedTxSignalsCount();
               for (size_t k = 0; k < canMappedTxSignalsCount; k++)
               {
                  if (const auto canMappedTxSignal = canNetworkNode->GetMappedTxSignalByIndex(k); canMappedTxSignal)
                  {
                     auto canMappedTxSignalTreeItem = new QTreeWidgetItem{ canMappedTxSignalsTreeItem };
                     canMappedTxSignalTreeItem->setText(0, canMappedTxSignal->GetName());
                     canMappedTxSignalTreeItem->setIcon(0, this->icons[Icon_e::SIGNAL]);
                     canMappedTxSignalTreeItem->setWhatsThis(0, ItemId::CAN_SIGNAL.data());
                     canMappedTxSignalTreeItem->setToolTip(0, "Mapped Tx Signal");

                     this->AttachAttributesToTree(canMappedTxSignalTreeItem);
                     this->AttachValueTableToTree(canMappedTxSignalTreeItem);
                  }
               }

               auto canMappedRxSignalsTreeItem = new QTreeWidgetItem{ canNetworkNodeTreeItem };
               canMappedRxSignalsTreeItem->setText(0, "Mapped Rx Signals");
               canMappedRxSignalsTreeItem->setIcon(0, this->icons[Icon_e::SIGNAL]);
               canMappedRxSignalsTreeItem->setWhatsThis(0, ItemId::CAN_MAPPED_RX_SIGNALS.data());
               canMappedRxSignalsTreeItem->setToolTip(0, "Mapped Rx Signals");
               size_t canMappedRxSignalsCount = canNetworkNode->GetMappedRxSignalsCount();
               for (size_t k = 0; k < canMappedRxSignalsCount; k++)
               {
                  if (const auto canMappedRxSignal = canNetworkNode->GetMappedRxSignalByIndex(k); canMappedRxSignal)
                  {
                     auto canMappedRxSignalTreeItem = new QTreeWidgetItem{ canMappedRxSignalsTreeItem };
                     canMappedRxSignalTreeItem->setText(0, canMappedRxSignal->GetName());
                     canMappedRxSignalTreeItem->setIcon(0, this->icons[Icon_e::SIGNAL]);
                     canMappedRxSignalTreeItem->setWhatsThis(0, ItemId::CAN_SIGNAL.data());
                     canMappedRxSignalTreeItem->setToolTip(0, "Mapped Rx Signal");

                     this->AttachAttributesToTree(canMappedRxSignalTreeItem);
                     this->AttachValueTableToTree(canMappedRxSignalTreeItem);
                  }
               }

               this->AttachAttributesToTree(canNetworkNodeTreeItem);
            }
         }

         auto canMessagesTreeItem = new QTreeWidgetItem{ networkTreeItem };
         canMessagesTreeItem->setText(0, "Messages");
         canMessagesTreeItem->setIcon(0, this->icons[Icon_e::MESSAGE]);
         canMessagesTreeItem->setWhatsThis(0, ItemId::CAN_MESSAGES.data());
         canMessagesTreeItem->setToolTip(0, "Can messages");
         size_t canMessagesCount = canNetwork->GetMessagesCount();
         for (size_t j = 0; j < canMessagesCount; j++)
         {
            if (const auto canMessage = canNetwork->GetMessageByIndex(j); canMessage)
            {
               auto canMessageTreeItem = new QTreeWidgetItem{ canMessagesTreeItem };
               canMessageTreeItem->setText(0, canMessage->GetName());
               canMessageTreeItem->setIcon(0, this->icons[Icon_e::MESSAGE]);
               canMessageTreeItem->setWhatsThis(0, ItemId::CAN_MESSAGE.data());
               canMessageTreeItem->setToolTip(0, "Can message");

               auto canMessageSignalsTreeItem = new QTreeWidgetItem{ canMessageTreeItem };
               canMessageSignalsTreeItem->setText(0, "Signals");
               canMessageSignalsTreeItem->setIcon(0, this->icons[Icon_e::SIGNAL]);
               canMessageSignalsTreeItem->setWhatsThis(0, ItemId::CAN_MESSAGE_SIGNALS.data());
               canMessageSignalsTreeItem->setToolTip(0, "Signals");
               size_t canMessageSignalsCount = canMessage->GetSignalsCount();
               for (size_t k = 0; k < canMessageSignalsCount; k++)
               {
                  if (const auto canMessageSignal = canMessage->GetSignalByIndex(k); canMessageSignal)
                  {
                     auto canMessageSignalTreeItem = new QTreeWidgetItem{ canMessageSignalsTreeItem };
                     canMessageSignalTreeItem->setText(0, canMessageSignal->GetName());
                     canMessageSignalTreeItem->setIcon(0, this->icons[Icon_e::SIGNAL]);
                     canMessageSignalTreeItem->setWhatsThis(0, ItemId::CAN_SIGNAL.data());
                     canMessageSignalTreeItem->setToolTip(0, "Signal");

                     this->AttachAttributesToTree(canMessageSignalTreeItem);
                     this->AttachValueTableToTree(canMessageSignalTreeItem);
                  }
               }

               this->AttachAttributesToTree(canMessageTreeItem);
            }
         }

         auto canSignalsTreeItem = new QTreeWidgetItem{ networkTreeItem };
         canSignalsTreeItem->setText(0, "Signals");
         canSignalsTreeItem->setWhatsThis(0, ItemId::CAN_SIGNALS.data());
         canSignalsTreeItem->setToolTip(0, "Can signals");
         canSignalsTreeItem->setIcon(0, this->icons[Icon_e::SIGNAL]);
         size_t canSignalsCount = canNetwork->GetSignalsCount();
         for (size_t j = 0; j < canSignalsCount; j++)
         {
            if (ICanSignal* canSignal = canNetwork->GetSignalByIndex(j); canSignal)
            {
               auto canSignalTreeItem = new QTreeWidgetItem{ canSignalsTreeItem };
               canSignalTreeItem->setText(0, canSignal->GetName());
               canSignalTreeItem->setIcon(0, this->icons[Icon_e::SIGNAL]);
               canSignalTreeItem->setWhatsThis(0, ItemId::CAN_SIGNAL.data());
               canSignalTreeItem->setToolTip(0, "Can signal");

               this->AttachAttributesToTree(canSignalTreeItem);
               this->AttachValueTableToTree(canSignalTreeItem);
            }
         }

         auto canEnvironmentVariablesTreeItem = new QTreeWidgetItem{ networkTreeItem };
         canEnvironmentVariablesTreeItem->setText(0, "Environment variables");
         canEnvironmentVariablesTreeItem->setWhatsThis(0, ItemId::CAN_ENVIRONMENT_VARIABLES.data());
         canEnvironmentVariablesTreeItem->setToolTip(0, "Can environment variables");
         canEnvironmentVariablesTreeItem->setIcon(0, this->icons[Icon_e::ENVIRONMENT_VARIABLE]);
         size_t canEnvironmentVariablesCount = canNetwork->GetEnvVarsCount();
         for (size_t j = 0; j < canEnvironmentVariablesCount; j++)
         {
            if (const auto canEnvVar = canNetwork->GetEnvVarByIndex(j); canEnvVar)
            {
               auto canEnvironmentVariableTreeItem = new QTreeWidgetItem{ canEnvironmentVariablesTreeItem };
               canEnvironmentVariableTreeItem->setText(0, canEnvVar->GetName());
               canEnvironmentVariableTreeItem->setIcon(0, this->icons[Icon_e::ENVIRONMENT_VARIABLE]);
               canEnvironmentVariableTreeItem->setWhatsThis(0, ItemId::CAN_ENVIRONMENT_VARIABLE.data());
               canEnvironmentVariableTreeItem->setToolTip(0, "Can environment variable");

               this->AttachAttributesToTree(canEnvironmentVariableTreeItem);
               this->AttachValueTableToTree(canEnvironmentVariableTreeItem);
            }
         }

         this->AttachAttributesToTree(networkTreeItem);
      }
   }
}

void BusConfigUI::BuildTable(void)
{
   if (this->currentTreeWidgetItem)
   {
      this->isTableWidgetFilled = false;

      this->ClearTableWidget();
      this->ui.tableWidget_Properties->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);

      const auto text = currentTreeWidgetItem->text(0);
      const auto itemType = currentTreeWidgetItem->whatsThis(0);
      const auto parent = currentTreeWidgetItem->parent();
      const auto parentText = (parent ? parent->text(0) : "");
      const auto parentItemType = (parent ? parent->whatsThis(0) : "");
      const auto canNetworkTreeItem = this->GetTreeItem(ItemId::NETWORK.data(), currentTreeWidgetItem);
      const auto canNetworkName = canNetworkTreeItem ? canNetworkTreeItem->text(0) : "";
      const auto canNetwork = this->canBusConfig->GetNetworkByName(canNetworkName.toUtf8());

      if (itemType == ItemId::NETWORKS.data())
      {
         this->BuildCanNetworksProperties();
      }
      else if (itemType == ItemId::NETWORK.data())
      {
         this->BuildCanNetworkProperties(canNetwork);
      }

      if (canNetwork)
      {
         this->ui.tableWidget_Properties->setWhatsThis(itemType);

         if (itemType == ItemId::CAN_NETWORK_NODE.data())
         {
            this->BuildCanNodeProperties(canNetwork, text);
         }
         else if (itemType == ItemId::CAN_NETWORK_NODES.data())
         {
            this->BuildCanNodesProperties(canNetwork);
         }
         else if (itemType == ItemId::CAN_MESSAGE.data())
         {
            this->ui.tableWidget_Properties->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Interactive);
            this->BuildCanMessageProperties(canNetwork, text);
         }
         else if (itemType == ItemId::CAN_MESSAGES.data())
         {
            this->BuildCanMessagesProperties(canNetwork);
         }
         else if (itemType == ItemId::CAN_TX_MESSAGES.data())
         {
            this->BuildCanTxMessagesProperties(canNetwork, parentText);
         }
         else if (itemType == ItemId::CAN_RX_MESSAGES.data())
         {
            this->BuildCanRxMessagesProperties(canNetwork, parentText);
         }
         else if (itemType == ItemId::CAN_SIGNAL.data())
         {
            this->ui.tableWidget_Properties->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Interactive);
            this->BuildCanSignalProperties(canNetwork, text);
         }
         else if (itemType == ItemId::CAN_SIGNALS.data())
         {
            this->BuildCanSignalsProperties(canNetwork);
         }
         else if (itemType == ItemId::CAN_MESSAGE_SIGNALS.data())
         {
            this->BuildCanMessageSignalsProperties(canNetwork, parentText);
         }
         else if (itemType == ItemId::CAN_MAPPED_TX_SIGNALS.data())
         {
            this->BuildCanMappedTxSignalsProperties(canNetwork, parentText);
         }
         else if (itemType == ItemId::CAN_MAPPED_TX_MESSAGE_SIGNALS.data())
         {
            this->BuildCanMappedTxMessageSignalsProperties(canNetwork, parentText);
         }
         else if (itemType == ItemId::CAN_MAPPED_RX_SIGNALS.data())
         {
            this->BuildCanMappedRxSignalsProperties(canNetwork, parentText);
         }
         else if (itemType == ItemId::CAN_MAPPED_RX_MESSAGE_SIGNALS.data())
         {
            const auto canNetworkNodeTreeItem = GetTreeItem(ItemId::CAN_NETWORK_NODE.data(), currentTreeWidgetItem);
            if (canNetworkNodeTreeItem)
            {
               this->BuildCanMappedRxMessageSignalsProperties(canNetwork,parentText, canNetworkNodeTreeItem->text(0));
            }
         }
         else if (itemType == ItemId::CAN_ENVIRONMENT_VARIABLE.data())
         {
            this->BuildCanEnvironmentVariableProperties(canNetwork, text);
         }
         else if (itemType == ItemId::CAN_ENVIRONMENT_VARIABLES.data())
         {
            this->BuildCanEnvironmentVariablesProperties(canNetwork);
         }
         else if (itemType == ItemId::VALUE_TABLE.data())
         {
            this->BuildCanValueTableProperties(canNetwork, parentItemType, parentText);
         }
         else if (itemType == ItemId::ATTRIBUTES.data() && parentItemType == ItemId::CAN_SIGNAL.data())
         {
            const auto canSignal = canNetwork->GetSignalByName(parentText.toUtf8());
            this->BuildAttributesProperties(canSignal);
         }
         else if (itemType == ItemId::ATTRIBUTES.data() && parentItemType == ItemId::CAN_MESSAGE.data())
         {
            const auto canMessage = canNetwork->GetMessageByName(parentText.toUtf8());
            this->BuildAttributesProperties(canMessage);
         }
         else if (itemType == ItemId::ATTRIBUTES.data() && parentItemType == ItemId::CAN_NETWORK_NODE.data())
         {
            const auto canNetworkNode = canNetwork->GetNodeByName(parentText.toUtf8());
            this->BuildAttributesProperties(canNetworkNode);
         }
         else if (itemType == ItemId::ATTRIBUTES.data() && parentItemType == ItemId::NETWORK.data())
         {
            this->BuildAttributesProperties(canNetwork);
         }
      }
   }

   this->isTableWidgetFilled = true;
}

void BusConfigUI::Clear(void)
{
   this->canBusConfig->Clear();
   this->ui.treeWidget_MainView->clear();
   this->ClearTableWidget();
}

void BusConfigUI::ClearTableWidget(void)
{
   for (size_t i = 0; i < this->ui.tableWidget_Properties->columnCount(); i++)
   {
      this->ui.tableWidget_Properties->setItemDelegateForColumn(i, nullptr);
   }
   this->ui.tableWidget_Properties->clear();
   this->ui.tableWidget_Properties->setRowCount(0);
   this->ui.tableWidget_Properties->setColumnCount(0);
}

void BusConfigUI::AttachAttributesToTree(QTreeWidgetItem* parent)
{
   auto attributesItem = new QTreeWidgetItem{ parent };
   attributesItem->setText(0, "Attributes");
   attributesItem->setIcon(0, this->icons[Icon_e::ATTRIBUTES]);
   attributesItem->setWhatsThis(0, ItemId::ATTRIBUTES.data());
   attributesItem->setToolTip(0, "Attributes");
}

void BusConfigUI::AttachValueTableToTree(QTreeWidgetItem* parent)
{
   auto valueTableItem = new QTreeWidgetItem{ parent };
   valueTableItem->setText(0, "Value table");
   valueTableItem->setIcon(0, this->icons[Icon_e::VALUE_TABLE]);
   valueTableItem->setWhatsThis(0, ItemId::VALUE_TABLE.data());
   valueTableItem->setToolTip(0, "Value table");
}

void BusConfigUI::BuildCanNetworkProperties(const ICanNetwork* canNetwork)
{
   if (canNetwork)
   {
      QStringList headerLabels;
      ranges::for_each(CanNetworkManager::PROPERTIES, [&headerLabels] (const std::string_view property)
      {
         headerLabels << property.data();
      });

      this->ui.tableWidget_Properties->setRowCount(1);
      this->ui.tableWidget_Properties->setColumnCount(headerLabels.size());
      this->ui.tableWidget_Properties->setHorizontalHeaderLabels(headerLabels);

      this->BuildCanNetworkRow(canNetwork, 0);
   }
}

void BusConfigUI::BuildCanNetworksProperties(void)
{
   size_t canNetworksCount = this->canBusConfig->GetNetworksCount();
   QStringList headerLabels;
   ranges::for_each(CanNetworkManager::PROPERTIES, [&headerLabels] (std::string_view property)
   {
      headerLabels << property.data();
   });

   this->ui.tableWidget_Properties->setRowCount(canNetworksCount);
   this->ui.tableWidget_Properties->setColumnCount(headerLabels.size());
   this->ui.tableWidget_Properties->setHorizontalHeaderLabels(headerLabels);

   for (size_t i = 0; i < canNetworksCount; i++)
   {
      this->BuildCanNetworkRow(this->canBusConfig->GetNetworkByIndex(i), i);
   }
}

void BusConfigUI::BuildCanNodeProperties(const ICanNetwork* canNetwork, const QString& nodeName)
{
   if (canNetwork)
   {
      QStringList headerLabels;
      ranges::for_each(CanNodeManager::PROPERTIES, [&headerLabels] (const std::string_view property)
      {
         headerLabels << property.data();
      });

      this->ui.tableWidget_Properties->setRowCount(1);
      this->ui.tableWidget_Properties->setColumnCount(headerLabels.size());
      this->ui.tableWidget_Properties->setHorizontalHeaderLabels(headerLabels);

      this->BuildCanNodeRow(canNetwork->GetNodeByName(nodeName.toUtf8()), 0);
   }
}

void BusConfigUI::BuildCanNodesProperties(const ICanNetwork* canNetwork)
{
   if (canNetwork)
   {
      size_t canNodesCount = canNetwork->GetNodesCount();
      QStringList headerLabels;
      ranges::for_each(CanNodeManager::PROPERTIES, [&headerLabels] (std::string_view property)
      {
         headerLabels << property.data();
      });

      this->ui.tableWidget_Properties->setRowCount(canNodesCount);
      this->ui.tableWidget_Properties->setColumnCount(headerLabels.size());
      this->ui.tableWidget_Properties->setHorizontalHeaderLabels(headerLabels);

      for (size_t i = 0; i < canNodesCount; i++)
      {
         this->BuildCanNodeRow(canNetwork->GetNodeByIndex(i), i);
      }
   }
}

void BusConfigUI::BuildCanMessageProperties(const ICanNetwork* canNetwork, const QString& messageName)
{
   if (canNetwork)
   {
      QStringList headerLabels;
      ranges::for_each(CanMessageManager::PROPERTIES, [&headerLabels](const std::string_view property)
      {
         headerLabels << property.data();
      });

      this->ui.tableWidget_Properties->setRowCount(1);
      this->ui.tableWidget_Properties->setColumnCount(headerLabels.size());
      this->ui.tableWidget_Properties->setHorizontalHeaderLabels(headerLabels);

      this->BuildCanMessageRow(canNetwork->GetMessageByName(messageName.toUtf8()), 0);

      this->SetComboDelegateForCanMessage();
   }
}

void BusConfigUI::BuildCanMessagesProperties(const ICanNetwork* canNetwork)
{
   if (canNetwork)
   {
      size_t canMessagesCount = canNetwork->GetMessagesCount();
      QStringList headerLabels;
      ranges::for_each(CanMessageManager::PROPERTIES, [&headerLabels](const std::string_view property)
      {
         headerLabels << property.data();
      });

      this->ui.tableWidget_Properties->setRowCount(canMessagesCount);
      this->ui.tableWidget_Properties->setColumnCount(headerLabels.size());
      this->ui.tableWidget_Properties->setHorizontalHeaderLabels(headerLabels);

      for (size_t i = 0; i < canMessagesCount; i++)
      {
         this->BuildCanMessageRow(canNetwork->GetMessageByIndex(i), i);
      }

      this->SetComboDelegateForCanMessage();
   }
}

void BusConfigUI::BuildCanTxMessagesProperties(const ICanNetwork* canNetwork, const QString& networkNodeName)
{
   if (canNetwork)
   {
      if (const auto canNetworkNode = canNetwork->GetNodeByName(networkNodeName.toUtf8()); canNetworkNode)
      {
         size_t canTxMessagesCount = canNetworkNode->GetTxMessagesCount();
         QStringList headerLabels;
         ranges::for_each(CanMessageManager::PROPERTIES, [&headerLabels] (const std::string_view property)
         {
            headerLabels << property.data();
         });

         this->ui.tableWidget_Properties->setRowCount(canTxMessagesCount);
         this->ui.tableWidget_Properties->setColumnCount(headerLabels.size());
         this->ui.tableWidget_Properties->setHorizontalHeaderLabels(headerLabels);

         for (size_t i = 0; i < canTxMessagesCount; i++)
         {
            this->BuildCanMessageRow(canNetworkNode->GetTxMessageByIndex(i), i);
         }

         this->SetComboDelegateForCanMessage();
      }
   }
}

void BusConfigUI::BuildCanRxMessagesProperties(const ICanNetwork* canNetwork, const QString& networkNodeName)
{
   if (canNetwork)
   {
      if (const auto canNetworkNode = canNetwork->GetNodeByName(networkNodeName.toUtf8()); canNetworkNode)
      {
         size_t canRxMessagesCount = canNetworkNode->GetRxMessagesCount();
         QStringList headerLabels;
         ranges::for_each(CanMessageManager::PROPERTIES, [&headerLabels](const std::string_view property)
         {
            headerLabels << property.data();
         });

         this->ui.tableWidget_Properties->setRowCount(canRxMessagesCount);
         this->ui.tableWidget_Properties->setColumnCount(headerLabels.size());
         this->ui.tableWidget_Properties->setHorizontalHeaderLabels(headerLabels);

         for (size_t i = 0; i < canRxMessagesCount; i++)
         {
            this->BuildCanMessageRow(canNetworkNode->GetRxMessageByIndex(i), i);
         }

         this->SetComboDelegateForCanMessage();
      }
   }
}

void BusConfigUI::BuildCanSignalProperties(const ICanNetwork* canNetwork, const QString& signalName)
{
   if (canNetwork)
   {
      QStringList headerLabels;
      ranges::for_each(CanSignalManager::PROPERTIES, [&headerLabels] (const std::string_view property)
      {
         headerLabels << property.data();
      });

      this->ui.tableWidget_Properties->setRowCount(1);
      this->ui.tableWidget_Properties->setColumnCount(headerLabels.size());
      this->ui.tableWidget_Properties->setHorizontalHeaderLabels(headerLabels);

      this->BuildCanSignalRow(canNetwork->GetSignalByName(signalName.toUtf8()), 0);

      this->SetComboDelegateForCanSignal();
   }
}

void BusConfigUI::BuildCanSignalsProperties(const ICanNetwork* canNetwork)
{
   if (canNetwork)
   {
      size_t canSignalsCount = canNetwork->GetSignalsCount();
      QStringList headerLabels;
      ranges::for_each(CanSignalManager::PROPERTIES, [&headerLabels](std::string_view property)
         { headerLabels << property.data(); });

      this->ui.tableWidget_Properties->setRowCount(canSignalsCount);
      this->ui.tableWidget_Properties->setColumnCount(headerLabels.size());
      this->ui.tableWidget_Properties->setHorizontalHeaderLabels(headerLabels);

      for (size_t i = 0; i < canSignalsCount; i++)
      {
         this->BuildCanSignalRow(canNetwork->GetSignalByIndex(i), i);
      }

      this->SetComboDelegateForCanSignal();
   }
}

void BusConfigUI::BuildCanMessageSignalsProperties(const ICanNetwork* canNetwork, const QString& messageName)
{
   if (canNetwork)
   {
      if (const auto message = canNetwork->GetMessageByName(messageName.toUtf8()); message)
      {
         size_t canMessageSignalsCount = message->GetSignalsCount();
         QStringList headerLabels;
         ranges::for_each(CanSignalManager::PROPERTIES, [&headerLabels](std::string_view property)
         {
            headerLabels << property.data();
         });

         this->ui.tableWidget_Properties->setRowCount(canMessageSignalsCount);
         this->ui.tableWidget_Properties->setColumnCount(headerLabels.size());
         this->ui.tableWidget_Properties->setHorizontalHeaderLabels(headerLabels);

         for (size_t i = 0; i < canMessageSignalsCount; i++)
         {
            this->BuildCanSignalRow(message->GetSignalByIndex(i), i);
         }

         this->SetComboDelegateForCanSignal();
      }
   }
}

void BusConfigUI::BuildCanMappedTxSignalsProperties(const ICanNetwork* canNetwork, const QString& networkNodeName)
{
   if (canNetwork)
   {
      if (const auto canNetworkNode = canNetwork->GetNodeByName(networkNodeName.toUtf8()); canNetworkNode)
      {
         size_t canMappedTxSignalsCount = canNetworkNode->GetMappedTxSignalsCount();
         QStringList headerLabels;
         ranges::for_each(CanSignalManager::PROPERTIES, [&headerLabels](std::string_view property)
         {
            headerLabels << property.data();
         });

         this->ui.tableWidget_Properties->setRowCount(canMappedTxSignalsCount);
         this->ui.tableWidget_Properties->setColumnCount(headerLabels.size());
         this->ui.tableWidget_Properties->setHorizontalHeaderLabels(headerLabels);

         for (size_t i = 0; i < canMappedTxSignalsCount; i++)
         {
            this->BuildCanSignalRow(canNetworkNode->GetMappedTxSignalByIndex(i), i);
         }

         this->SetComboDelegateForCanSignal();
      }
   }
}

void BusConfigUI::BuildCanMappedTxMessageSignalsProperties(const ICanNetwork* canNetwork, const QString& messageName)
{
   if (canNetwork)
   {
      if (const auto canMessage = canNetwork->GetMessageByName(messageName.toUtf8()); canMessage)
      {
         size_t canMappedTxSignalsCount = canMessage->GetSignalsCount();
         QStringList headerLabels;
         ranges::for_each(CanSignalManager::PROPERTIES, [&headerLabels](std::string_view property)
         {
            headerLabels << property.data();
         });

         this->ui.tableWidget_Properties->setRowCount(canMappedTxSignalsCount);
         this->ui.tableWidget_Properties->setColumnCount(headerLabels.size());
         this->ui.tableWidget_Properties->setHorizontalHeaderLabels(headerLabels);

         for (size_t i = 0; i < canMappedTxSignalsCount; i++)
         {
            this->BuildCanSignalRow(canMessage->GetSignalByIndex(i), i);
         }

         this->SetComboDelegateForCanSignal();
      }
   }
}

void BusConfigUI::BuildCanMappedRxSignalsProperties(const ICanNetwork* canNetwork, const QString& networkNodeName)
{
   if (canNetwork)
   {
      if (const auto canNetworkNode = canNetwork->GetNodeByName(networkNodeName.toUtf8()); canNetworkNode)
      {
         size_t canMappedRxSignalsCount = canNetworkNode->GetMappedRxSignalsCount();
         QStringList headerLabels;
         ranges::for_each(CanSignalManager::PROPERTIES, [&headerLabels](std::string_view property)
         {
            headerLabels << property.data();
         });

         this->ui.tableWidget_Properties->setRowCount(canMappedRxSignalsCount);
         this->ui.tableWidget_Properties->setColumnCount(headerLabels.size());
         this->ui.tableWidget_Properties->setHorizontalHeaderLabels(headerLabels);

         for (size_t i = 0; i < canMappedRxSignalsCount; i++)
         {
            this->BuildCanSignalRow(canNetworkNode->GetMappedRxSignalByIndex(i), i);
         }

         this->SetComboDelegateForCanSignal();
      }
   }
}

void BusConfigUI::BuildCanMappedRxMessageSignalsProperties(const ICanNetwork* canNetwork, const QString& messageName, const QString& networkNodeName)
{
   if (canNetwork)
   {
      if (const auto canMessage = canNetwork->GetMessageByName(messageName.toUtf8()); canMessage)
      {
         if (const auto canReceiver = canNetwork->GetNodeByName(networkNodeName.toUtf8()); canReceiver)
         {
            size_t canMappedRxSignalsCount = canReceiver->GetMappedRxSignalsCount();

            size_t mappedRxMessageSignalsCounter{};
            for (size_t i = 0; i < canMappedRxSignalsCount; i++)
            {
               if (const auto mappedRxSignal = canReceiver->GetMappedRxSignalByIndex(i); mappedRxSignal)
               {
                  if (const auto rxMessage = mappedRxSignal->GetMessage(); rxMessage)
                  {
                     if (!std::strcmp(rxMessage->GetName(), canMessage->GetName()))
                     {
                        ++mappedRxMessageSignalsCounter;
                     }
                  }
               }
            }

            QStringList headerLabels;
            ranges::for_each(CanSignalManager::PROPERTIES, [&headerLabels] (std::string_view property)
            {
               headerLabels << property.data();
            });

            this->ui.tableWidget_Properties->setRowCount(mappedRxMessageSignalsCounter);
            this->ui.tableWidget_Properties->setColumnCount(headerLabels.size());
            this->ui.tableWidget_Properties->setHorizontalHeaderLabels(headerLabels);

            mappedRxMessageSignalsCounter = 0;
            for (size_t i = 0; i < canMappedRxSignalsCount; i++)
            {
               if (const auto mappedRxSignal = canReceiver->GetMappedRxSignalByIndex(i); mappedRxSignal)
               {
                  if (const auto rxMessage = mappedRxSignal->GetMessage(); rxMessage)
                  {
                     if (!std::strcmp(rxMessage->GetName(), canMessage->GetName()))
                     {
                        this->BuildCanSignalRow(canReceiver->GetMappedRxSignalByIndex(i), mappedRxMessageSignalsCounter++);
                     }
                  }
               }
            }

            this->SetComboDelegateForCanSignal();
         }
      }
   }
}

void BusConfigUI::BuildCanEnvironmentVariableProperties(const ICanNetwork* canNetwork, const QString& envVarName)
{
   if (canNetwork)
   {
      QStringList headerLabels;
      ranges::for_each(CanEnvVarManager::PROPERTIES, [&headerLabels](std::string_view property)
      {
         headerLabels << property.data();
      });

      this->ui.tableWidget_Properties->setRowCount(1);
      this->ui.tableWidget_Properties->setColumnCount(headerLabels.size());
      this->ui.tableWidget_Properties->setHorizontalHeaderLabels(headerLabels);

      this->BuildCanEnvVarRow(canNetwork->GetEnvVarByName(envVarName.toUtf8()), 0);

      this->SetComboDelegateForCanEnvVar();
   }
}

void BusConfigUI::BuildCanEnvironmentVariablesProperties(const ICanNetwork* canNetwork)
{
   if (canNetwork)
   {
      size_t canEnvVarsCount = canNetwork->GetEnvVarsCount();
      QStringList headerLabels;
      ranges::for_each(CanEnvVarManager::PROPERTIES, [&headerLabels] (std::string_view property)
      {
         headerLabels << property.data();
      });

      this->ui.tableWidget_Properties->setRowCount(canEnvVarsCount);
      this->ui.tableWidget_Properties->setColumnCount(headerLabels.size());
      this->ui.tableWidget_Properties->setHorizontalHeaderLabels(headerLabels);

      for (size_t i = 0; i < canEnvVarsCount; i++)
      {
         this->BuildCanEnvVarRow(canNetwork->GetEnvVarByIndex(i), i);
      }

      this->SetComboDelegateForCanEnvVar();
   }
}

void BusConfigUI::BuildCanValueTableProperties(const ICanNetwork* canNetwork, const QString& canValueTableOwnerType, const QString& canValueTableOwnerName)
{
   auto FillValueTable = [this] (const ICanValueTable* canValueTable)
   {
      if (canValueTable == nullptr) { return; }

      QStringList headerLabels;
      headerLabels << "Value" << "Description";

      size_t valuesCount = canValueTable->GetValuesCount();
      this->ui.tableWidget_Properties->setRowCount(valuesCount);
      this->ui.tableWidget_Properties->setColumnCount(2);

      this->ui.tableWidget_Properties->setHorizontalHeaderLabels(headerLabels);

      for (size_t i = 0; i < valuesCount; i++)
      {
         auto value = canValueTable->GetValue(i);
         this->ui.tableWidget_Properties->setItem(i, 0, new QTableWidgetItem { toQString(value) });
         this->ui.tableWidget_Properties->setItem(i, 1, new QTableWidgetItem{ canValueTable->GetValueDescription(value) });
      }
   };
   if (canValueTableOwnerType == ItemId::CAN_SIGNAL.data())
   {
      if (const auto canSignal = canNetwork->GetSignalByName(canValueTableOwnerName.toUtf8()); canSignal)
      {
         FillValueTable(canSignal->GetValueTable());
      }
   }
   else if (canValueTableOwnerType == ItemId::CAN_ENVIRONMENT_VARIABLE.data())
   {
      if (const auto canEnvVar = canNetwork->GetEnvVarByName(canValueTableOwnerName.toUtf8()); canEnvVar)
      {
         FillValueTable(canEnvVar->GetValueTable());
      }
   }
}

void BusConfigUI::BuildAttributesProperties(const ICanAttributeOwner* attributeOwner)
{
   if (attributeOwner)
   {
      size_t attributesCount = attributeOwner->GetAttributesValuesCount();
      QStringList headerLabels;
      headerLabels << "Name" << "Value";

      this->ui.tableWidget_Properties->setRowCount(attributesCount);
      this->ui.tableWidget_Properties->setColumnCount(headerLabels.size());
      this->ui.tableWidget_Properties->setHorizontalHeaderLabels(headerLabels);

      uint16_t row = 0;
      const auto fillAttributeValuesRow = [this, attributeOwner, &row]
         (const ICanAttribute* attribute)
      {
         const auto attributeValue = attributeOwner->GetAttributeValue(attribute->GetName());
         if (attributeValue != nullptr)
         {
            this->ui.tableWidget_Properties->setItem(row, 0, new QTableWidgetItem{ this->icons[Icon_e::ATTRIBUTES], attribute->GetName() });
            const auto fillAttributeValueColumn = [this, &row]
               (const std::string& value)
            {
               this->ui.tableWidget_Properties->setItem(row, 1, new QTableWidgetItem{ value.c_str() });
               ++row;
            };
            ICanAttributeManager::ForAttributeStrValue(attributeValue, fillAttributeValueColumn);
         }
      };
      ICanAttributeManager::ForEachAttribute(attributeOwner, fillAttributeValuesRow);
   }
}

void BusConfigUI::BuildCanNetworkRow(const ICanNetwork* network, int row)
{
   if (network)
   {
      this->ui.tableWidget_Properties->setItem(row, 0, new QTableWidgetItem{ this->icons[Icon_e::NETWORK], network->GetName() });
      this->ui.tableWidget_Properties->setItem(row, 1, new QTableWidgetItem{ network->GetProtocol() });
      this->ui.tableWidget_Properties->setItem(row, 2, new QTableWidgetItem{ network->GetComment() });
   }
}

void BusConfigUI::BuildCanNodeRow(const ICanNode* node, int row)
{
   if (node)
   {
      this->ui.tableWidget_Properties->setItem(row, 0, new QTableWidgetItem{ this->icons[Icon_e::NETWORK_NODE], node->GetName() });

      const auto canNetwork = node->GetNetwork();
      const QString canNetworkName = canNetwork ? canNetwork->GetName() : "";
      this->ui.tableWidget_Properties->setItem(row, 1, new QTableWidgetItem{ canNetwork->GetName() });

      this->ui.tableWidget_Properties->setItem(row, 2, new QTableWidgetItem{ toHexQString(node->GetAddress()) });
      this->ui.tableWidget_Properties->setItem(row, 3, new QTableWidgetItem{ node->GetComment() });
   }
}

void BusConfigUI::BuildCanSignalRow(const ICanSignal* signal, int row)
{
   if (signal)
   {
      this->ui.tableWidget_Properties->setItem(row, 0, new QTableWidgetItem{ this->icons[Icon_e::SIGNAL], signal->GetName() });

      const auto canNetwork = signal->GetNetwork();
      const QString canNetworkName = canNetwork ? canNetwork->GetName() : "";
      this->ui.tableWidget_Properties->setItem(row, 1, new QTableWidgetItem{ canNetwork->GetName() });

      const auto canMessage = signal->GetMessage();
      const QString canMessageName = canMessage ? canMessage->GetName() : "";
      this->ui.tableWidget_Properties->setItem(row, 2, new QTableWidgetItem{ canMessage->GetName() });

      this->ui.tableWidget_Properties->setItem(row, 3, new TableWidgetItem<uint32_t>{ toQString(signal->GetStartBit()) });
      this->ui.tableWidget_Properties->setItem(row, 4, new QTableWidgetItem{ toQString(signal->GetSize()) });

      int pos = static_cast<int>(signal->GetByteOrder());
      const QString byteOrder = pos >= 0 ? CanSignalManager::BYTE_ORDERS[pos] : CanSignalManager::ByteOrder::DEFAULT.data();
      pos = static_cast<int>(signal->GetValueType());
      const QString valueType = pos >= 0 ? CanSignalManager::VALUE_TYPES[pos] : CanSignalManager::ValueType::DEFAULT.data();

      const QString valueTableName = std::invoke([&signal]
      {
         if (const auto valueTable = signal->GetValueTable(); valueTable)
         {
            return valueTable->GetName();
         }
         return "";
      });

      this->ui.tableWidget_Properties->setItem(row, 5, new QTableWidgetItem{ byteOrder });
      this->ui.tableWidget_Properties->setItem(row, 6, new QTableWidgetItem{ valueType });

      this->ui.tableWidget_Properties->setItem(row, 7, new QTableWidgetItem{ toQString(signal->GetInitialValue()) });
      this->ui.tableWidget_Properties->setItem(row, 8, new QTableWidgetItem{ toQString(signal->GetFactor()) });
      this->ui.tableWidget_Properties->setItem(row, 9, new QTableWidgetItem{ toQString(signal->GetOffset()) });
      this->ui.tableWidget_Properties->setItem(row, 10, new QTableWidgetItem{ toQString(signal->GetMinimum()) });
      this->ui.tableWidget_Properties->setItem(row, 11, new QTableWidgetItem{ toQString(signal->GetMaximum()) });

      this->ui.tableWidget_Properties->setItem(row, 12, new QTableWidgetItem{ signal->GetUnit() });
      this->ui.tableWidget_Properties->setItem(row, 13, new QTableWidgetItem{ valueTableName });
      this->ui.tableWidget_Properties->setItem(row, 14, new QTableWidgetItem{ signal->GetComment() });
   }
}

void BusConfigUI::BuildCanMessageRow(const ICanMessage* canMessage, int row)
{
   if (canMessage)
   {
      this->ui.tableWidget_Properties->setItem(row, 0, new QTableWidgetItem{ this->icons[Icon_e::MESSAGE], canMessage->GetName() });
      
      const auto canNetwork = canMessage->GetNetwork();
      const QString canNetworkName = canNetwork ? canNetwork->GetName() : "";
      this->ui.tableWidget_Properties->setItem(row, 1, new QTableWidgetItem{ canNetworkName });
      
      this->ui.tableWidget_Properties->setItem(row, 2, new QTableWidgetItem{ toQString(canMessage->GetId()) });

      int pos = static_cast<int>(canMessage->GetIdFormat());
      const QString idFormat = pos >= 0 ? CanMessageManager::ID_FORMATS[pos] : CanMessageManager::IdFormat::DEFAULT.data();
      pos = static_cast<int>(canMessage->GetTxMethod());
      const QString txMethod = pos >= 0 ? CanMessageManager::TX_METHODS[pos] : ICanMessage::TxMethod::DEFAULT;

      this->ui.tableWidget_Properties->setItem(row, 3, new QTableWidgetItem{ idFormat });
      this->ui.tableWidget_Properties->setItem(row, 4, new QTableWidgetItem{ toQString(canMessage->GetSize()) });
      this->ui.tableWidget_Properties->setItem(row, 5, new QTableWidgetItem{ txMethod });
      this->ui.tableWidget_Properties->setItem(row, 6, new QTableWidgetItem{ toQString(canMessage->GetCycleTime()) });
      this->ui.tableWidget_Properties->setItem(row, 7, new QTableWidgetItem{ canMessage->GetComment() });
   }
}

void BusConfigUI::BuildCanEnvVarRow(const ICanEnvVar* envVar, int row)
{
   if (envVar)
   {
      std::variant<const ICanEnvVar*, const ICanIntEnvVar*, const ICanFloatEnvVar*, const ICanStringEnvVar*, const ICanDataEnvVar*>
         vCanEnvVar = envVar;

      switch (envVar->GetType())
      {
         case ICanEnvVar::Type_e::INTEGER:
         {
            vCanEnvVar = dynamic_cast<const ICanIntEnvVar*>(envVar);
            break;
         }
         case ICanEnvVar::Type_e::FLOAT:
         {
            vCanEnvVar = dynamic_cast<const ICanFloatEnvVar*>(envVar);
            break;
         }
         case ICanEnvVar::Type_e::STRING:
         {
            vCanEnvVar = dynamic_cast<const ICanStringEnvVar*>(envVar);
            break;
         }
         case ICanEnvVar::Type_e::DATA:
         {
            vCanEnvVar = dynamic_cast<const ICanDataEnvVar*>(envVar);
            break;
         }
         default:
         {
            break;
         }
      }

      this->ui.tableWidget_Properties->setItem(row, 0, new QTableWidgetItem{ this->icons[Icon_e::ENVIRONMENT_VARIABLE], envVar->GetName() });

      const auto canNetwork = envVar->GetNetwork();
      const QString canNetworkName = (canNetwork ? canNetwork->GetName() : "");
      this->ui.tableWidget_Properties->setItem(row, 1, new QTableWidgetItem{ canNetworkName });

      int pos = static_cast<int>(envVar->GetType());
      const QString type = pos >= 0 ? CanEnvVarManager::TYPES[pos] : CanEnvVarManager::Type::DEFAULT.data();
      pos = static_cast<int>(envVar->GetAccessType());
      const QString accessType = pos >= 0 ? CanEnvVarManager::ACCESS_TYPES[pos] : CanEnvVarManager::AccessType::DEFAULT.data();

      const QString valueTableName = std::invoke([&envVar]
      {
         if (const auto valueTable = envVar->GetValueTable(); valueTable)
         {
            return valueTable->GetName();
         }
         return "";
      });

      this->ui.tableWidget_Properties->setItem(row, 2, new QTableWidgetItem{ type });
      this->ui.tableWidget_Properties->setItem(row, 3, new QTableWidgetItem{ envVar->GetUnit() });

      std::visit([this, &row](auto&& arg)
      {
         using T = std::decay_t<decltype(arg)>;
         if constexpr (std::is_same_v<T, const ICanIntEnvVar*> || std::is_same_v<T, const ICanFloatEnvVar*>)
         {
            this->ui.tableWidget_Properties->setItem(row, 4, new QTableWidgetItem{ toQString(arg->GetMinimum()) });
            this->ui.tableWidget_Properties->setItem(row, 5, new QTableWidgetItem{ toQString(arg->GetMaximum()) });
            this->ui.tableWidget_Properties->setItem(row, 6, new QTableWidgetItem{ toQString(arg->GetInitialValue()) });
            this->ui.tableWidget_Properties->setItem(row, 7, new QTableWidgetItem{ "-" });
         }
         else if constexpr (std::is_same_v<T, const ICanDataEnvVar*>)
         {
            this->ui.tableWidget_Properties->setItem(row, 4, new QTableWidgetItem{ "-" });
            this->ui.tableWidget_Properties->setItem(row, 5, new QTableWidgetItem{ "-" });
            this->ui.tableWidget_Properties->setItem(row, 6, new QTableWidgetItem{ "-" });
            this->ui.tableWidget_Properties->setItem(row, 7, new QTableWidgetItem{ toQString(arg->GetLength()) });
         }
         else
         {
            this->ui.tableWidget_Properties->setItem(row, 4, new QTableWidgetItem{ "-" });
            this->ui.tableWidget_Properties->setItem(row, 5, new QTableWidgetItem{ "-" });
            this->ui.tableWidget_Properties->setItem(row, 6, new QTableWidgetItem{ "-" });
            this->ui.tableWidget_Properties->setItem(row, 7, new QTableWidgetItem{ "-" });
         }
      }, vCanEnvVar);

      this->ui.tableWidget_Properties->setItem(row, 8, new QTableWidgetItem{ accessType });
      this->ui.tableWidget_Properties->setItem(row, 9, new QTableWidgetItem{ valueTableName });
      this->ui.tableWidget_Properties->setItem(row, 10, new QTableWidgetItem{ envVar->GetComment() });
   }
}

auto BusConfigUI::Find(const QString& itemName)
{
   return this->ui.treeWidget_MainView->findItems(itemName, Qt::MatchExactly | Qt::MatchRecursive, 0);
}

void BusConfigUI::RemoveFromTreeWidget(const QString& itemName)
{
   auto treeItems = this->Find(itemName);
   for (auto& treeItem : treeItems)
   {
      auto parent = treeItem->parent();
      parent->removeChild(treeItem);
   }
}

void BusConfigUI::RemoveCanMessageFromTreeWidget(ICanNetwork* canNetwork, const QString& messageName)
{
   if (const auto canMessage = canNetwork->GetMessageByName(messageName.toUtf8()); canMessage)
   {
      for (size_t i = 0; i < canMessage->GetSignalsCount(); i++)
      {
         if (const auto canSignal = canMessage->GetSignalByIndex(i); canSignal)
         {
            this->RemoveFromTreeWidget(canSignal->GetName());
         }
      }
      this->RemoveFromTreeWidget(messageName);
   }
}

void BusConfigUI::ChangeTreeWidgetItemName(const QString& itemName, const QString& newItemName)
{
   auto treeItems = this->Find(itemName);
   for (auto& treeItem : treeItems)
   {
      treeItem->setText(0, newItemName);
   }
}

void BusConfigUI::SetComboDelegateForCanSignal(void)
{
   ComboDelegate* byteOrderDelegate = new ComboDelegate{ CanSignalManager::BYTE_ORDERS };
   this->ui.tableWidget_Properties->setItemDelegateForColumn(5, byteOrderDelegate);
   ComboDelegate* valueTypeDelegate = new ComboDelegate{ CanSignalManager::VALUE_TYPES };
   this->ui.tableWidget_Properties->setItemDelegateForColumn(6, valueTypeDelegate);
}

void BusConfigUI::SetComboDelegateForCanMessage(void)
{
   ComboDelegate* idFormatsDelegate = new ComboDelegate{ CanMessageManager::ID_FORMATS, 25, 150 };
   this->ui.tableWidget_Properties->setItemDelegateForColumn(3, idFormatsDelegate);
   ComboDelegate* txMethodsDelegate = new ComboDelegate{ CanMessageManager::TX_METHODS, 25, 150 };
   this->ui.tableWidget_Properties->setItemDelegateForColumn(5, txMethodsDelegate);
}

void BusConfigUI::SetComboDelegateForCanEnvVar(void)
{
   ComboDelegate* typeDelegate = new ComboDelegate{ CanEnvVarManager::TYPES };
   this->ui.tableWidget_Properties->setItemDelegateForColumn(2, typeDelegate);
   ComboDelegate* accessTypeDelegate = new ComboDelegate{ CanEnvVarManager::ACCESS_TYPES };
   this->ui.tableWidget_Properties->setItemDelegateForColumn(8, accessTypeDelegate);
}

QTreeWidgetItem* BusConfigUI::GetTreeItem(const QString& ancestorItemWhatsThis, QTreeWidgetItem* descendantItem)
{
   if (descendantItem)
   {
      if (descendantItem->whatsThis(0) == ancestorItemWhatsThis)
      {
         return descendantItem;
      }
      else
      {
         return GetTreeItem(ancestorItemWhatsThis, descendantItem->parent());
      }
   }
   return nullptr;
};

void BusConfigUI::TableWidgetRemoveMenuEntryConfig(QMenu* menu, const QString& itemType, ICanNetwork* canNetwork, const QString& name)
{
   if (menu && canNetwork)
   {
      auto removeMenuEntry = new QAction{ "Remove", menu };
      menu->addAction(removeMenuEntry);

      connect(removeMenuEntry, &QAction::triggered, this, [this, itemType, name, canNetwork]
      {
         if (itemType == ItemId::CAN_MESSAGE.data())
         {
            this->RemoveCanMessageFromTreeWidget(canNetwork, name);
            this->RemoveCanMessage(canNetwork, name);
         }
         else if (itemType == ItemId::CAN_MESSAGES.data())
         {
            this->RemoveCanMessageFromTreeWidget(canNetwork, name);
            this->RemoveCanMessage(canNetwork, name);
         }
         else if (itemType == ItemId::CAN_TX_MESSAGES.data())
         {
            this->RemoveCanMessageFromTreeWidget(canNetwork, name);
            this->RemoveCanMessage(canNetwork, name);
         }
         else if (itemType == ItemId::CAN_RX_MESSAGES.data())
         {
            this->RemoveCanMessageFromTreeWidget(canNetwork, name);
            this->RemoveCanMessage(canNetwork, name);
         }
         else if (itemType == ItemId::CAN_SIGNAL.data())
         {
            this->RemoveFromTreeWidget(name);
            this->RemoveCanSignal(canNetwork, name);
         }
         else if (itemType == ItemId::CAN_SIGNALS.data())
         {
            this->RemoveFromTreeWidget(name);
            this->RemoveCanSignal(canNetwork, name);
         }
         else if (itemType == ItemId::CAN_MESSAGE_SIGNALS.data())
         {
            this->RemoveFromTreeWidget(name);
            this->RemoveCanSignal(canNetwork, name);
         }
         else if (itemType == ItemId::CAN_MAPPED_TX_SIGNALS.data())
         {
            this->RemoveFromTreeWidget(name);
            this->RemoveCanSignal(canNetwork, name);
         }
         else if (itemType == ItemId::CAN_MAPPED_TX_MESSAGE_SIGNALS.data())
         {
            this->RemoveFromTreeWidget(name);
            this->RemoveCanSignal(canNetwork, name);
         }
         else if (itemType == ItemId::CAN_MAPPED_RX_SIGNALS.data())
         {
            this->RemoveFromTreeWidget(name);
            this->RemoveCanSignal(canNetwork, name);
         }
         else if (itemType == ItemId::CAN_MAPPED_RX_MESSAGE_SIGNALS.data())
         {
            this->RemoveFromTreeWidget(name);
            this->RemoveCanSignal(canNetwork, name);
         }
         else if (itemType == ItemId::CAN_ENVIRONMENT_VARIABLE.data())
         {
            this->RemoveFromTreeWidget(name);
            this->RemoveCanEnvVar(canNetwork, name);
         }
         else if (itemType == ItemId::CAN_ENVIRONMENT_VARIABLES.data())
         {
            this->RemoveFromTreeWidget(name);
            this->RemoveCanEnvVar(canNetwork, name);
         }

         this->RemoveFromTableWidget(name);
         //this->ui.tableWidget_Properties->removeRow(row);
      });
   }
}

void BusConfigUI::TableWidgetCaseSensitiveMenuEntryConfig(QMenu* menu)
{
   if (menu)
   {
      auto caseSensitiveMenuEntry = new QAction{ "Case sensitive", menu };
      caseSensitiveMenuEntry->setCheckable(true);
      caseSensitiveMenuEntry->setChecked(this->caseSensitive);
      menu->addAction(caseSensitiveMenuEntry);
      connect(caseSensitiveMenuEntry, &QAction::triggered, this, [caseSensitiveMenuEntry, this]()
      {
         this->caseSensitive = caseSensitiveMenuEntry->isChecked();
      });
   }
}

void BusConfigUI::TableWidgetSortSignalsByNameMenuEntryConfig(QMenu* menu, ICanNetwork* canNetwork)
{
   if (menu && canNetwork)
   {
      auto sortByNameMenuEntry = new QAction{ "Sort signals by name", menu };
      menu->addAction(sortByNameMenuEntry);

      connect(sortByNameMenuEntry, &QAction::triggered, this, [this, canNetwork]
      {
         canNetwork->SortSignalsByName(this->caseSensitive);
         this->BuildTable();
      });
   }
}

void BusConfigUI::TableWidgetSortSignalsByNameMenuEntryConfig(QMenu* menu, ICanMessage* canMessage)
{
   if (menu && canMessage)
   {
      auto sortByNameMenuEntry = new QAction{ "Sort signals by name", menu };
      menu->addAction(sortByNameMenuEntry);

      connect(sortByNameMenuEntry, &QAction::triggered, this, [this, canMessage]
      {
         canMessage->SortSignalsByName(this->caseSensitive);
         this->BuildTable();
      });
   }
}

void BusConfigUI::TableWidgetSortSignalsByStartBitMenuEntryConfig(QMenu* menu, ICanMessage* canMessage)
{
   if (menu && canMessage)
   {
      auto sortByStartBitMenuEntry = new QAction{ "Sort signals by start bit", menu };
      menu->addAction(sortByStartBitMenuEntry);

      connect(sortByStartBitMenuEntry, &QAction::triggered, this, [this, canMessage]
      {
         canMessage->SortSignalsByStartBit();
         this->BuildTable();
      });
   }
}

void BusConfigUI::TableWidgetSortSignalsByMessageMenuEntryConfig(QMenu* menu, ICanNetwork* canNetwork)
{
   if (menu && canNetwork)
   {
      auto sortByMessageMenuEntry = new QAction{ "Sort signals by message", menu };
      menu->addAction(sortByMessageMenuEntry);

      connect(sortByMessageMenuEntry, &QAction::triggered, this, [this, canNetwork]
      {
         canNetwork->SortSignalsByMessageName(this->caseSensitive);
         this->BuildTable();
      });
   }
}

void BusConfigUI::TableWidgetNewSignalMenuEntryConfig(QMenu* menu, ICanNetwork* canNetwork)
{
   if (menu && canNetwork)
   {
      auto newSignalMenuEntry = new QAction{ "New signal", menu };
      menu->addAction(newSignalMenuEntry);

      connect(newSignalMenuEntry, &QAction::triggered, this, [this, canNetwork]
      {
         this->canSignalCreator->Create(canNetwork);
         this->canSignalCreator->setModal(true);
         this->canSignalCreator->show();
      });
   }
}

void BusConfigUI::TableWidgetSortMessagesByNameMenuEntryConfig(QMenu* menu, ICanNetwork* canNetwork)
{
   if (menu && canNetwork)
   {
      auto sortByNameMenuEntry = new QAction{ "Sort messages by name", menu };
      menu->addAction(sortByNameMenuEntry);

      connect(sortByNameMenuEntry, &QAction::triggered, this, [this, canNetwork]
      {
         canNetwork->SortMessagesByName(this->caseSensitive);
         this->BuildTable();
      });
   }
}

void BusConfigUI::TableWidgetSortMessagesByIdMenuEntryConfig(QMenu* menu, ICanNetwork* canNetwork)
{
   if (menu && canNetwork)
   {
      auto sortByIdMenuEntry = new QAction{ "Sort messages by id", menu };
      menu->addAction(sortByIdMenuEntry);

      connect(sortByIdMenuEntry, &QAction::triggered, this, [this, canNetwork]
      {
         canNetwork->SortMessagesById();
         this->BuildTable();
      });
   }
}

void BusConfigUI::TableWidgetNewMessageMenuEntryConfig(QMenu* menu, ICanNetwork* canNetwork)
{
   if (menu && canNetwork)
   {
      auto newMessageMenuEntry = new QAction{ "New message", menu };
      menu->addAction(newMessageMenuEntry);

      connect(newMessageMenuEntry, &QAction::triggered, this, [this, canNetwork]
      {
         this->canMessageCreator->Create(canNetwork);
         this->canMessageCreator->setModal(true);
         this->canMessageCreator->show();
      });
   }
}

void BusConfigUI::TableWidgetSortNodesByNameMenuEntryConfig(QMenu* menu, ICanNetwork* canNetwork)
{
   if (menu && canNetwork)
   {
      auto sortByNameMenuEntry = new QAction{ "Sort nodes by name", menu };
      menu->addAction(sortByNameMenuEntry);

      connect(sortByNameMenuEntry, &QAction::triggered, this, [this, canNetwork]
      {
         canNetwork->SortNodesByName(this->caseSensitive);
         this->BuildTable();
      });
   }
}

void BusConfigUI::TableWidgetNewNodeMenuEntryConfig(QMenu* menu, ICanNetwork* canNetwork)
{
   if (menu && canNetwork)
   {
      auto newNodeMenuEntry = new QAction{ "New node", menu };
      menu->addAction(newNodeMenuEntry);

      connect(newNodeMenuEntry, &QAction::triggered, this, [this, canNetwork]
      {
         this->canNodeCreator->Create(canNetwork);
         this->canNodeCreator->setModal(true);
         this->canNodeCreator->show();
      });
   }
}

void BusConfigUI::ShowMenuForCanSignalsTableItem(QMenu* menu, ICanNetwork* canNetwork, const QString& name)
{
   if (canNetwork)
   {
      this->TableWidgetRemoveMenuEntryConfig(menu, ItemId::CAN_SIGNALS.data(), canNetwork, name);

      menu->addSeparator();

      this->TableWidgetCaseSensitiveMenuEntryConfig(menu);
      this->TableWidgetSortSignalsByNameMenuEntryConfig(menu, canNetwork);
      this->TableWidgetSortSignalsByMessageMenuEntryConfig(menu, canNetwork);

      menu->addSeparator();

      this->TableWidgetNewSignalMenuEntryConfig(menu, canNetwork);
   }
}

void BusConfigUI::ShowMenuForCanMessageSignalsTableItem(QMenu* menu, ICanNetwork* canNetwork, const QString& name)
{
   if (canNetwork)
   {
      if (const auto canMessage = canNetwork->GetMessageByName(name.toUtf8()); canMessage)
      {
         this->TableWidgetRemoveMenuEntryConfig(menu, ItemId::CAN_SIGNALS.data(), canNetwork, name);

         menu->addSeparator();

         this->TableWidgetCaseSensitiveMenuEntryConfig(menu);
         this->TableWidgetSortSignalsByNameMenuEntryConfig(menu, canMessage);
         this->TableWidgetSortSignalsByStartBitMenuEntryConfig(menu, canMessage);
      }
   }
}

void BusConfigUI::ShowMenuForCanMessagesTableItem(QMenu* menu, ICanNetwork* canNetwork, const QString& name)
{
   if (canNetwork)
   {
      this->TableWidgetRemoveMenuEntryConfig(menu, ItemId::CAN_MESSAGES.data(), canNetwork, name);

      menu->addSeparator();

      this->TableWidgetCaseSensitiveMenuEntryConfig(menu);
      this->TableWidgetSortMessagesByNameMenuEntryConfig(menu, canNetwork);
      this->TableWidgetSortMessagesByIdMenuEntryConfig(menu, canNetwork);

      menu->addSeparator();

      this->TableWidgetNewMessageMenuEntryConfig(menu, canNetwork);
   }
}

void BusConfigUI::ShowMenuForCanNodesTableItem(QMenu* menu, ICanNetwork* canNetwork, const QString& name)
{
   if (canNetwork)
   {
      this->TableWidgetRemoveMenuEntryConfig(menu, ItemId::CAN_NETWORK_NODES.data(), canNetwork, name);

      menu->addSeparator();

      this->TableWidgetCaseSensitiveMenuEntryConfig(menu);
      this->TableWidgetSortNodesByNameMenuEntryConfig(menu, canNetwork);

      menu->addSeparator();

      this->TableWidgetNewNodeMenuEntryConfig(menu, canNetwork);
   }
}

void BusConfigUI::ShowMenuForCanSignalsTreeItem(QMenu* menu, ICanNetwork* canNetwork)
{
   if (canNetwork)
   {
      this->TableWidgetNewSignalMenuEntryConfig(menu, canNetwork);
   }
}

void BusConfigUI::ShowMenuForCanSignalTreeItem(QMenu* menu, ICanNetwork* canNetwork, const QString& name)
{
   if (canNetwork)
   {
      this->TableWidgetNewSignalMenuEntryConfig(menu, canNetwork);

      menu->addSeparator();

      this->TableWidgetRemoveMenuEntryConfig(menu, ItemId::CAN_SIGNAL.data(), canNetwork, name);
   }
}

void BusConfigUI::ShowMenuForCanMessageSignalsTreeItem(QMenu* menu, ICanNetwork* canNetwork)
{
   if (canNetwork)
   {
      this->TableWidgetNewSignalMenuEntryConfig(menu, canNetwork);
   }
}

void BusConfigUI::ShowMenuForCanMessagesTreeItem(QMenu* menu, ICanNetwork* canNetwork)
{
   if (canNetwork)
   {
      this->TableWidgetNewMessageMenuEntryConfig(menu, canNetwork);
   }
}

void BusConfigUI::ShowMenuForCanMessageTreeItem(QMenu* menu, ICanNetwork* canNetwork, const QString& name)
{
   if (canNetwork)
   {
      this->TableWidgetNewMessageMenuEntryConfig(menu, canNetwork);

      menu->addSeparator();

      this->TableWidgetRemoveMenuEntryConfig(menu, ItemId::CAN_MESSAGE.data(), canNetwork, name);
   }
}

void BusConfigUI::ShowMenuForCanNodesTreeItem(QMenu* menu, ICanNetwork* canNetwork)
{
   if (canNetwork)
   {
      this->TableWidgetNewNodeMenuEntryConfig(menu, canNetwork);
   }
}

void BusConfigUI::ShowMenuForCanNodeTreeItem(QMenu* menu, ICanNetwork* canNetwork, const QString& name)
{
   if (canNetwork)
   {
      this->TableWidgetNewNodeMenuEntryConfig(menu, canNetwork);

      menu->addSeparator();

      this->TableWidgetRemoveMenuEntryConfig(menu, ItemId::CAN_NETWORK_NODE.data(), canNetwork, name);
   }
}

void BusConfigUI::RemoveFromTableWidget(const QString& name)
{
   for (size_t i = 0; i < this->ui.tableWidget_Properties->rowCount(); i++)
   {
      for (size_t j = 0; j < this->ui.tableWidget_Properties->columnCount(); j++)
      {
         const auto text = this->ui.tableWidget_Properties->item(i, j)->text();
         if (text == name)
         {
            this->ui.tableWidget_Properties->removeRow(i);
            --i;
            break;
         }
      }
   }
}