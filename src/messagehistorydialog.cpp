#include "messagehistorydialog.h"
#include "ui_messagehistorydialog.h"

MessageHistoryDialog::MessageHistoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageHistoryDialog)
{
    this->ui->setupUi(this);
}

MessageHistoryDialog::~MessageHistoryDialog()
{
    delete this->ui;
}

int MessageHistoryDialog::getValue()
{
    return this->ui->spinBox->value();
}

void MessageHistoryDialog::setValue(int value)
{
    this->ui->spinBox->setValue(value);
}
