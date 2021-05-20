/**
 * @file newconnection.cpp
 * @author Žaneta Grossová <xgross11>
 */

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

void NewConnection::processForm()
{
    ServerName serverName;
    serverName.host = this->ui->hostLineEdit->text();
    serverName.port = this->ui->portNumber->currentText().toInt();

    QString fullConnectionAdress = serverName.host + ":" + QString::number(serverName.port);

    QString client_id = this->ui->NameLineEdit->text();
    if (client_id == "") {
        client_id = "fit-mqtt-client";
    }

    emit createNewConnection(fullConnectionAdress, client_id);
}

void NewConnection::setupActions()
{
    connect(this->ui->connectButt, &QPushButton::released, this, &NewConnection::processForm);
    connect(this->ui->connectButt, &QPushButton::released, this, &QDialog::accept);
    connect(this->ui->saveButt, &QPushButton::released, this, &NewConnection::saveConnection);
}
