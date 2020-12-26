#pragma once

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

private:
   Ui::CommunicationMatrix* ui;
};
