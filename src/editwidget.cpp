#include "editwidget.h"
#include "ui_editwidget.h"

EditWidget::EditWidget(widgetStructure widgetStructure, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditWidget)
{
    ui->setupUi(this);
    this->ui->setpointValue->setEnabled(false);

    setupActions(widgetStructure);
}

EditWidget::~EditWidget()
{
    delete ui;
}

void EditWidget::setupActions(widgetStructure widgetStructure)
{
    this->ui->nameEdit->setText(widgetStructure.name);
    this->ui->setpointCheckBox->setChecked(widgetStructure.setpoint);
    if(this->ui->setpointCheckBox->isChecked()) {
        ui->setpointValue->setText(widgetStructure.setpointValue);
        ui->setpointValue->setEnabled(true);
    }

    this->ui->typeOptions->setCurrentText(widgetStructure.type);
    this->ui->topicOptions->setCurrentText(widgetStructure.topic);

    connect(this->ui->cancelButton, &QPushButton::clicked, this, &EditWidget::reject);
    connect(this->ui->saveButton, &QPushButton::clicked, this, &EditWidget::setWidgetSettings);
    connect(this->ui->saveButton, &QPushButton::clicked, this, &EditWidget::accept);
    connect(this->ui->setpointCheckBox, &QCheckBox::clicked, this, &EditWidget::checkBoxStatusChanged);
}

void EditWidget::setWidgetSettings()
{
    widgetStructure widgetStructure;
    widgetStructure.name = this->ui->nameEdit->text();
    widgetStructure.setpoint = this->ui->setpointCheckBox->isChecked();
    if(this->ui->setpointCheckBox->isChecked() == true) {
        //widgetStructure.setpointValue = this->ui->setpointValue->value();
    }
    widgetStructure.topic = this->ui->topicOptions->currentText();
    widgetStructure.type = this->ui->typeOptions->currentText();

    emit applyChangesOnWidget(widgetStructure);
}

void EditWidget::checkBoxStatusChanged()
{
    if(this->ui->setpointCheckBox->isChecked())
    {
        ui->setpointValue->setEnabled(true);
    }
    else this->ui->setpointValue->setEnabled(false);
}
