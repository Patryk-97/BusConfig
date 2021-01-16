#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_BusConfigUI.h"
#include "ICanBusConfig.h"
#include "CommunicationMatrix.h"
#include "AttributeDefinitions.h"
#include "CanMessageSimulator.h"
#include <qmap.h>

class BusConfigUI : public QMainWindow
{
    Q_OBJECT
public:
    BusConfigUI(QWidget *parent = Q_NULLPTR);
    ~BusConfigUI();

    friend CommunicationMatrix;

private slots:
   void on_actionClear_triggered();
   void on_actionOpen_triggered();
   void on_actionExport_triggered();
   void on_actionExit_triggered();
   void on_actionBase_triggered();
   void on_actionAttribute_definitions_triggered();
   void on_actionCommunication_matrix_triggered();
   void on_actionCan_message_simulator_triggered();
   void on_treeWidget_MainView_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
   void on_tableWidget_Properties_itemChanged(QTableWidgetItem* item);
   void on_tableWidget_Properties_cellChanged(int row, int column);
   void ShowMenuForTableWidgetItem(const QPoint& pos);
private:

   bool LoadFile(const QString& fileName);
   bool LoadDbcFile(const QString& fileName);
   void AddLog(const QString& log);
   void BuildTree(void);
   void BuildTable(void);
   void Clear(void);
   void ClearTableWidget(void);
   
   void AttachAttributesToTree(QTreeWidgetItem* parent);
   void AttachValueTableToTree(QTreeWidgetItem* parent);

   // Table widget properties
   void BuildCanMessageProperties(const QString& messageName);
   void BuildCanMessagesProperties(void);
   void BuildCanTxMessagesProperties(const QString& networkNodeName);
   void BuildCanRxMessagesProperties(const QString& networkNodeName);
   void BuildCanSignalProperties(const QString& signalName);
   void BuildCanSignalsProperties(void);
   void BuildCanMessageSignalsProperties(const QString& messageName);
   void BuildCanMappedTxSignalsProperties(const QString& networkNodeName);
   void BuildCanMappedTxMessageSignalsProperties(const QString& messageName);
   void BuildCanMappedRxSignalsProperties(const QString& networkNodeName);
   void BuildCanMappedRxMessageSignalsProperties(const QString& messageName, const QString& networkNodeName);
   void BuildCanEnvironmentVariableProperties(const QString& envVarName);
   void BuildCanEnvironmentVariablesProperties(void);
   void BuildCanValueTableProperties(const QString& canValueTableOwnerType, const QString& canValueTableOwnerName);
   void BuildAttributesProperties(const ICanAttributeOwner* attributeOwner);

   void BuildCanSignalRow(const ICanSignal* signal, int row);
   void BuildCanMessageRow(const ICanMessage* message, int row);
   void BuildCanEnvVarRow(const ICanEnvVar* envVar, int row);

   auto Find(const QString& itemName);
   void RemoveFromTreeWidget(const QString& itemName);
   void RemoveCanMessageFromTreeWidget(const QString& messageName);
   void ChangeTreeWidgetItemName(const QString& itemName, const QString& newItemName);

   void SetComboDelegateForCanSignal(void);
   void SetComboDelegateForCanMessage(void);
   void SetComboDelegateForCanEnvVar(void);

   template <std::integral Integral>
   inline void RemoveCanMessage(const Integral& integral)
   {
      canNetwork->RemoveMessageByIndex(integral);
   }

   inline void RemoveCanMessage(const QString& name)
   {
      canNetwork->RemoveMessageByName(name.toUtf8());
   }

   template <std::integral Integral>
   inline void RemoveCanSignal(const Integral& integral)
   {
      canNetwork->RemoveSignalByIndex(integral);
   }

   inline void RemoveCanSignal(const QString& name)
   {
      canNetwork->RemoveSignalByName(name.toUtf8());
   }

   template <std::integral Integral>
   inline void RemoveCanEnvVar(const Integral& integral)
   {
      canNetwork->RemoveEnvVarByIndex(integral);
   }

   inline void RemoveCanEnvVar(const QString& name)
   {
      canNetwork->RemoveEnvVarByName(name.toUtf8());
   }

   // member variables
    Ui::BusConfigUIClass ui;
    ICanBusConfig* canBusConfig { nullptr };
    CommunicationMatrix* communicationMatrix { new CommunicationMatrix {} };
    AttributeDefinitions* attributeDefinitions { new AttributeDefinitions {} };
    CanMessageSimulator* canMessageSimulator { new CanMessageSimulator {} };
    bool isTableWidgetFilled { false };
    QTreeWidgetItem* currentTreeWidgetItem { nullptr };
    bool caseSensitive { false };

    enum class Icon_e
    {
       MESSAGE = 0,
       SIGNAL = 1,
       NETWORK_NODE = 2,
       NETWORK = 3,
       ENVIRONMENT_VARIABLE = 4,
       VALUE_TABLE = 5,
       ATTRIBUTES = 6,
       HEX = 10,
       DEC = 11,
       COMMUNICATION_MATRIX = 12,
       CAN_MESSAGE_SIMULATOR = 13
    };

    enum class Base_e
    {
       DEC = 0,
       HEX = 1
    };

    QMap<Icon_e, QIcon> icons;
    Base_e base { Base_e::DEC };

    class ItemId
    {
    public:
       ItemId() = delete;

       static constexpr std::string_view CAN_SIGNAL = "CanSignal";
       static constexpr std::string_view CAN_SIGNALS = "CanSignals";
       static constexpr std::string_view CAN_MESSAGE = "CanMessage";
       static constexpr std::string_view CAN_MESSAGES = "CanMessages";
       static constexpr std::string_view CAN_TX_MESSAGES = "CanTxMessages";
       static constexpr std::string_view CAN_RX_MESSAGES = "CanRxMessages";
       static constexpr std::string_view CAN_MESSAGE_SIGNALS = "CanMessageSignals";
       static constexpr std::string_view CAN_MAPPED_TX_SIGNALS = "CanMappedTxSignals";
       static constexpr std::string_view CAN_MAPPED_TX_MESSAGE_SIGNALS = "CanMappedTxMessageSignals";
       static constexpr std::string_view CAN_MAPPED_RX_SIGNALS = "CanMappedRxSignals";
       static constexpr std::string_view CAN_MAPPED_RX_MESSAGE_SIGNALS = "CanMappedRxMessageSignals";
       static constexpr std::string_view CAN_NETWORK_NODE = "CanNetworkNode";
       static constexpr std::string_view CAN_NETWORK_NODES = "CanNetworkNodes";
       static constexpr std::string_view CAN_ENVIRONMENT_VARIABLE = "CanEnvironmentVariable";
       static constexpr std::string_view CAN_ENVIRONMENT_VARIABLES = "CanEnvironmentVariables";
       static constexpr std::string_view NETWORKS = "Networks";
       static constexpr std::string_view NETWORK = "Network";
       static constexpr std::string_view ATTRIBUTES = "Attributes";
       static constexpr std::string_view VALUE_TABLE = "Value table";
    };
};
