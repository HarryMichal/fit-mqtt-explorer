/**
 * @file dashboardpage.cpp
 * @author Žaneta Grossová <xgross11>
 */

#include "dashboardpage.h"
#include "ui_dashboardpage.h"
#include "dashboardwidget.h"
#include "ui_dashboardwidget.h"

#include <QtWidgets>

DashboardPage::DashboardPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DashboardPage),
    layout(new QVBoxLayout)


{
    ui->setupUi(this);

    QList<QWidget*> widgetList;
    setupActions();

}

DashboardPage::~DashboardPage()
{
    delete ui;
}

void DashboardPage::setupActions()
{
    layout = qobject_cast<QVBoxLayout*>(ui->scrollArea->layout());
}

void DashboardPage::AddNewWidget()
{
    QWidget* widget = new DashboardWidget;
    widget->setStyleSheet(".QFrame{border-radius: 6px;}");
    layout->insertWidget(layout->count(), widget);
    widgetList.insert(0, widget);
}

void DashboardPage::receiveNewMessages(const QHash<QString, QList<Message*>> new_msgs)
{
    foreach (QWidget* widget, widgetList)
    {
        ((DashboardWidget*)widget)->UpdateNewValue(new_msgs);
    }

}
