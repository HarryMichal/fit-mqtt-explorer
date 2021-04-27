#include "newconnection.h"
#include "ui_newconnection.h"

NewConnection::NewConnection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newconnection)
{
    ui->setupUi(this);
}

NewConnection::~NewConnection()
{
    delete ui;
}
