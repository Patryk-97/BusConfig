#include "BusConfigUI.h"
#include "DllLoader.h"
#include "ICanBusConfig.h"
#include <qmessagebox.h>
#include <QFileDialog>

DllLoader<ICanBusConfig, bool> dllLoader{ "BusConfigDll", "CanBusConfigInstanceCreate", "CanBusConfigInstanceDelete" };

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
       this->AddLog("Successfully loaded BusConfigDll.dll");
       this->setWindowState(Qt::WindowMaximized);
       this->canBusConfig = dllLoader.pfCreate();
    }
    else
    {
      this->AddLog("Loading BusConfigDll.dll failed");
    }
}

void BusConfigUI::on_actionClear_triggered()
{
   this->Clear();
   this->AddLog("Database cleaned");
}

void BusConfigUI::on_actionOpen_triggered()
{
   QFileDialog dlgOpen(this);
   dlgOpen.setWindowTitle("Open bus configuration file");
   dlgOpen.setFileMode(QFileDialog::ExistingFile);
   dlgOpen.setNameFilter(trUtf8("Bus configuration files (*.dbc *.ldf)"));
   QStringList fileName;
   if (dlgOpen.exec())
   {
      fileName = dlgOpen.selectedFiles();
      this->LoadFile(fileName[0]);
   }
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
      this->AddLog(QString("Ldf bus configuration file has not supported yet.\r\n"));
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
   networkTreeItem->setText(0, "Network");

   auto networkNodesTreeItem = new QTreeWidgetItem{ networkTreeItem };
   networkNodesTreeItem->setText(0, "Network nodes");
   size_t canNodesCount = this->canBusConfig->GetNodesCount();
   for (size_t i = 0; i < canNodesCount; i++)
   {
      if (const auto canNode = this->canBusConfig->GetNodeByIndex(i); canNode != nullptr)
      {
         auto canNodeTreeItem = new QTreeWidgetItem{ networkNodesTreeItem };
         canNodeTreeItem->setText(0, canNode->GetName());

         // Tx Messages
         auto txMessagesTreeItem = new QTreeWidgetItem{ canNodeTreeItem };
         txMessagesTreeItem->setText(0, "Tx Messages");
         size_t txMessagesCount = canNode->GetTxMessagesCount();
         for (size_t j = 0; j < txMessagesCount; j++)
         {
            if (const auto txMessage = canNode->GetTxMessageByIndex(j); txMessage != nullptr)
            {
               auto txMessageTreeItem = new QTreeWidgetItem{ txMessagesTreeItem };
               txMessageTreeItem->setText(0, txMessage->GetName());
            }
         }

         // Rx Messages
         auto rxMessagesTreeItem = new QTreeWidgetItem{ canNodeTreeItem };
         rxMessagesTreeItem->setText(0, "Rx Messages");
         size_t rxMessagesCount = canNode->GetRxMessagesCount();
         for (size_t j = 0; j < rxMessagesCount; j++)
         {
            if (const auto rxMessage = canNode->GetRxMessageByIndex(j); rxMessage != nullptr)
            {
               auto rxMessageTreeItem = new QTreeWidgetItem{ rxMessagesTreeItem };
               rxMessageTreeItem->setText(0, rxMessage->GetName());
            }
         }

         // Mapped Tx Signals
         auto txSignalsTreeItem = new QTreeWidgetItem{ canNodeTreeItem };
         txSignalsTreeItem->setText(0, "Mapped Tx Signals");
         size_t txSignalsCount = canNode->GetMappedTxSignalsCount();
         for (size_t j = 0; j < txSignalsCount; j++)
         {
            if (const auto txSignal = canNode->GetMappedTxSignalByIndex(j); txSignal != nullptr)
            {
               auto txSignalTreeItem = new QTreeWidgetItem{ txSignalsTreeItem };
               txSignalTreeItem->setText(0, txSignal->GetName());
            }
         }

         // Mapped Rx Signals
         auto rxSignalsTreeItem = new QTreeWidgetItem { canNodeTreeItem };
         rxSignalsTreeItem->setText(0, "Mapped Rx Signals");
         size_t rxSignalsCount = canNode->GetMappedRxSignalsCount();
         for (size_t j = 0; j < rxSignalsCount; j++)
         {
            if (const auto rxSignal = canNode->GetMappedRxSignalByIndex(j); rxSignal != nullptr)
            {
               auto rxSignalTreeItem = new QTreeWidgetItem{ rxSignalsTreeItem };
               rxSignalTreeItem->setText(0, rxSignal->GetName());
            }
         }
      }
   }

   auto canMessagesTreeItem = new QTreeWidgetItem{ networkTreeItem };
   canMessagesTreeItem->setText(0, "Messages");
   auto canSignalsTreeItem = new QTreeWidgetItem{ networkTreeItem };
   canSignalsTreeItem->setText(0, "Signals");
   size_t canMessagesCount = this->canBusConfig->GetMessagesCount();
   for (size_t i = 0; i < canMessagesCount; i++)
   {
      if (ICanMessage* canMessage = this->canBusConfig->GetMessageByIndex(i); canMessage != nullptr)
      {
         auto canMessageTreeItem = new QTreeWidgetItem{ canMessagesTreeItem };
         canMessageTreeItem->setText(0, canMessage->GetName());

         size_t canSignalsCount = canMessage->GetSignalsCount();
         for (size_t j = 0; j < canSignalsCount; j++)
         {
            if (ICanSignal* canSignal = canMessage->GetSignalByIndex(j); canSignal != nullptr)
            {
               auto canSignalTreeItem = new QTreeWidgetItem{ canSignalsTreeItem };
               canSignalTreeItem->setText(0, canSignal->GetName());
            }
         }
      }
   }
}

void BusConfigUI::Clear(void)
{
   this->canBusConfig->Clear();
   this->ui.treeWidget_MainView->clear();
}