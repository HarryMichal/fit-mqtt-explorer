#include "dashboardwidget.h"
#include "ui_dashboardwidget.h"

#include <QList>
#include <QRegularExpression>

DashboardWidget::DashboardWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dashboardWidget)

{
    ui->setupUi(this);

    DashboardWidget::setupActions();
}

DashboardWidget::~DashboardWidget()
{
    delete ui;
}

void DashboardWidget::setupActions()
{
    this->ui->setpointLabel->setVisible(false);
    this->ui->setpointValue->setVisible(false);

    connect(this->ui->removeButton, &QPushButton::clicked, this, &DashboardWidget::RemoveWidget);
    connect(this->ui->editButton, &QPushButton::clicked, this, &DashboardWidget::EditWidget);
}

void DashboardWidget::RemoveWidget()
{
     delete this;
}

void DashboardWidget::EditWidget()
{
    widgetStructure widgetStructure;
    widgetStructure.name = this->ui->nameLabel->text();
    widgetStructure.topic = this->ui->topicLabel->text();
    widgetStructure.type = this->ui->typeLabel->text();
    widgetStructure.setpoint = this->ui->setpointLabel->isVisible();
    widgetStructure.setpointValue = this->ui->setpointValue->text().toDouble();

    this->editWidget = new class EditWidget(widgetStructure, this);

    connect(this->editWidget, &EditWidget::applyChangesOnWidget, this, &DashboardWidget::ApplyChanges);

    this->editWidget->show();

}

void DashboardWidget::ApplyChanges(widgetStructure widgetStructure)
{
    this->ui->nameLabel->setText(widgetStructure.name);
    this->ui->typeLabel->setText(widgetStructure.type);

    if(widgetStructure.setpoint == true) {
        this->ui->setpointLabel->setVisible(true);
        this->ui->setpointValue->setText(widgetStructure.setpointValue);
        this->ui->setpointValue->setVisible(true);
    }
    else {
        this->ui->setpointLabel->setVisible(false);
        this->ui->setpointValue->setVisible(false);
    }

    this->ui->topicLabel->setText(widgetStructure.topic);

}

void DashboardWidget::UpdateNewValue(const QHash<QString, QList<Message*>> new_msgs)
{
    QString topic = ui->topicLabel->text();
    QList<Message*> messageList = new_msgs.value(topic);
    if(!messageList.isEmpty()){
        ui->valueLabel->setText(QString::fromStdString(messageList.last()->getMessage()));
    }
    setValueFormat();
}

void DashboardWidget::setValueFormat()
{
    QString type = ui->typeLabel->text();
    QString value = ui->valueLabel->text();
    if (type == "Temperature")
    {

    }
}
