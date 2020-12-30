#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_BusConfigUI.h"
#include "ICanBusConfig.h"
#include "CommunicationMatrix.h"
#include <qmap.h>

class BusConfigUI : public QMainWindow
{
    Q_OBJECT
public:
    BusConfigUI(QWidget *parent = Q_NULLPTR);

    friend CommunicationMatrix;

private slots:
   void on_actionClear_triggered();
   void on_actionOpen_triggered();
   void on_actionExit_triggered();
   void on_actionBase_triggered();
   void on_actionCommunication_matrix_triggered();
   void on_treeWidget_MainView_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
   void ShowMenuForTableWidgetItem(const QPoint& pos);
private:

   bool LoadFile(const QString& fileName);
   bool LoadDbcFile(const QString& fileName);
   void AddLog(const QString& log);
   void BuildTree(void);
   void Clear(void);
   
   // Table widget properties
   void BuildCanMessageProperties(const char * messageName);
   void BuildCanMessagesProperties(void);
   void BuildCanSignalProperties(const char* signalName);
   void BuildCanSignalsProperties(void);
   void BuildCanEnvironmentVariableProperties(const char* envVarName);
   void BuildCanEnvironmentVariablesProperties(void);
   void BuildAttributesProperties(const ICanAttributeOwner* attributeOwner);

   void RemoveFromTreeWidget(const QString& itemName);

   template <std::integral Integral>
   inline void RemoveCanMessage(const Integral& integral)
   {
      this->canBusConfig->RemoveMessageByIndex(integral);
   }

   inline void RemoveCanMessage(const QString& name)
   {
      this->canBusConfig->RemoveMessageByName(name.toUtf8());
   }

   template <std::integral Integral>
   inline void RemoveCanSignal(const Integral& integral)
   {
      this->canBusConfig->RemoveSignalByIndex(integral);
   }

   inline void RemoveCanSignal(const QString& name)
   {
      this->canBusConfig->RemoveSignalByName(name.toUtf8());
   }

   template <std::integral Integral>
   inline void RemoveCanEnvVar(const Integral& integral)
   {
      this->canBusConfig->RemoveEnvVarByIndex(integral);
   }

   inline void RemoveCanEnvVar(const QString& name)
   {
      this->canBusConfig->RemoveEnvVarByName(name.toUtf8());
   }

   // member variables
    Ui::BusConfigUIClass ui;
    ICanBusConfig* canBusConfig { nullptr };
    CommunicationMatrix* communicationMatrix { new CommunicationMatrix {} };

    enum class Icon_e
    {
       MESSAGE = 0,
       SIGNAL = 1,
       NETWORK_NODE = 2,
       NETWORK = 3,
       ENVIRONMENT_VARIABLE = 4,
       HEX = 10,
       DEC = 11,
       COMMUNICATION_MATRIX = 12
    };

    enum class Base_e
    {
       DEC = 0,
       HEX = 1
    };

    QMap<Icon_e, QIcon> icons;
    Base_e base { Base_e::DEC };
};
