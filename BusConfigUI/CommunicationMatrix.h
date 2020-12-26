#pragma once

#include "ICanBusConfig.h"
#include <QDialog>

namespace Ui
{
   class CommunicationMatrix;
}

class CommunicationMatrix : public QDialog
{
   Q_OBJECT
public:
    explicit CommunicationMatrix(QWidget* parent = nullptr);
    ~CommunicationMatrix();

    bool Create(ICanBusConfig* canBusConfig);

private:
   Ui::CommunicationMatrix* ui;
};
