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
#include <algorithm>
#include <qlocale.h>
#include "LineEditFactory.h"
#include "Conversions.h"
#include "ComboDelegate.h"

namespace ranges = std::ranges;

DllLoader<ICanBusConfig> dllLoader{ "BusConfigDll", "CanBusConfigInstanceCreate", "CanBusConfigInstanceDelete" };

bool LoadBusConfigDll(void)
{
   // local variables
   bool rV = false;
   ICanBusConfig* busConfig = nullptr;

   if (dllLoader.Load())
   {
      busConfig = dllLoader.pfCreate();

      if (busConfig != nullptr)
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
       }

       // add menu for right click for table widget
       this->ui.tableWidget_Properties->setContextMenuPolicy(Qt::CustomContextMenu);
       connect(this->ui.tableWidget_Properties, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowMenuForTableWidgetItem(const QPoint&)));

       this->ui.tableWidget_Properties->setStyleSheet("QTableWidget::item { padding: 0 10px; border: 0; }");
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
   delete this->communicationMatrix;
   delete this->attributeDefinitions;
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
   this->attributeDefinitions->Create(this->canBusConfig);
   this->attributeDefinitions->show();
}

void BusConfigUI::on_actionCommunication_matrix_triggered()
{
   this->communicationMatrix->Create(this->canBusConfig);
   this->communicationMatrix->show();
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
   size_t itemIndex = (size_t)-1;
   this->isTableWidgetFilled = false;

   this->ui.tableWidget_Properties->clear();
   this->ui.tableWidget_Properties->setRowCount(0);
   this->ui.tableWidget_Properties->setColumnCount(0);
   this->ui.tableWidget_Properties->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);

   if (current)
   {
      const auto text = current->text(0);
      const auto itemType = current->whatsThis(0);
      const auto parent = current->parent();
      const auto parentText = (parent ? parent->text(0) : "");
      const auto parentItemType = (parent ? parent->whatsThis(0) : "");

      this->ui.tableWidget_Properties->setWhatsThis(itemType);

      if (itemType == ItemId::CAN_MESSAGE.data())
      {
         this->ui.tableWidget_Properties->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Interactive);
         this->BuildCanMessageProperties(text);
      }
      else if (itemType == ItemId::CAN_MESSAGES.data())
      {
         this->BuildCanMessagesProperties();
      }
      else if (itemType == ItemId::CAN_SIGNAL.data())
      {
         this->ui.tableWidget_Properties->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Interactive);
         this->BuildCanSignalProperties(text);
      }
      else if (itemType == ItemId::CAN_SIGNALS.data())
      {
         this->BuildCanSignalsProperties();
      }
      else if (itemType == ItemId::CAN_ENVIRONMENT_VARIABLE.data())
      {
         this->BuildCanEnvironmentVariableProperties(text);
      }
      else if (itemType == ItemId::CAN_ENVIRONMENT_VARIABLES.data())
      {
         this->BuildCanEnvironmentVariablesProperties();
      }
      else if (itemType == ItemId::VALUE_TABLE.data())
      {
         this->BuildCanValueTableProperties(parentItemType, parentText);
      }
      else if (itemType == ItemId::ATTRIBUTES.data() && parentItemType == ItemId::CAN_SIGNAL.data())
      {
         const auto canSignal = this->canBusConfig->GetSignalByName(parentText.toUtf8());
         this->BuildAttributesProperties(canSignal);
      }
      else if (itemType == ItemId::ATTRIBUTES.data() && parentItemType == ItemId::CAN_MESSAGE.data())
      {
         const auto canMessage = this->canBusConfig->GetMessageByName(parentText.toUtf8());
         this->BuildAttributesProperties(canMessage);
      }
      else if (itemType == ItemId::ATTRIBUTES.data() && parentItemType == ItemId::CAN_NETWORK_NODE.data())
      {
         const auto canNetworkNode = this->canBusConfig->GetNodeByName(parentText.toUtf8());
         this->BuildAttributesProperties(canNetworkNode);
      }
      else if (itemType == ItemId::ATTRIBUTES.data() && parentItemType == ItemId::NETWORK.data())
      {
         this->BuildAttributesProperties(this->canBusConfig);
      }
   }

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

      if (itemType == ItemId::CAN_MESSAGE.data())
      {
         if (CanMessageManager::Validate(this->canBusConfig, row, data, column))
         {
            if (column == 0)
            {
               const QString name = CanMessageManager::GetData(this->canBusConfig, row, column);
               this->ChangeTreeWidgetItemName(name, data);
            }
            CanMessageManager::Modify(this->canBusConfig, row, data, column);
         }
         else
         {
            const QString data = CanMessageManager::GetData(this->canBusConfig, row, column);
            this->ui.tableWidget_Properties->item(row, column)->setText(data);
         }
      }
      else if (itemType == ItemId::CAN_MESSAGES.data())
      {
         if (CanMessageManager::Validate(this->canBusConfig, row, data, column))
         {
            if (column == 0)
            {
               const QString name = CanMessageManager::GetData(this->canBusConfig, row, column);
               this->ChangeTreeWidgetItemName(name, data);
            }
            CanMessageManager::Modify(this->canBusConfig, row, data, column);
         }
         else
         {
            const QString data = CanMessageManager::GetData(this->canBusConfig, row, column);
            this->ui.tableWidget_Properties->item(row, column)->setText(data);
         }
      }
      else if (itemType == ItemId::CAN_SIGNAL.data())
      {
         if (CanSignalManager::Validate(this->canBusConfig, row, data, column))
         {
            if (column == 0)
            {
               const QString name = CanSignalManager::GetData(this->canBusConfig, row, column);
               this->ChangeTreeWidgetItemName(name, data);
            }
            CanSignalManager::Modify(this->canBusConfig, row, data, column);
         }
         else
         {
            const QString data = CanSignalManager::GetData(this->canBusConfig, row, column);
            this->ui.tableWidget_Properties->item(row, column)->setText(data);
         }
      }
      else if (itemType == ItemId::CAN_SIGNALS.data())
      {
         if (CanSignalManager::Validate(this->canBusConfig, row, data, column))
         {
            if (column == 0)
            {
               const QString name = CanSignalManager::GetData(this->canBusConfig, row, column);
               this->ChangeTreeWidgetItemName(name, data);
            }
            CanSignalManager::Modify(this->canBusConfig, row, data, column);
         }
         else
         {
            const QString data = CanSignalManager::GetData(this->canBusConfig, row, column);
            this->ui.tableWidget_Properties->item(row, column)->setText(data);
         }
      }
      else if (itemType == ItemId::CAN_ENVIRONMENT_VARIABLE.data())
      {
         //this->BuildCanEnvironmentVariableProperties(text);
      }
      else if (itemType == ItemId::CAN_ENVIRONMENT_VARIABLES.data())
      {
         //this->BuildCanEnvironmentVariablesProperties();
      }
   }

}

void BusConfigUI::on_tableWidget_Properties_cellChanged(int row, int column)
{
   if (this->isTableWidgetFilled)
   {
      const auto data = this->ui.tableWidget_Properties->item(row, column)->text();
      const auto itemType = this->ui.tableWidget_Properties->whatsThis();

      if (itemType == "CanMessage")
      {
         CanMessageManager::Modify(this->canBusConfig, row, data, column);
      }
      else if (itemType == "CanMessages")
      {
         CanMessageManager::Modify(this->canBusConfig, row, data, column);
      }
      else if (itemType == "CanSignal")
      {
         CanSignalManager::Modify(this->canBusConfig, row, data, column);
      }
      else if (itemType == "CanSignals")
      {
         CanSignalManager::Modify(this->canBusConfig, row, data, column);
      }
      else if (itemType == "CanEnvironmentVariable")
      {
         //this->BuildCanEnvironmentVariableProperties(text);
      }
      else if (itemType == "CanEnvironmentVariables")
      {
         //this->BuildCanEnvironmentVariablesProperties();
      }
   }
}

// menu for right click for table widget
void BusConfigUI::ShowMenuForTableWidgetItem(const QPoint& pos)
{
   QPoint globalPos = this->ui.tableWidget_Properties->mapToGlobal(pos);
   QTableWidgetItem* item = this->ui.tableWidget_Properties->itemAt(pos);

   //create right click menu item
   QMenu* itemMenu = new QMenu { this->ui.tableWidget_Properties };
   QAction* itemMenuEntry{ nullptr };
   itemMenuEntry = new QAction { "Remove", itemMenu };
   itemMenu->addAction(itemMenuEntry);

   const auto itemType = this->ui.tableWidget_Properties->whatsThis();
   const int row = item->row();

   connect(itemMenuEntry, &QAction::triggered, this, [this, &itemType, row]
   {
      QString name = this->ui.tableWidget_Properties->item(row, 0)->text();
      if (itemType == "CanMessage")
      {
         this->RemoveCanMessage(name);
      }
      else if (itemType == "CanMessages")
      {
         this->RemoveCanMessage(row);
      }
      else if (itemType == "CanSignal")
      {
         this->RemoveCanSignal(name);
      }
      else if (itemType == "CanSignals")
      {
         this->RemoveCanSignal(row);
      }
      else if (itemType == "CanEnvironmentVariable")
      {
         this->RemoveCanEnvVar(name);
      }
      else if (itemType == "CanEnvironmentVariables")
      {
         this->RemoveCanEnvVar(row);
      }

      this->ui.tableWidget_Properties->removeRow(row);
      this->RemoveFromTreeWidget(name);
   });

   QAction* input = itemMenu->exec(globalPos);
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
   auto networkTreeItem = new QTreeWidgetItem{ this->ui.treeWidget_MainView };
   this->ui.treeWidget_MainView->addTopLevelItem(networkTreeItem);
   networkTreeItem->setText(0, ItemId::NETWORK.data());
   networkTreeItem->setIcon(0, this->icons[Icon_e::NETWORK]);
   networkTreeItem->setWhatsThis(0, ItemId::NETWORK.data());
   networkTreeItem->setToolTip(0, "Network");

   auto canNetworkNodesTreeItem = new QTreeWidgetItem{ networkTreeItem };
   canNetworkNodesTreeItem->setText(0, "Network nodes");
   canNetworkNodesTreeItem->setIcon(0, this->icons[Icon_e::NETWORK_NODE]);
   canNetworkNodesTreeItem->setWhatsThis(0, ItemId::CAN_NETWORK_NODES.data());
   canNetworkNodesTreeItem->setToolTip(0, "Can network nodes");
   size_t canNodesCount = this->canBusConfig->GetNodesCount();
   for (size_t i = 0; i < canNodesCount; i++)
   {
      if (const auto canNode = this->canBusConfig->GetNodeByIndex(i); canNode != nullptr)
      {
         auto canNodeTreeItem = new QTreeWidgetItem{ canNetworkNodesTreeItem };
         canNodeTreeItem->setText(0, canNode->GetName());
         canNodeTreeItem->setWhatsThis(0, ItemId::CAN_NETWORK_NODE.data());
         canNodeTreeItem->setToolTip(0, "Can network node");
         canNodeTreeItem->setIcon(0, this->icons[Icon_e::NETWORK_NODE]);

         // Tx Messages
         auto txMessagesTreeItem = new QTreeWidgetItem{ canNodeTreeItem };
         txMessagesTreeItem->setText(0, "Tx Messages");
         txMessagesTreeItem->setIcon(0, this->icons[Icon_e::MESSAGE]);
         size_t txMessagesCount = canNode->GetTxMessagesCount();
         for (size_t j = 0; j < txMessagesCount; j++)
         {
            if (const auto txMessage = canNode->GetTxMessageByIndex(j); txMessage != nullptr)
            {
               auto txMessageTreeItem = new QTreeWidgetItem{ txMessagesTreeItem };
               txMessageTreeItem->setText(0, txMessage->GetName());
               txMessageTreeItem->setIcon(0, this->icons[Icon_e::MESSAGE]);
            }
         }

         // Rx Messages
         auto rxMessagesTreeItem = new QTreeWidgetItem{ canNodeTreeItem };
         rxMessagesTreeItem->setText(0, "Rx Messages");
         rxMessagesTreeItem->setIcon(0, this->icons[Icon_e::MESSAGE]);
         size_t rxMessagesCount = canNode->GetRxMessagesCount();
         for (size_t j = 0; j < rxMessagesCount; j++)
         {
            if (const auto rxMessage = canNode->GetRxMessageByIndex(j); rxMessage != nullptr)
            {
               auto rxMessageTreeItem = new QTreeWidgetItem{ rxMessagesTreeItem };
               rxMessageTreeItem->setText(0, rxMessage->GetName());
               rxMessageTreeItem->setIcon(0, this->icons[Icon_e::MESSAGE]);
            }
         }

         // Mapped Tx Signals
         auto txSignalsTreeItem = new QTreeWidgetItem{ canNodeTreeItem };
         txSignalsTreeItem->setText(0, "Mapped Tx Signals");
         txSignalsTreeItem->setIcon(0, this->icons[Icon_e::SIGNAL]);
         size_t txSignalsCount = canNode->GetMappedTxSignalsCount();
         for (size_t j = 0; j < txSignalsCount; j++)
         {
            if (const auto txSignal = canNode->GetMappedTxSignalByIndex(j); txSignal != nullptr)
            {
               auto txSignalTreeItem = new QTreeWidgetItem{ txSignalsTreeItem };
               txSignalTreeItem->setText(0, txSignal->GetName());
               txSignalTreeItem->setIcon(0, this->icons[Icon_e::SIGNAL]);
            }
         }

         // Mapped Rx Signals
         auto rxSignalsTreeItem = new QTreeWidgetItem { canNodeTreeItem };
         rxSignalsTreeItem->setText(0, "Mapped Rx Signals");
         rxSignalsTreeItem->setIcon(0, this->icons[Icon_e::SIGNAL]);
         size_t rxSignalsCount = canNode->GetMappedRxSignalsCount();
         for (size_t j = 0; j < rxSignalsCount; j++)
         {
            if (const auto rxSignal = canNode->GetMappedRxSignalByIndex(j); rxSignal != nullptr)
            {
               auto rxSignalTreeItem = new QTreeWidgetItem{ rxSignalsTreeItem };
               rxSignalTreeItem->setText(0, rxSignal->GetName());
               rxSignalTreeItem->setIcon(0, this->icons[Icon_e::SIGNAL]);
            }
         }

         // Can Network Node Attributes
         auto attributesItem = new QTreeWidgetItem{ canNodeTreeItem };
         attributesItem->setText(0, "Attributes");
         attributesItem->setIcon(0, this->icons[Icon_e::ATTRIBUTES]);
         attributesItem->setWhatsThis(0, ItemId::ATTRIBUTES.data());
         attributesItem->setToolTip(0, "Attributes");
      }
   }

   auto canMessagesTreeItem = new QTreeWidgetItem{ networkTreeItem };
   canMessagesTreeItem->setText(0, "Messages");
   canMessagesTreeItem->setIcon(0, this->icons[Icon_e::MESSAGE]);
   canMessagesTreeItem->setWhatsThis(0, ItemId::CAN_MESSAGES.data());
   canMessagesTreeItem->setToolTip(0, "Can messages");
   size_t canMessagesCount = this->canBusConfig->GetMessagesCount();
   for (size_t i = 0; i < canMessagesCount; i++)
   {
      if (ICanMessage* canMessage = this->canBusConfig->GetMessageByIndex(i); canMessage != nullptr)
      {
         auto canMessageTreeItem = new QTreeWidgetItem{ canMessagesTreeItem };
         canMessageTreeItem->setText(0, canMessage->GetName());
         canMessageTreeItem->setIcon(0, this->icons[Icon_e::MESSAGE]);
         canMessageTreeItem->setWhatsThis(0, ItemId::CAN_MESSAGE.data());
         canMessageTreeItem->setToolTip(0, "Can message");

         auto attributesItem = new QTreeWidgetItem{ canMessageTreeItem };
         attributesItem->setText(0, "Attributes");
         attributesItem->setIcon(0, this->icons[Icon_e::ATTRIBUTES]);
         attributesItem->setWhatsThis(0, ItemId::ATTRIBUTES.data());
         attributesItem->setToolTip(0, "Attributes");
      }
   }

   auto canSignalsTreeItem = new QTreeWidgetItem{ networkTreeItem };
   canSignalsTreeItem->setText(0, "Signals");
   canSignalsTreeItem->setWhatsThis(0, ItemId::CAN_SIGNALS.data());
   canSignalsTreeItem->setToolTip(0, "Can signals");
   canSignalsTreeItem->setIcon(0, this->icons[Icon_e::SIGNAL]);
   size_t canSignalsCount = this->canBusConfig->GetSignalsCount();
   for (size_t i = 0; i < canSignalsCount; i++)
   {
      if (ICanSignal* canSignal = this->canBusConfig->GetSignalByIndex(i); canSignal != nullptr)
      {
         auto canSignalTreeItem = new QTreeWidgetItem{ canSignalsTreeItem };
         canSignalTreeItem->setText(0, canSignal->GetName());
         canSignalTreeItem->setIcon(0, this->icons[Icon_e::SIGNAL]);
         canSignalTreeItem->setWhatsThis(0, ItemId::CAN_SIGNAL.data());
         canSignalTreeItem->setToolTip(0, "Can signal");

         auto attributesItem = new QTreeWidgetItem{ canSignalTreeItem };
         attributesItem->setText(0, "Attributes");
         attributesItem->setIcon(0, this->icons[Icon_e::ATTRIBUTES]);
         attributesItem->setWhatsThis(0, ItemId::ATTRIBUTES.data());
         attributesItem->setToolTip(0, "Attributes");

         auto valueTableItem = new QTreeWidgetItem{ canSignalTreeItem };
         valueTableItem->setText(0, "Value table");
         valueTableItem->setIcon(0, this->icons[Icon_e::VALUE_TABLE]);
         valueTableItem->setWhatsThis(0, ItemId::VALUE_TABLE.data());
         valueTableItem->setToolTip(0, "Value table");
      }
   }

   auto canEnvironmentVariablesTreeItem = new QTreeWidgetItem{ networkTreeItem };
   canEnvironmentVariablesTreeItem->setText(0, "Environment variables");
   canEnvironmentVariablesTreeItem->setWhatsThis(0, ItemId::CAN_ENVIRONMENT_VARIABLES.data());
   canEnvironmentVariablesTreeItem->setToolTip(0, "Can environment variables");
   canEnvironmentVariablesTreeItem->setIcon(0, this->icons[Icon_e::ENVIRONMENT_VARIABLE]);
   size_t canEnvironmentVariablesCount = this->canBusConfig->GetEnvVarsCount();
   for (size_t i = 0; i < canEnvironmentVariablesCount; i++)
   {
      if (const auto canEnvVar = this->canBusConfig->GetEnvVarByIndex(i); canEnvVar != nullptr)
      {
         auto canEnvironmentVariableTreeItem = new QTreeWidgetItem{ canEnvironmentVariablesTreeItem };
         canEnvironmentVariableTreeItem->setText(0, canEnvVar->GetName());
         canEnvironmentVariableTreeItem->setIcon(0, this->icons[Icon_e::ENVIRONMENT_VARIABLE]);
         canEnvironmentVariableTreeItem->setWhatsThis(0, ItemId::CAN_ENVIRONMENT_VARIABLE.data());
         canEnvironmentVariableTreeItem->setToolTip(0, "Can environment variable");

         auto attributesItem = new QTreeWidgetItem{ canEnvironmentVariableTreeItem };
         attributesItem->setText(0, "Attributes");
         attributesItem->setIcon(0, this->icons[Icon_e::ATTRIBUTES]);
         attributesItem->setWhatsThis(0, ItemId::ATTRIBUTES.data());
         attributesItem->setToolTip(0, "Attributes");

         auto valueTableItem = new QTreeWidgetItem{ canEnvironmentVariableTreeItem };
         valueTableItem->setText(0, "Value table");
         valueTableItem->setIcon(0, this->icons[Icon_e::VALUE_TABLE]);
         valueTableItem->setWhatsThis(0, ItemId::VALUE_TABLE.data());
         valueTableItem->setToolTip(0, "Value table");
      }
   }

   auto attributesItem = new QTreeWidgetItem{ networkTreeItem };
   attributesItem->setText(0, "Attributes");
   attributesItem->setIcon(0, this->icons[Icon_e::ATTRIBUTES]);
   attributesItem->setWhatsThis(0, ItemId::ATTRIBUTES.data());
   attributesItem->setToolTip(0, "Attributes");
}

void BusConfigUI::Clear(void)
{
   this->canBusConfig->Clear();
   this->ui.treeWidget_MainView->clear();
   for (size_t i = 0; i < this->ui.tableWidget_Properties->columnCount(); i++)
   {
      this->ui.tableWidget_Properties->setItemDelegateForColumn(i, nullptr);
   }
   this->ui.tableWidget_Properties->clear();
   this->ui.tableWidget_Properties->setRowCount(0);
   this->ui.tableWidget_Properties->setColumnCount(0);
}

void BusConfigUI::BuildCanMessageProperties(const QString& messageName)
{
   if (const auto message = this->canBusConfig->GetMessageByName(messageName.toUtf8()); message != nullptr)
   {
      QStringList headerLabels;
      ranges::for_each(CanMessageManager::PROPERTIES, [&headerLabels](const std::string_view property)
         { headerLabels << property.data(); });

      this->ui.tableWidget_Properties->setRowCount(1);
      this->ui.tableWidget_Properties->setColumnCount(headerLabels.size());
      this->ui.tableWidget_Properties->setHorizontalHeaderLabels(headerLabels);
      this->ui.tableWidget_Properties->setItem(0, 0, new QTableWidgetItem{ this->icons[Icon_e::MESSAGE], message->GetName() });
      this->ui.tableWidget_Properties->setItem(0, 1, new QTableWidgetItem{ toQString(message->GetId()) });

      ComboDelegate* idFormatsDelegate = new ComboDelegate{ CanMessageManager::ID_FORMATS, 50, 150 };
      this->ui.tableWidget_Properties->setItemDelegateForColumn(2, idFormatsDelegate);
      ComboDelegate* txMethodsDelegate = new ComboDelegate{ CanMessageManager::TX_METHODS, 50, 150 };
      this->ui.tableWidget_Properties->setItemDelegateForColumn(4, txMethodsDelegate);

      int pos = static_cast<int>(message->GetIdFormat());
      const QString idFormat = pos >= 0 ? CanMessageManager::ID_FORMATS[pos] : CanMessageManager::IdFormat::DEFAULT.data();
      pos = static_cast<int>(message->GetTxMethod());
      const QString txMethod = pos >= 0 ? CanMessageManager::TX_METHODS[pos] : ICanMessage::TxMethod::DEFAULT;

      this->ui.tableWidget_Properties->setItem(0, 2, new QTableWidgetItem{ idFormat });
      this->ui.tableWidget_Properties->setItem(0, 3, new QTableWidgetItem{ toQString(message->GetSize()) });
      this->ui.tableWidget_Properties->setItem(0, 4, new QTableWidgetItem{ txMethod });
      this->ui.tableWidget_Properties->setItem(0, 5, new QTableWidgetItem{ toQString(message->GetCycleTime()) });
      this->ui.tableWidget_Properties->setItem(0, 6, new QTableWidgetItem{ message->GetComment() });
   }
}

void BusConfigUI::BuildCanMessagesProperties(void)
{
   size_t canMessagesCount = this->canBusConfig->GetMessagesCount();
   QStringList headerLabels;
   ranges::for_each(CanMessageManager::PROPERTIES, [&headerLabels] (const std::string_view property)
      { headerLabels << property.data(); });

   this->ui.tableWidget_Properties->setRowCount(canMessagesCount);
   this->ui.tableWidget_Properties->setColumnCount(headerLabels.size());
   this->ui.tableWidget_Properties->setHorizontalHeaderLabels(headerLabels);

   for (size_t i = 0; i < canMessagesCount; i++)
   {
      if (const auto message = this->canBusConfig->GetMessageByIndex(i); message != nullptr)
      {
         this->ui.tableWidget_Properties->setItem(i, 0, new QTableWidgetItem{ this->icons[Icon_e::MESSAGE], message->GetName() });
         if (this->base == Base_e::DEC)
         {
            this->ui.tableWidget_Properties->setItem(i, 1, new QTableWidgetItem{ toQString(message->GetId()) });
         }
         else
         {
            this->ui.tableWidget_Properties->setItem(i, 1, new QTableWidgetItem{ toHexQString(message->GetId()) });
         }

         int pos = static_cast<int>(message->GetIdFormat());
         const QString idFormat = pos >= 0 ? CanMessageManager::ID_FORMATS[pos] : CanMessageManager::IdFormat::DEFAULT.data();
         pos = static_cast<int>(message->GetTxMethod());
         const QString txMethod = pos >= 0 ? CanMessageManager::TX_METHODS[pos] : ICanMessage::TxMethod::DEFAULT;

         this->ui.tableWidget_Properties->setItem(i, 2, new QTableWidgetItem{ idFormat });
         this->ui.tableWidget_Properties->setItem(i, 3, new QTableWidgetItem{ toQString(message->GetSize()) });
         this->ui.tableWidget_Properties->setItem(i, 4, new QTableWidgetItem{ txMethod });
         this->ui.tableWidget_Properties->setItem(i, 5, new QTableWidgetItem{ toQString(message->GetCycleTime()) });
         this->ui.tableWidget_Properties->setItem(i, 6, new QTableWidgetItem{ message->GetComment() });
      }
   }

   ComboDelegate* idFormatsDelegate = new ComboDelegate{ CanMessageManager::ID_FORMATS, 25, 150 };
   this->ui.tableWidget_Properties->setItemDelegateForColumn(2, idFormatsDelegate);
   ComboDelegate* txMethodsDelegate = new ComboDelegate{ CanMessageManager::TX_METHODS, 25, 150 };
   this->ui.tableWidget_Properties->setItemDelegateForColumn(4, txMethodsDelegate);
}

void BusConfigUI::BuildCanSignalProperties(const QString& signalName)
{
   if (const auto signal = this->canBusConfig->GetSignalByName(signalName.toUtf8()); signal)
   {
      QStringList headerLabels;
      ranges::for_each(CanSignalManager::PROPERTIES, [&headerLabels] (const std::string_view property)
         { headerLabels << property.data(); });

      this->ui.tableWidget_Properties->setRowCount(1);
      this->ui.tableWidget_Properties->setColumnCount(headerLabels.size());
      this->ui.tableWidget_Properties->setHorizontalHeaderLabels(headerLabels);
      this->ui.tableWidget_Properties->setItem(0, 0, new QTableWidgetItem{ this->icons[Icon_e::SIGNAL], signal->GetName() });
      
      this->ui.tableWidget_Properties->setItem(0, 1, new QTableWidgetItem { toQString(signal->GetStartBit()) });
      this->ui.tableWidget_Properties->setItem(0, 2, new QTableWidgetItem { toQString(signal->GetSize()) });

      ComboDelegate* byteOrderDelegate = new ComboDelegate{ CanSignalManager::BYTE_ORDERS };
      this->ui.tableWidget_Properties->setItemDelegateForColumn(3, byteOrderDelegate);
      ComboDelegate* valueTypeDelegate = new ComboDelegate{ CanSignalManager::VALUE_TYPES };
      this->ui.tableWidget_Properties->setItemDelegateForColumn(4, valueTypeDelegate);

      int pos = static_cast<int>(signal->GetByteOrder());
      const QString byteOrder = pos >= 0 ? CanSignalManager::BYTE_ORDERS[pos] : CanSignalManager::ByteOrder::DEFAULT.data();
      pos = static_cast<int>(signal->GetValueType());
      const QString valueType = pos >= 0 ? CanSignalManager::VALUE_TYPES[pos] : CanSignalManager::ValueType::DEFAULT.data();

      const QString valueTableName = std::invoke([&signal]
      {
         if (ICanValueTable* valueTable = signal->GetValueTable(); valueTable != nullptr)
         {
            return valueTable->GetName();
         }
         return "";
      });

      this->ui.tableWidget_Properties->setItem(0, 3, new QTableWidgetItem{ byteOrder });
      this->ui.tableWidget_Properties->setItem(0, 4, new QTableWidgetItem{ valueType });
      
      this->ui.tableWidget_Properties->setItem(0, 5, new QTableWidgetItem{ toQString(signal->GetFactor()) });
      this->ui.tableWidget_Properties->setItem(0, 6, new QTableWidgetItem{ toQString(signal->GetOffset()) });
      this->ui.tableWidget_Properties->setItem(0, 7, new QTableWidgetItem{ toQString(signal->GetMinimum()) });
      this->ui.tableWidget_Properties->setItem(0, 8, new QTableWidgetItem{ toQString(signal->GetMaximum()) });

      this->ui.tableWidget_Properties->setItem(0, 9, new QTableWidgetItem{ signal->GetUnit() });
      this->ui.tableWidget_Properties->setItem(0, 10, new QTableWidgetItem{ valueTableName });
      this->ui.tableWidget_Properties->setItem(0, 11, new QTableWidgetItem{ signal->GetComment() });
   }
}

void BusConfigUI::BuildCanSignalsProperties(void)
{
   size_t canSignalsCount = this->canBusConfig->GetSignalsCount();
   QStringList headerLabels;
   ranges::for_each(CanSignalManager::PROPERTIES, [&headerLabels] (std::string_view property)
      { headerLabels << property.data(); });

   this->ui.tableWidget_Properties->setRowCount(canSignalsCount);
   this->ui.tableWidget_Properties->setColumnCount(headerLabels.size());
   this->ui.tableWidget_Properties->setHorizontalHeaderLabels(headerLabels);

   for (size_t i = 0; i < canSignalsCount; i++)
   {
      if (const auto signal = this->canBusConfig->GetSignalByIndex(i); signal)
      {
         this->ui.tableWidget_Properties->setItem(i, 0, new QTableWidgetItem{ this->icons[Icon_e::SIGNAL], signal->GetName() });

         this->ui.tableWidget_Properties->setItem(i, 1, new QTableWidgetItem{ toQString(signal->GetStartBit()) });
         this->ui.tableWidget_Properties->setItem(i, 2, new QTableWidgetItem{ toQString(signal->GetSize()) });

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

         this->ui.tableWidget_Properties->setItem(i, 3, new QTableWidgetItem { byteOrder });
         this->ui.tableWidget_Properties->setItem(i, 4, new QTableWidgetItem { valueType });

         this->ui.tableWidget_Properties->setItem(i, 5, new QTableWidgetItem { toQString(signal->GetFactor()) });
         this->ui.tableWidget_Properties->setItem(i, 6, new QTableWidgetItem{ toQString(signal->GetOffset()) });
         this->ui.tableWidget_Properties->setItem(i, 7, new QTableWidgetItem{ toQString(signal->GetMinimum()) });
         this->ui.tableWidget_Properties->setItem(i, 8, new QTableWidgetItem{ toQString(signal->GetMaximum()) });

         this->ui.tableWidget_Properties->setItem(i, 9, new QTableWidgetItem{ signal->GetUnit() });
         this->ui.tableWidget_Properties->setItem(i, 10, new QTableWidgetItem{ valueTableName });
         this->ui.tableWidget_Properties->setItem(i, 11, new QTableWidgetItem{ signal->GetComment() });
      }
   }

   ComboDelegate* byteOrderDelegate = new ComboDelegate{ CanSignalManager::BYTE_ORDERS };
   this->ui.tableWidget_Properties->setItemDelegateForColumn(3, byteOrderDelegate);
   ComboDelegate* valueTypeDelegate = new ComboDelegate{ CanSignalManager::VALUE_TYPES };
   this->ui.tableWidget_Properties->setItemDelegateForColumn(4, valueTypeDelegate);
}

void BusConfigUI::BuildCanEnvironmentVariableProperties(const QString& envVarName)
{
   if (const auto canEnvVar = this->canBusConfig->GetEnvVarByName(envVarName.toUtf8()); canEnvVar)
   {
      QStringList headerLabels;
      std::variant<ICanEnvVar*, ICanIntEnvVar*, ICanFloatEnvVar*, ICanStringEnvVar*, ICanDataEnvVar*>
         envVar = canEnvVar;
      headerLabels << "Name" << "Type" << "Unit" << "Minimum" << "Maximum" << "Initial value";
      headerLabels << "Length (Bytes)" << "Access" << "Value table" << "Comment";
      this->ui.tableWidget_Properties->setRowCount(1);
      this->ui.tableWidget_Properties->setColumnCount(headerLabels.size());
      this->ui.tableWidget_Properties->setHorizontalHeaderLabels(headerLabels);
      this->ui.tableWidget_Properties->setItem(0, 0, new QTableWidgetItem{ this->icons[Icon_e::ENVIRONMENT_VARIABLE], canEnvVar->GetName() });

      const QString type = std::invoke([&canEnvVar, &envVar]
      {
         switch (canEnvVar->GetType())
         {
            case ICanEnvVar::Type_e::INTEGER:
            {
               envVar = dynamic_cast<ICanIntEnvVar*>(canEnvVar);
               return "Integer";
            }
            case ICanEnvVar::Type_e::FLOAT:
            {
               envVar = dynamic_cast<ICanFloatEnvVar*>(canEnvVar);
               return "Float";
            }
            case ICanEnvVar::Type_e::STRING:
            {
               envVar = dynamic_cast<ICanStringEnvVar*>(canEnvVar);
               return "String";
            }
            case ICanEnvVar::Type_e::DATA:
            {
               envVar = dynamic_cast<ICanDataEnvVar*>(canEnvVar);
               return "Data";
            }
         }
         return "Integer";
      });

      const QString accessType = std::invoke([&canEnvVar] () -> QString
      {
         switch (canEnvVar->GetAccessType())
         {
            case ICanEnvVar::AccessType_e::UNRESTRICTED:
            {
               return "Unrestricted";
            }
            case ICanEnvVar::AccessType_e::READ:
            {
               return "Read";
            }
            case ICanEnvVar::AccessType_e::WRITE:
            {
               return "Write";
            }
            case ICanEnvVar::AccessType_e::READ_WRITE:
            {
               return "Read & Write";
            }
         }
         return "Unrestricted";
      });

      const QString valueTableName = std::invoke([&canEnvVar]
      {
         if (const auto valueTable = canEnvVar->GetValueTable(); valueTable)
         {
            return valueTable->GetName();
         }
         return "";
      });

      this->ui.tableWidget_Properties->setItem(0, 1, new QTableWidgetItem{ type });
      this->ui.tableWidget_Properties->setItem(0, 2, new QTableWidgetItem{ canEnvVar->GetUnit() });

      std::visit([this] (auto&& arg)
      {
         using T = std::decay_t<decltype(arg)>;
         if constexpr (std::is_same_v<T, ICanIntEnvVar*> || std::is_same_v<T, ICanFloatEnvVar*>)
         {
            this->ui.tableWidget_Properties->setItem(0, 3, new QTableWidgetItem{ toQString(arg->GetMinimum()) });
            this->ui.tableWidget_Properties->setItem(0, 4, new QTableWidgetItem{ toQString(arg->GetMaximum()) });
            this->ui.tableWidget_Properties->setItem(0, 5, new QTableWidgetItem{ toQString(arg->GetInitialValue()) });
            this->ui.tableWidget_Properties->setItem(0, 6, new QTableWidgetItem{ "-" });
         }
         else if constexpr (std::is_same_v<T, ICanDataEnvVar*>)
         {
            this->ui.tableWidget_Properties->setItem(0, 3, new QTableWidgetItem{ "-" });
            this->ui.tableWidget_Properties->setItem(0, 4, new QTableWidgetItem{ "-" });
            this->ui.tableWidget_Properties->setItem(0, 5, new QTableWidgetItem{ "-" });
            this->ui.tableWidget_Properties->setItem(0, 6, new QTableWidgetItem{ toQString(arg->GetLength()) });
         }
         else
         {
            this->ui.tableWidget_Properties->setItem(0, 3, new QTableWidgetItem{ "-" });
            this->ui.tableWidget_Properties->setItem(0, 4, new QTableWidgetItem{ "-" });
            this->ui.tableWidget_Properties->setItem(0, 5, new QTableWidgetItem{ "-" });
            this->ui.tableWidget_Properties->setItem(0, 6, new QTableWidgetItem{ "-" });
         }
      }, envVar);

      /*if (canEnvVar->GetType() == ICanEnvVar::Type_e::INTEGER)
      {
         this->ui.tableWidget_Properties->setItem(0, 3, new QTableWidgetItem{ toQString(std::get<ICanIntEnvVar*>(envVar)->GetMinimum()) });
         this->ui.tableWidget_Properties->setItem(0, 4, new QTableWidgetItem{ toQString(std::get<ICanIntEnvVar*>(envVar)->GetMaximum()) });
         this->ui.tableWidget_Properties->setItem(0, 5, new QTableWidgetItem{ toQString(std::get<ICanIntEnvVar*>(envVar)->GetInitialValue()) });
         this->ui.tableWidget_Properties->setItem(0, 6, new QTableWidgetItem{ "-" });
      }
      else if (canEnvVar->GetType() == ICanEnvVar::Type_e::FLOAT)
      {
         this->ui.tableWidget_Properties->setItem(0, 3, new QTableWidgetItem{ toQString(std::get<ICanFloatEnvVar*>(envVar)->GetMinimum()) });
         this->ui.tableWidget_Properties->setItem(0, 4, new QTableWidgetItem{ toQString(std::get<ICanFloatEnvVar*>(envVar)->GetMaximum()) });
         this->ui.tableWidget_Properties->setItem(0, 5, new QTableWidgetItem{ toQString(std::get<ICanFloatEnvVar*>(envVar)->GetInitialValue()) });
         this->ui.tableWidget_Properties->setItem(0, 6, new QTableWidgetItem{ "-" });
      }
      else if (canEnvVar->GetType() == ICanEnvVar::Type_e::DATA)
      {
         this->ui.tableWidget_Properties->setItem(0, 3, new QTableWidgetItem{ "-" });
         this->ui.tableWidget_Properties->setItem(0, 4, new QTableWidgetItem{ "-" });
         this->ui.tableWidget_Properties->setItem(0, 5, new QTableWidgetItem{ "-" });
         this->ui.tableWidget_Properties->setItem(0, 6, new QTableWidgetItem{ toQString(std::get<ICanDataEnvVar*>(envVar)->GetLength()) });
      }
      else
      {
         this->ui.tableWidget_Properties->setItem(0, 3, new QTableWidgetItem{ "-" });
         this->ui.tableWidget_Properties->setItem(0, 4, new QTableWidgetItem{ "-" });
         this->ui.tableWidget_Properties->setItem(0, 5, new QTableWidgetItem{ "-" });
         this->ui.tableWidget_Properties->setItem(0, 6, new QTableWidgetItem{ "-" });
      }*/

      this->ui.tableWidget_Properties->setItem(0, 7, new QTableWidgetItem{ accessType });
      this->ui.tableWidget_Properties->setItem(0, 8, new QTableWidgetItem{ valueTableName });
      this->ui.tableWidget_Properties->setItem(0, 9, new QTableWidgetItem{ canEnvVar->GetComment() });
      //this->ui.tableWidget_Properties->setEditTriggers(QAbstractItemView::NoEditTriggers);
   }
}

void BusConfigUI::BuildCanEnvironmentVariablesProperties(void)
{
   size_t canEnvVarsCount = this->canBusConfig->GetEnvVarsCount();
   QStringList headerLabels;
   headerLabels << "Name" << "Type" << "Unit" << "Minimum" << "Maximum" << "Initial value";
   headerLabels << "Length (Bytes)" << "Access" << "Value table" << "Comment";
   this->ui.tableWidget_Properties->setRowCount(canEnvVarsCount);
   this->ui.tableWidget_Properties->setColumnCount(headerLabels.size());
   this->ui.tableWidget_Properties->setHorizontalHeaderLabels(headerLabels);
   std::variant<const ICanEnvVar*, ICanIntEnvVar*, ICanFloatEnvVar*, ICanStringEnvVar*, ICanDataEnvVar*>
      envVar;

   for (size_t i = 0; i < canEnvVarsCount; i++)
   {
      if (const auto canEnvVar = this->canBusConfig->GetEnvVarByIndex(i); canEnvVar)
      {
         envVar = canEnvVar;
         this->ui.tableWidget_Properties->setItem(i, 0, new QTableWidgetItem{ this->icons[Icon_e::ENVIRONMENT_VARIABLE], canEnvVar->GetName() });

         const QString type = std::invoke([&canEnvVar, &envVar]() -> QString
         {
            switch (canEnvVar->GetType())
            {
               case ICanEnvVar::Type_e::INTEGER:
               {
                  envVar = dynamic_cast<ICanIntEnvVar*>(canEnvVar);
                  return "Integer";
               }
               case ICanEnvVar::Type_e::FLOAT:
               {
                  envVar = dynamic_cast<ICanFloatEnvVar*>(canEnvVar);
                  return "Float";
               }
               case ICanEnvVar::Type_e::STRING:
               {
                  envVar = dynamic_cast<ICanStringEnvVar*>(canEnvVar);
                  return "String";
               }
               case ICanEnvVar::Type_e::DATA:
               {
                  envVar = dynamic_cast<ICanDataEnvVar*>(canEnvVar);
                  return "Data";
               }
            }
            return "Integer";
         });

         const QString accessType = std::invoke([&canEnvVar]() -> QString
         {
            switch (canEnvVar->GetAccessType())
            {
               case ICanEnvVar::AccessType_e::UNRESTRICTED:
               {
                  return "Unrestricted";
               }
               case ICanEnvVar::AccessType_e::READ:
               {
                  return "Read";
               }
               case ICanEnvVar::AccessType_e::WRITE:
               {
                  return "Write";
               }
               case ICanEnvVar::AccessType_e::READ_WRITE:
               {
                  return "Read & Write";
               }
            }
            return "Unrestricted";
         });

         const QString valueTableName = std::invoke([&canEnvVar]() -> QString
         {
            if (ICanValueTable* valueTable = canEnvVar->GetValueTable(); valueTable)
            {
               return valueTable->GetName();
            }
            return "";
         });

         this->ui.tableWidget_Properties->setItem(i, 1, new QTableWidgetItem{ type });
         this->ui.tableWidget_Properties->setItem(i, 2, new QTableWidgetItem{ canEnvVar->GetUnit() });

         std::visit([this, &i] (auto&& arg)
         {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, ICanIntEnvVar*>)
            {
               this->ui.tableWidget_Properties->setItem(i, 3, new QTableWidgetItem{ toQString(arg->GetMinimum()) });
               this->ui.tableWidget_Properties->setItem(i, 4, new QTableWidgetItem{ toQString(arg->GetMaximum()) });
               this->ui.tableWidget_Properties->setItem(i, 5, new QTableWidgetItem{ toQString(arg->GetInitialValue()) });
               this->ui.tableWidget_Properties->setItem(i, 6, new QTableWidgetItem{ "-" });
            }
            else if constexpr (std::is_same_v<T, ICanFloatEnvVar*>)
            {
               this->ui.tableWidget_Properties->setItem(i, 3, new QTableWidgetItem{ toQString(arg->GetMinimum()) });
               this->ui.tableWidget_Properties->setItem(i, 4, new QTableWidgetItem{ toQString(arg->GetMaximum()) });
               this->ui.tableWidget_Properties->setItem(i, 5, new QTableWidgetItem{ toQString(arg->GetInitialValue()) });
               this->ui.tableWidget_Properties->setItem(i, 6, new QTableWidgetItem{ "-" });
            }
            else if constexpr (std::is_same_v<T, ICanDataEnvVar*>)
            {
               this->ui.tableWidget_Properties->setItem(i, 3, new QTableWidgetItem{ "-" });
               this->ui.tableWidget_Properties->setItem(i, 4, new QTableWidgetItem{ "-" });
               this->ui.tableWidget_Properties->setItem(i, 5, new QTableWidgetItem{ "-" });
               this->ui.tableWidget_Properties->setItem(i, 6, new QTableWidgetItem{ toQString(arg->GetLength()) });
            }
            else
            {
               this->ui.tableWidget_Properties->setItem(i, 3, new QTableWidgetItem{ "-" });
               this->ui.tableWidget_Properties->setItem(i, 4, new QTableWidgetItem{ "-" });
               this->ui.tableWidget_Properties->setItem(i, 5, new QTableWidgetItem{ "-" });
               this->ui.tableWidget_Properties->setItem(i, 6, new QTableWidgetItem{ "-" });
            }
         }, envVar);

         /*if (canEnvVar->GetType() == ICanEnvVar::Type_e::INTEGER)
         {
            this->ui.tableWidget_Properties->setItem(i, 3, new QTableWidgetItem{ toQString(std::get<ICanIntEnvVar*>(envVar)->GetMinimum()) });
            this->ui.tableWidget_Properties->setItem(i, 4, new QTableWidgetItem{ toQString(std::get<ICanIntEnvVar*>(envVar)->GetMaximum()) });
            this->ui.tableWidget_Properties->setItem(i, 5, new QTableWidgetItem{ toQString(std::get<ICanIntEnvVar*>(envVar)->GetInitialValue()) });
            this->ui.tableWidget_Properties->setItem(i, 6, new QTableWidgetItem{ "-" });
         }
         else if (canEnvVar->GetType() == ICanEnvVar::Type_e::FLOAT)
         {
            this->ui.tableWidget_Properties->setItem(i, 3, new QTableWidgetItem{ toQString(std::get<ICanFloatEnvVar*>(envVar)->GetMinimum()) });
            this->ui.tableWidget_Properties->setItem(i, 4, new QTableWidgetItem{ toQString(std::get<ICanFloatEnvVar*>(envVar)->GetMaximum()) });
            this->ui.tableWidget_Properties->setItem(i, 5, new QTableWidgetItem{ toQString(std::get<ICanFloatEnvVar*>(envVar)->GetInitialValue()) });
            this->ui.tableWidget_Properties->setItem(i, 6, new QTableWidgetItem{ "-" });
         }
         else if (canEnvVar->GetType() == ICanEnvVar::Type_e::DATA)
         {
            this->ui.tableWidget_Properties->setItem(i, 3, new QTableWidgetItem{ "-" });
            this->ui.tableWidget_Properties->setItem(i, 4, new QTableWidgetItem{ "-" });
            this->ui.tableWidget_Properties->setItem(i, 5, new QTableWidgetItem{ "-" });
            this->ui.tableWidget_Properties->setItem(i, 6, new QTableWidgetItem{ toQString(std::get<ICanDataEnvVar*>(envVar)->GetLength()) });
         }
         else
         {
            this->ui.tableWidget_Properties->setItem(i, 3, new QTableWidgetItem{ "-" });
            this->ui.tableWidget_Properties->setItem(i, 4, new QTableWidgetItem{ "-" });
            this->ui.tableWidget_Properties->setItem(i, 5, new QTableWidgetItem{ "-" });
            this->ui.tableWidget_Properties->setItem(i, 6, new QTableWidgetItem{ "-" });
         } */

         this->ui.tableWidget_Properties->setItem(i, 7, new QTableWidgetItem{ accessType });
         this->ui.tableWidget_Properties->setItem(i, 8, new QTableWidgetItem{ valueTableName });
         this->ui.tableWidget_Properties->setItem(i, 9, new QTableWidgetItem{ canEnvVar->GetComment() });
         //this->ui.tableWidget_Properties->setEditTriggers(QAbstractItemView::NoEditTriggers);
      }
   }
}

void BusConfigUI::BuildCanValueTableProperties(const QString& canValueTableOwnerType, const QString& canValueTableOwnerName)
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
      if (const auto canSignal = this->canBusConfig->GetSignalByName(canValueTableOwnerName.toUtf8()); canSignal)
      {
         FillValueTable(canSignal->GetValueTable());
      }
   }
   else if (canValueTableOwnerType == ItemId::CAN_ENVIRONMENT_VARIABLE.data())
   {
      if (const auto canEnvVar = this->canBusConfig->GetEnvVarByName(canValueTableOwnerName.toUtf8()); canEnvVar)
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

void BusConfigUI::ChangeTreeWidgetItemName(const QString& itemName, const QString& newItemName)
{
   auto treeItems = this->Find(itemName);
   for (auto& treeItem : treeItems)
   {
      treeItem->setText(0, newItemName);
   }
}