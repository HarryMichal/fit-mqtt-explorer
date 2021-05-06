#include <QString>

#include "newconnection.h"
#include "ui_newconnection.h"

NewConnection::NewConnection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newconnection)
{
    ui->setupUi(this);

    NewConnection::setupActions();
    NewConnection::loadConnections();
}

NewConnection::~NewConnection()
{
    delete ui;
}

void NewConnection::saveConnection()
{

}

void NewConnection::loadConnections()
{

}

void NewConnection::deleteConnection()
{

}

QString NewConnection::connectToContent()
{
    ServerName serverName;
    serverName.host = this->ui->hostLineEdit->text();
    serverName.port = this->ui->comboBox->currentText().toInt();

    QString adress = serverName.host + ":" + serverName.port;

    this->createNewConnection();
    return adress;
}

void NewConnection::setupActions()
{
    //connect(this->ui->connectButt, &QPushButton::released, this, &NewConnection::createNewConnection);
    connect(this->ui->connectButt, &QPushButton::released, this, &NewConnection::connectToContent);
    connect(this->ui->connectButt, &QPushButton::released, this, &QDialog::accept);
    connect(this->ui->saveButt, &QPushButton::released, this, &NewConnection::saveConnection);
}
