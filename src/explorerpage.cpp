#include "explorerpage.h"
#include "ui_explorerpage.h"

ExplorerPage::ExplorerPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExplorerPage)
{
    ui->setupUi(this);
}

ExplorerPage::~ExplorerPage()
{
    delete ui;
}
