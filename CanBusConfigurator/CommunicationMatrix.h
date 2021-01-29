#pragma once

#include "ICanNetwork.h"
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

    bool Create(ICanNetwork* canNetwork);

private:
   Ui::CommunicationMatrix* ui;
};
