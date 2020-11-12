#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_BusConfigUI.h"

class BusConfigUI : public QMainWindow
{
    Q_OBJECT

public:
    BusConfigUI(QWidget *parent = Q_NULLPTR);

private:
    Ui::BusConfigUIClass ui;
};
