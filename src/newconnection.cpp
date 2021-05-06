#include "newconnection.h"
#include "ui_newconnection.h"

NewConnection::NewConnection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newconnection)
{
    ui->setupUi(this);

    NewConnection::setupSignals();
}

NewConnection::~NewConnection()
{
    delete ui;
}


void NewConnection::setupSignals()
{
    connect(this->ui->connectButt, &QPushButton::released, this, &NewConnection::createNewConnection);
    connect(this->ui->connectButt, &QPushButton::released, this, &QDialog::accept);
}
