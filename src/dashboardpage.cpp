#include "dashboardpage.h"
#include "ui_dashboardpage.h"

DashboardPage::DashboardPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DashboardPage)
{
    ui->setupUi(this);
}

DashboardPage::~DashboardPage()
{
    delete ui;
}
