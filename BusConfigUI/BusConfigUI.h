#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_BusConfigUI.h"
#include "ICanBusConfig.h"

class BusConfigUI : public QMainWindow
{
    Q_OBJECT

public:
    BusConfigUI(QWidget *parent = Q_NULLPTR);


private slots:
   void on_actionOpen_triggered();
private:

   bool LoadFile(const QString& fileName);

   // member variables
    Ui::BusConfigUIClass ui;
    ICanBusConfig* canBusConfig;
};
